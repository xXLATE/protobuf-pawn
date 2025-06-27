package generator

import (
	"fmt"
	"strings"

	"google.golang.org/protobuf/compiler/protogen"
	"google.golang.org/protobuf/reflect/protoreflect"
)

// GeneratePawnSerializationFile generates Pack/Unpack functions for each message
func GeneratePawnSerializationFile(gen *protogen.Plugin, file *protogen.File) *protogen.GeneratedFile {
	g := gen.NewGeneratedFile(file.GeneratedFilenamePrefix+"_serialization.inc", file.GoImportPath)

	// Header comment
	g.P("// Protobuf serialization functions for Pawn")
	g.P("// Generated from: ", file.Desc.Path())
	g.P()

	// Generate utility functions first
	generateSerializationUtils(g)

	// Generate Pack/Unpack functions for each message (including nested)
	for _, message := range file.Messages {
		generateMessageFunctions(g, message)
	}

	// Generate helper functions for finding structures by ID
	generateHelperFunctions(g, file)

	return g
}

// generateSerializationUtils generates utility functions for protobuf encoding
func generateSerializationUtils(g *protogen.GeneratedFile) {
	g.P("// ========== Protobuf Utility Functions ==========")
	g.P()

	// Varint encoding function
	g.P("// Encode varint value to buffer")
	g.P("stock EncodeVarint(value, buffer[], &offset) {")
	g.P("    new temp = value;")
	g.P("    while (temp >= 0x80) {")
	g.P("        buffer[offset++] = (temp & 0x7F) | 0x80;")
	g.P("        temp >>>= 7;")
	g.P("    }")
	g.P("    buffer[offset++] = temp & 0x7F;")
	g.P("}")
	g.P()

	// Varint decoding function
	g.P("// Decode varint value from buffer")
	g.P("stock DecodeVarint(const buffer[], &offset, maxOffset) {")
	g.P("    new result = 0;")
	g.P("    new shift = 0;")
	g.P("    new byte;")
	g.P("    while (offset < maxOffset) {")
	g.P("        byte = buffer[offset++];")
	g.P("        result |= (byte & 0x7F) << shift;")
	g.P("        if ((byte & 0x80) == 0) break;")
	g.P("        shift += 7;")
	g.P("        if (shift >= 32) break; // Prevent overflow")
	g.P("    }")
	g.P("    return result;")
	g.P("}")
	g.P()

	// Fixed32 encoding/decoding
	g.P("// Encode 32-bit fixed value to buffer")
	g.P("stock EncodeFixed32(value, buffer[], &offset) {")
	g.P("    buffer[offset++] = value & 0xFF;")
	g.P("    buffer[offset++] = (value >>> 8) & 0xFF;")
	g.P("    buffer[offset++] = (value >>> 16) & 0xFF;")
	g.P("    buffer[offset++] = (value >>> 24) & 0xFF;")
	g.P("}")
	g.P()

	g.P("// Decode 32-bit fixed value from buffer")
	g.P("stock DecodeFixed32(const buffer[], &offset) {")
	g.P("    new result = buffer[offset] |")
	g.P("        (buffer[offset + 1] << 8) |")
	g.P("        (buffer[offset + 2] << 16) |")
	g.P("        (buffer[offset + 3] << 24);")
	g.P("    offset += 4;")
	g.P("    return result;")
	g.P("}")
	g.P()

	// Fixed64 encoding/decoding
	g.P("// Encode 64-bit fixed value to buffer")
	g.P("stock EncodeFixed64(value, buffer[], &offset) {")
	g.P("    // Note: Pawn doesn't have native 64-bit support, treating as float")
	g.P("    new low = _:value;")
	g.P("    new high = 0; // Upper 32 bits set to 0 for simplicity")
	g.P("    buffer[offset++] = low & 0xFF;")
	g.P("    buffer[offset++] = (low >>> 8) & 0xFF;")
	g.P("    buffer[offset++] = (low >>> 16) & 0xFF;")
	g.P("    buffer[offset++] = (low >>> 24) & 0xFF;")
	g.P("    buffer[offset++] = high & 0xFF;")
	g.P("    buffer[offset++] = (high >>> 8) & 0xFF;")
	g.P("    buffer[offset++] = (high >>> 16) & 0xFF;")
	g.P("    buffer[offset++] = (high >>> 24) & 0xFF;")
	g.P("}")
	g.P()

	g.P("// Decode 64-bit fixed value from buffer")
	g.P("stock Float:DecodeFixed64(const buffer[], &offset) {")
	g.P("    // Note: Pawn doesn't have native 64-bit support, reading only lower 32 bits")
	g.P("    new result = buffer[offset] |")
	g.P("        (buffer[offset + 1] << 8) |")
	g.P("        (buffer[offset + 2] << 16) |")
	g.P("        (buffer[offset + 3] << 24);")
	g.P("    offset += 8; // Skip all 8 bytes")
	g.P("    return Float:result;")
	g.P("}")
	g.P()

	// String encoding/decoding
	g.P("// Encode string to buffer")
	g.P("stock EncodeString(const str[], buffer[], &offset) {")
	g.P("    new len = strlen(str);")
	g.P("    EncodeVarint(len, buffer, offset);")
	g.P("    for (new i = 0; i < len; i++) {")
	g.P("        buffer[offset++] = str[i];")
	g.P("    }")
	g.P("}")
	g.P()

	g.P("// Decode string from buffer")
	g.P("stock DecodeString(const buffer[], &offset, maxOffset, dest[], maxlen) {")
	g.P("    new len = DecodeVarint(buffer, offset, maxOffset);")
	g.P("    if (len >= maxlen) len = maxlen - 1;")
	g.P("    for (new i = 0; i < len && offset < maxOffset; i++) {")
	g.P("        dest[i] = buffer[offset++];")
	g.P("    }")
	g.P("    dest[len] = 0;")
	g.P("    return len;")
	g.P("}")
	g.P()

	// Wire type constants
	g.P("// Protobuf wire types")
	g.P("#define WIRE_TYPE_VARINT    0")
	g.P("#define WIRE_TYPE_FIXED64   1")
	g.P("#define WIRE_TYPE_LENGTH_DELIMITED 2")
	g.P("#define WIRE_TYPE_FIXED32   5")
	g.P()

	// Tag encoding
	g.P("// Encode field tag (field number + wire type)")
	g.P("stock EncodeTag(fieldNumber, wireType, buffer[], &offset) {")
	g.P("    EncodeVarint((fieldNumber << 3) | wireType, buffer, offset);")
	g.P("}")
	g.P()

	g.P("// Decode field tag")
	g.P("stock DecodeTag(const buffer[], &offset, maxOffset, &fieldNumber, &wireType) {")
	g.P("    new tag = DecodeVarint(buffer, offset, maxOffset);")
	g.P("    fieldNumber = tag >>> 3;")
	g.P("    wireType = tag & 0x7;")
	g.P("    return tag;")
	g.P("}")
	g.P()
}

// generateMessageFunctions generates Pack/Unpack functions for message and its nested messages
func generateMessageFunctions(g *protogen.GeneratedFile, message *protogen.Message) {
	// First generate functions for nested messages
	for _, nested := range message.Messages {
		generateMessageFunctions(g, nested)
	}

	// Then generate functions for this message
	generateMessagePackFunction(g, message)
	generateMessageUnpackFunction(g, message)
}

// generateMessagePackFunction generates Pack function for a message
func generateMessagePackFunction(g *protogen.GeneratedFile, message *protogen.Message) {
	messageName := message.GoIdent.GoName
	enumName := "e" + messageName

	// Collect dependencies
	nestedTypes, hasRepeatedStrings := collectMessageDependencies(message, nil)

	g.P("// ========== ", messageName, " Pack Function ==========")
	g.P("// Pack ", messageName, " structure into byte buffer")

	// Generate function signature with all needed arrays
	signature := fmt.Sprintf("stock Pack%s(const data[%s], buffer[], &offset", messageName, enumName)

	// Add repeated strings array if needed
	if hasRepeatedStrings {
		signature += ", const repeatedStrings[][E_REPEATED_STRING], maxRepeatedStrings"
	}

	// Add nested message arrays
	for _, nestedType := range nestedTypes {
		signature += fmt.Sprintf(", const %ss[][e%s], max%ss", strings.ToLower(nestedType), nestedType, nestedType)
	}

	signature += ") {"
	g.P(signature)
	g.P()

	// Generate packing code for each field
	for _, field := range message.Fields {
		generateFieldPackCode(g, field, messageName)
	}

	g.P("}")
	g.P()
}

// Collect all dependencies (nested message types and repeated strings) for a message
func collectMessageDependencies(message *protogen.Message, visited map[string]bool) ([]string, bool) {
	if visited == nil {
		visited = make(map[string]bool)
	}

	messageName := message.GoIdent.GoName
	if visited[messageName] {
		return nil, false
	}
	visited[messageName] = true

	var nestedTypes []string
	hasRepeatedStrings := false

	for _, field := range message.Fields {
		if field.Desc.IsList() && isStringField(field) {
			hasRepeatedStrings = true
		}

		if field.Desc.Kind() == protoreflect.MessageKind {
			nestedTypeName := field.Message.GoIdent.GoName
			// Add this nested type
			found := false
			for _, existing := range nestedTypes {
				if existing == nestedTypeName {
					found = true
					break
				}
			}
			if !found {
				nestedTypes = append(nestedTypes, nestedTypeName)
			}

			// Recursively collect dependencies of nested type
			subNested, subHasRepeated := collectMessageDependencies(field.Message, visited)
			for _, subType := range subNested {
				found := false
				for _, existing := range nestedTypes {
					if existing == subType {
						found = true
						break
					}
				}
				if !found {
					nestedTypes = append(nestedTypes, subType)
				}
			}
			if subHasRepeated {
				hasRepeatedStrings = true
			}
		}
	}

	return nestedTypes, hasRepeatedStrings
}

func generateMessageUnpackFunction(g *protogen.GeneratedFile, message *protogen.Message) {
	messageName := message.GoIdent.GoName
	enumName := "e" + messageName

	// Collect dependencies
	nestedTypes, hasRepeatedStrings := collectMessageDependencies(message, nil)

	g.P("// ========== ", messageName, " Unpack Function ==========")
	g.P("// Unpack ", messageName, " structure from byte buffer")

	// Generate function signature with all needed arrays
	signature := fmt.Sprintf("stock Unpack%s(const buffer[], bufferSize, data[%s]", messageName, enumName)

	// Add repeated strings array if needed
	if hasRepeatedStrings {
		signature += ", repeatedStrings[][E_REPEATED_STRING], &maxRepeatedStrings"
	}

	// Add nested message arrays
	for _, nestedType := range nestedTypes {
		signature += fmt.Sprintf(", %ss[][e%s], &max%ss", strings.ToLower(nestedType), nestedType, nestedType)
	}

	signature += ") {"
	g.P(signature)

	g.P("    new offset = 0;")
	g.P("    new fieldNumber, wireType;")
	g.P()

	g.P("    // Clear the data structure")
	for i, field := range message.Fields {
		enumFieldName := generateCorrectEnumFieldName(field, messageName)
		fieldName := convertFieldName(string(field.Desc.Name()))

		if isStringField(field) && !field.Desc.IsList() {
			g.P("    data[", enumFieldName, "][0] = 0;")
		} else if field.Desc.Kind() == protoreflect.BoolKind && !field.Desc.IsList() {
			g.P("    data[", enumFieldName, "] = false;")
		} else if field.Desc.IsList() {
			g.P("    // Clear array for ", fieldName)
			g.P("    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			if isStringField(field) {
				g.P("        data[", enumFieldName, "][i][0] = 0;")
			} else if field.Desc.Kind() == protoreflect.BoolKind {
				g.P("        data[", enumFieldName, "][i] = false;")
			} else {
				g.P("        data[", enumFieldName, "][i] = 0;")
			}
			g.P("    }")
		} else {
			g.P("    data[", enumFieldName, "] = 0;")
		}
		if i == len(message.Fields)-1 {
			g.P()
		}
	}

	g.P("    while (offset < bufferSize) {")
	g.P("        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);")
	g.P("        ")
	g.P("        switch (fieldNumber) {")

	// Generate unpacking code for each field
	for _, field := range message.Fields {
		generateFieldUnpackCode(g, field, messageName)
	}

	g.P("            default: {")
	g.P("                // Unknown field, skip it")
	g.P("                switch (wireType) {")
	g.P("                    case WIRE_TYPE_VARINT: DecodeVarint(buffer, offset, bufferSize);")
	g.P("                    case WIRE_TYPE_FIXED32: offset += 4;")
	g.P("                    case WIRE_TYPE_FIXED64: offset += 8;")
	g.P("                    case WIRE_TYPE_LENGTH_DELIMITED: {")
	g.P("                        new len = DecodeVarint(buffer, offset, bufferSize);")
	g.P("                        offset += len;")
	g.P("                    }")
	g.P("                }")
	g.P("            }")
	g.P("        }")
	g.P("    }")
	g.P("    return 1; // Success")
	g.P("}")
	g.P()
}

// Helper function to check if field is string type
func isStringField(field *protogen.Field) bool {
	return field.Desc.Kind() == protoreflect.StringKind || field.Desc.Kind() == protoreflect.BytesKind
}

// generateFieldPackCode generates packing code using correct enum field names
func generateFieldPackCode(g *protogen.GeneratedFile, field *protogen.Field, messageName string) {
	fieldNumber := field.Desc.Number()
	enumFieldName := generateCorrectEnumFieldName(field, messageName)

	g.P("    // Field ", fieldNumber, ": ", field.Desc.Name())

	if field.Desc.IsList() {
		// Repeated fields - working code without comments
		switch field.Desc.Kind() {
		case protoreflect.StringKind:
			g.P("    // Pack repeated string array using IDs")
			g.P("    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("        if (data[", enumFieldName, "][i] != 0) {")
			g.P("            new tempString[256];")
			g.P("            if (GetRepeatedStringById(data[", enumFieldName, "][i], repeatedStrings, maxRepeatedStrings, tempString)) {")
			g.P("                EncodeTag(", fieldNumber, ", WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);")
			g.P("                EncodeString(tempString, buffer, offset);")
			g.P("            }")
			g.P("        }")
			g.P("    }")
		case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Uint32Kind,
			protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Uint64Kind,
			protoreflect.Sfixed32Kind, protoreflect.Fixed32Kind,
			protoreflect.Sfixed64Kind, protoreflect.Fixed64Kind:
			g.P("    // Pack repeated int array directly")
			g.P("    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("        if (data[", enumFieldName, "][i] != 0) {")
			g.P("            EncodeTag(", fieldNumber, ", WIRE_TYPE_VARINT, buffer, offset);")
			g.P("            EncodeVarint(data[", enumFieldName, "][i], buffer, offset);")
			g.P("        }")
			g.P("    }")
		case protoreflect.FloatKind:
			g.P("    // Pack repeated float array directly")
			g.P("    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("        if (data[", enumFieldName, "][i] != 0.0) {")
			g.P("            EncodeTag(", fieldNumber, ", WIRE_TYPE_FIXED32, buffer, offset);")
			g.P("            EncodeFixed32(_:data[", enumFieldName, "][i], buffer, offset);")
			g.P("        }")
			g.P("    }")
		case protoreflect.DoubleKind:
			g.P("    // Pack repeated double array directly")
			g.P("    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("        if (data[", enumFieldName, "][i] != 0.0) {")
			g.P("            EncodeTag(", fieldNumber, ", WIRE_TYPE_FIXED64, buffer, offset);")
			g.P("            EncodeFixed64(_:data[", enumFieldName, "][i], buffer, offset);")
			g.P("        }")
			g.P("    }")
		case protoreflect.BoolKind:
			g.P("    // Pack repeated bool array directly")
			g.P("    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("        if (data[", enumFieldName, "][i]) {")
			g.P("            EncodeTag(", fieldNumber, ", WIRE_TYPE_VARINT, buffer, offset);")
			g.P("            EncodeVarint(data[", enumFieldName, "][i] ? 1 : 0, buffer, offset);")
			g.P("        }")
			g.P("    }")
		case protoreflect.EnumKind:
			g.P("    // Pack repeated enum array directly")
			g.P("    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("        if (data[", enumFieldName, "][i] != 0) {")
			g.P("            EncodeTag(", fieldNumber, ", WIRE_TYPE_VARINT, buffer, offset);")
			g.P("            EncodeVarint(_:data[", enumFieldName, "][i], buffer, offset);")
			g.P("        }")
			g.P("    }")
		case protoreflect.MessageKind:
			nestedMessageName := field.Message.GoIdent.GoName
			if field.Desc.IsList() {
				// Pack repeated nested message using ID array
				g.P("    // Pack repeated message field ", field.Desc.Name(), " using ID array")
				g.P("    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
				g.P("        if (data[", enumFieldName, "][i] != 0) {")
				g.P("            new temp", nestedMessageName, "[e", nestedMessageName, "];")
				g.P("            if (Get", nestedMessageName, "ById(data[", enumFieldName, "][i], ", strings.ToLower(nestedMessageName), "s, max", nestedMessageName, "s, temp", nestedMessageName, ")) {")
				g.P("                // Pack as length-delimited submessage")
				g.P("                new subBuffer[1024];")
				g.P("                new subOffset = 0;")
				g.P("                Pack", nestedMessageName, "(temp", nestedMessageName, ", subBuffer, subOffset);")
				g.P("                EncodeTag(", fieldNumber, ", WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);")
				g.P("                EncodeVarint(subOffset, buffer, offset);")
				g.P("                for (new j = 0; j < subOffset; j++) {")
				g.P("                    buffer[offset++] = subBuffer[j];")
				g.P("                }")
				g.P("            }")
				g.P("        }")
				g.P("    }")
			} else {
				// Pack single nested message using ID
				g.P("    // Pack nested message field ", field.Desc.Name(), " using ID")
				g.P("    if (data[", enumFieldName, "] != 0) {")
				g.P("        new temp", nestedMessageName, "[e", nestedMessageName, "];")
				g.P("        if (Get", nestedMessageName, "ById(data[", enumFieldName, "], ", strings.ToLower(nestedMessageName), "s, max", nestedMessageName, "s, temp", nestedMessageName, ")) {")
				g.P("            // Pack as length-delimited submessage")
				g.P("            new subBuffer[1024];")
				g.P("            new subOffset = 0;")
				g.P("            Pack", nestedMessageName, "(temp", nestedMessageName, ", subBuffer, subOffset);")
				g.P("            EncodeTag(", fieldNumber, ", WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);")
				g.P("            EncodeVarint(subOffset, buffer, offset);")
				g.P("            for (new i = 0; i < subOffset; i++) {")
				g.P("                buffer[offset++] = subBuffer[i];")
				g.P("            }")
				g.P("        }")
				g.P("    }")
			}
		}
	} else {
		// Single fields - working code without comments
		switch field.Desc.Kind() {
		case protoreflect.BoolKind:
			g.P("    if (data[", enumFieldName, "]) {")
			g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_VARINT, buffer, offset);")
			g.P("        EncodeVarint(data[", enumFieldName, "] ? 1 : 0, buffer, offset);")
			g.P("    }")
		case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Uint32Kind,
			protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Uint64Kind,
			protoreflect.Sfixed32Kind, protoreflect.Fixed32Kind,
			protoreflect.Sfixed64Kind, protoreflect.Fixed64Kind:
			g.P("    if (data[", enumFieldName, "] != 0) {")
			g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_VARINT, buffer, offset);")
			g.P("        EncodeVarint(data[", enumFieldName, "], buffer, offset);")
			g.P("    }")
		case protoreflect.FloatKind:
			g.P("    if (data[", enumFieldName, "] != 0.0) {")
			g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_FIXED32, buffer, offset);")
			g.P("        EncodeFixed32(_:data[", enumFieldName, "], buffer, offset);")
			g.P("    }")
		case protoreflect.DoubleKind:
			g.P("    if (data[", enumFieldName, "] != 0.0) {")
			g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_FIXED64, buffer, offset);")
			g.P("        EncodeFixed64(_:data[", enumFieldName, "], buffer, offset);")
			g.P("    }")
		case protoreflect.StringKind, protoreflect.BytesKind:
			g.P("    if (strlen(data[", enumFieldName, "]) > 0) {")
			g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);")
			g.P("        EncodeString(data[", enumFieldName, "], buffer, offset);")
			g.P("    }")
		case protoreflect.EnumKind:
			g.P("    if (data[", enumFieldName, "] != 0) {")
			g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_VARINT, buffer, offset);")
			g.P("        EncodeVarint(_:data[", enumFieldName, "], buffer, offset);")
			g.P("    }")
		case protoreflect.MessageKind:
			nestedMessageName := field.Message.GoIdent.GoName
			if field.Desc.IsList() {
				g.P("    // Pack repeated message field ", field.Desc.Name(), " using ID array")
				g.P("    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
				g.P("        if (data[", enumFieldName, "][i] != 0) {")
				g.P("            new temp", nestedMessageName, "[e", nestedMessageName, "];")
				g.P("            if (Get", nestedMessageName, "ById(data[", enumFieldName, "][i], ", strings.ToLower(nestedMessageName), "s, max", nestedMessageName, "s, temp", nestedMessageName, ")) {")
				g.P("                // Pack as length-delimited submessage")
				g.P("                new subBuffer[1024];")
				g.P("                new subOffset = 0;")
				g.P("                Pack", nestedMessageName, "(temp", nestedMessageName, ", subBuffer, subOffset);")
				g.P("                EncodeTag(", fieldNumber, ", WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);")
				g.P("                EncodeVarint(subOffset, buffer, offset);")
				g.P("                for (new j = 0; j < subOffset; j++) {")
				g.P("                    buffer[offset++] = subBuffer[j];")
				g.P("                }")
				g.P("            }")
				g.P("        }")
				g.P("    }")
			} else {
				g.P("    // Pack nested message field ", field.Desc.Name(), " using ID")
				g.P("    if (data[", enumFieldName, "] != 0) {")
				g.P("        new temp", nestedMessageName, "[e", nestedMessageName, "];")
				g.P("        if (Get", nestedMessageName, "ById(data[", enumFieldName, "], ", strings.ToLower(nestedMessageName), "s, max", nestedMessageName, "s, temp", nestedMessageName, ")) {")
				g.P("            // Pack as length-delimited submessage")
				g.P("            new subBuffer[1024];")
				g.P("            new subOffset = 0;")
				g.P("            Pack", nestedMessageName, "(temp", nestedMessageName, ", subBuffer, subOffset);")
				g.P("            EncodeTag(", fieldNumber, ", WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);")
				g.P("            EncodeVarint(subOffset, buffer, offset);")
				g.P("            for (new i = 0; i < subOffset; i++) {")
				g.P("                buffer[offset++] = subBuffer[i];")
				g.P("            }")
				g.P("        }")
				g.P("    }")
			}
		}
	}
	g.P()
}

// generateFieldUnpackCode generates unpacking code using correct enum field names
func generateFieldUnpackCode(g *protogen.GeneratedFile, field *protogen.Field, messageName string) {
	fieldNumber := field.Desc.Number()
	enumFieldName := generateCorrectEnumFieldName(field, messageName)

	g.P("            case ", fieldNumber, ": {")
	g.P("                // ", field.Desc.Name())

	if field.Desc.IsList() {
		// Repeated fields unpacking
		switch field.Desc.Kind() {
		case protoreflect.StringKind:
			g.P("                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {")
			g.P("                    // Add repeated string to global repeatedStrings array")
			g.P("                    new tempString[256];")
			g.P("                    DecodeString(buffer, offset, bufferSize, tempString, 256);")
			g.P("                    // Add string to repeatedStrings array")
			g.P("                    if (maxRepeatedStrings < sizeof(repeatedStrings)) {")
			g.P("                        strcopy(repeatedStrings[maxRepeatedStrings][E_REPEATED_STRING_Text], tempString, 256);")
			g.P("                        repeatedStrings[maxRepeatedStrings][E_REPEATED_STRING_Id] = maxRepeatedStrings + 1;")
			g.P("                        // Find empty slot in ID array and assign string ID")
			g.P("                        for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("                            if (data[", enumFieldName, "][i] == 0) {")
			g.P("                                data[", enumFieldName, "][i] = repeatedStrings[maxRepeatedStrings][E_REPEATED_STRING_Id];")
			g.P("                                break;")
			g.P("                            }")
			g.P("                        }")
			g.P("                        maxRepeatedStrings++;")
			g.P("                    }")
			g.P("                }")
		case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Uint32Kind,
			protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Uint64Kind,
			protoreflect.Sfixed32Kind, protoreflect.Fixed32Kind,
			protoreflect.Sfixed64Kind, protoreflect.Fixed64Kind:
			g.P("                if (wireType == WIRE_TYPE_VARINT) {")
			g.P("                    // Find empty slot in array")
			g.P("                    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("                        if (data[", enumFieldName, "][i] == 0) {")
			g.P("                            data[", enumFieldName, "][i] = DecodeVarint(buffer, offset, bufferSize);")
			g.P("                            break;")
			g.P("                        }")
			g.P("                    }")
			g.P("                }")
		case protoreflect.FloatKind:
			g.P("                if (wireType == WIRE_TYPE_FIXED32) {")
			g.P("                    // Find empty slot in array")
			g.P("                    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("                        if (data[", enumFieldName, "][i] == 0.0) {")
			g.P("                            data[", enumFieldName, "][i] = Float:DecodeFixed32(buffer, offset);")
			g.P("                            break;")
			g.P("                        }")
			g.P("                    }")
			g.P("                }")
		case protoreflect.DoubleKind:
			g.P("                if (wireType == WIRE_TYPE_FIXED64) {")
			g.P("                    // Find empty slot in array")
			g.P("                    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("                        if (data[", enumFieldName, "][i] == 0.0) {")
			g.P("                            data[", enumFieldName, "][i] = Float:DecodeFixed64(buffer, offset);")
			g.P("                            break;")
			g.P("                        }")
			g.P("                    }")
			g.P("                }")
		case protoreflect.BoolKind:
			g.P("                if (wireType == WIRE_TYPE_VARINT) {")
			g.P("                    // Find empty slot in array")
			g.P("                    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("                        if (!data[", enumFieldName, "][i]) {")
			g.P("                            data[", enumFieldName, "][i] = bool:(DecodeVarint(buffer, offset, bufferSize) != 0);")
			g.P("                            break;")
			g.P("                        }")
			g.P("                    }")
			g.P("                }")
		case protoreflect.EnumKind:
			g.P("                if (wireType == WIRE_TYPE_VARINT) {")
			g.P("                    // Find empty slot in array")
			g.P("                    for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("                        if (data[", enumFieldName, "][i] == 0) {")
			g.P("                            data[", enumFieldName, "][i] = ", field.Enum.GoIdent.GoName, ":DecodeVarint(buffer, offset, bufferSize);")
			g.P("                            break;")
			g.P("                        }")
			g.P("                    }")
			g.P("                }")
		case protoreflect.MessageKind:
			nestedMessageName := field.Message.GoIdent.GoName
			g.P("                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {")
			g.P("                    // Unpack repeated nested message ", nestedMessageName)
			g.P("                    new len = DecodeVarint(buffer, offset, bufferSize);")
			g.P("                    new startOffset = offset;")
			g.P("                    new temp", nestedMessageName, "[e", nestedMessageName, "];")
			// Generate proper call with all dependencies
			callArgs := fmt.Sprintf("buffer[startOffset], len, temp%s", nestedMessageName)
			// Add dependencies for nested message
			nestedDeps, nestedHasStrings := collectMessageDependencies(field.Message, nil)
			if nestedHasStrings {
				callArgs += ", repeatedStrings, maxRepeatedStrings"
			}
			for _, dep := range nestedDeps {
				callArgs += fmt.Sprintf(", %ss, max%ss", strings.ToLower(dep), dep)
			}
			g.P("                    Unpack", nestedMessageName, "(", callArgs, ");")
			g.P("                    // Add to global ", strings.ToLower(nestedMessageName), "s array")
			g.P("                    if (max", nestedMessageName, "s < sizeof(", strings.ToLower(nestedMessageName), "s)) {")
			g.P("                        temp", nestedMessageName, "[e", getMessagePrefix(nestedMessageName), "_Id] = max", nestedMessageName, "s + 1;")
			g.P("                        for (new j = 0; j < e", nestedMessageName, "; j++) {")
			g.P("                            ", strings.ToLower(nestedMessageName), "s[max", nestedMessageName, "s][j] = temp", nestedMessageName, "[j];")
			g.P("                        }")
			g.P("                        // Find empty slot in ID array")
			g.P("                        for (new i = 0; i < sizeof(data[", enumFieldName, "]); i++) {")
			g.P("                            if (data[", enumFieldName, "][i] == 0) {")
			g.P("                                data[", enumFieldName, "][i] = temp", nestedMessageName, "[e", getMessagePrefix(nestedMessageName), "_Id];")
			g.P("                                break;")
			g.P("                            }")
			g.P("                        }")
			g.P("                        max", nestedMessageName, "s++;")
			g.P("                    }")
			g.P("                    offset += len;")
			g.P("                }")
		}
	} else {
		// Single fields unpacking
		switch field.Desc.Kind() {
		case protoreflect.BoolKind:
			g.P("                if (wireType == WIRE_TYPE_VARINT) {")
			g.P("                    data[", enumFieldName, "] = bool:(DecodeVarint(buffer, offset, bufferSize) != 0);")
			g.P("                }")
		case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Uint32Kind,
			protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Uint64Kind,
			protoreflect.Sfixed32Kind, protoreflect.Fixed32Kind,
			protoreflect.Sfixed64Kind, protoreflect.Fixed64Kind:
			g.P("                if (wireType == WIRE_TYPE_VARINT) {")
			g.P("                    data[", enumFieldName, "] = DecodeVarint(buffer, offset, bufferSize);")
			g.P("                }")
		case protoreflect.FloatKind:
			g.P("                if (wireType == WIRE_TYPE_FIXED32) {")
			g.P("                    data[", enumFieldName, "] = Float:DecodeFixed32(buffer, offset);")
			g.P("                }")
		case protoreflect.DoubleKind:
			g.P("                if (wireType == WIRE_TYPE_FIXED64) {")
			g.P("                    data[", enumFieldName, "] = Float:DecodeFixed64(buffer, offset);")
			g.P("                }")
		case protoreflect.StringKind, protoreflect.BytesKind:
			g.P("                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {")
			g.P("                    DecodeString(buffer, offset, bufferSize, data[", enumFieldName, "], 256);")
			g.P("                }")
		case protoreflect.EnumKind:
			g.P("                if (wireType == WIRE_TYPE_VARINT) {")
			g.P("                    data[", enumFieldName, "] = ", field.Enum.GoIdent.GoName, ":DecodeVarint(buffer, offset, bufferSize);")
			g.P("                }")
		case protoreflect.MessageKind:
			nestedMessageName := field.Message.GoIdent.GoName
			g.P("                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {")
			g.P("                    // Unpack nested message ", nestedMessageName)
			g.P("                    new len = DecodeVarint(buffer, offset, bufferSize);")
			g.P("                    new startOffset = offset;")
			g.P("                    new temp", nestedMessageName, "[e", nestedMessageName, "];")
			// Generate proper call with all dependencies
			callArgs := fmt.Sprintf("buffer[startOffset], len, temp%s", nestedMessageName)
			// Add dependencies for nested message
			nestedDeps, nestedHasStrings := collectMessageDependencies(field.Message, nil)
			if nestedHasStrings {
				callArgs += ", repeatedStrings, maxRepeatedStrings"
			}
			for _, dep := range nestedDeps {
				callArgs += fmt.Sprintf(", %ss, max%ss", strings.ToLower(dep), dep)
			}
			g.P("                    Unpack", nestedMessageName, "(", callArgs, ");")
			g.P("                    // Add to global ", strings.ToLower(nestedMessageName), "s array")
			g.P("                    if (max", nestedMessageName, "s < sizeof(", strings.ToLower(nestedMessageName), "s)) {")
			g.P("                        temp", nestedMessageName, "[e", getMessagePrefix(nestedMessageName), "_Id] = max", nestedMessageName, "s + 1;")
			g.P("                        for (new j = 0; j < e", nestedMessageName, "; j++) {")
			g.P("                            ", strings.ToLower(nestedMessageName), "s[max", nestedMessageName, "s][j] = temp", nestedMessageName, "[j];")
			g.P("                        }")
			g.P("                        data[", enumFieldName, "] = temp", nestedMessageName, "[e", getMessagePrefix(nestedMessageName), "_Id];")
			g.P("                        max", nestedMessageName, "s++;")
			g.P("                    }")
			g.P("                    offset += len;")
			g.P("                }")
		}
	}

	g.P("            }")
}

// generateCorrectEnumFieldName generates the correct enum field name matching the include generator
func generateCorrectEnumFieldName(field *protogen.Field, messageName string) string {
	var builder strings.Builder

	// Field name with proper capitalization and underscores converted
	fieldName := convertFieldName(string(field.Desc.Name()))

	if field.Desc.IsList() {
		// Repeated fields
		switch field.Desc.Kind() {
		case protoreflect.StringKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("RSId") // Array of repeated string IDs
		case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Uint32Kind,
			protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Uint64Kind,
			protoreflect.Sfixed32Kind, protoreflect.Fixed32Kind,
			protoreflect.Sfixed64Kind, protoreflect.Fixed64Kind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName) // Array of integers directly
		case protoreflect.FloatKind, protoreflect.DoubleKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName) // Array of floats directly
		case protoreflect.BoolKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName) // Array of bools directly
		case protoreflect.MessageKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("Id") // Array of message IDs
		case protoreflect.EnumKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName) // Array of enums directly
		default:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("Id")
		}
	} else {
		// Single fields
		switch field.Desc.Kind() {
		case protoreflect.BoolKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
		case protoreflect.StringKind, protoreflect.BytesKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
		case protoreflect.FloatKind, protoreflect.DoubleKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
		case protoreflect.MessageKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("Id") // Single message ID
		case protoreflect.EnumKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
		default:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
		}
	}

	return builder.String()
}

// generateHelperFunctions generates helper functions for finding structures by ID
func generateHelperFunctions(g *protogen.GeneratedFile, file *protogen.File) {
	g.P("// ========== Helper Functions for ID Lookup ==========")
	g.P()

	// Generate helper function for repeated strings
	g.P("// Get repeated string by ID")
	g.P("stock GetRepeatedStringById(id, const data[][E_REPEATED_STRING], maxItems, result[256]) {")
	g.P("    for (new i = 0; i < maxItems; i++) {")
	g.P("        if (data[i][E_REPEATED_STRING_Id] == id) {")
	g.P("            strcopy(result, data[i][E_REPEATED_STRING_Text], 256);")
	g.P("            return 1;")
	g.P("        }")
	g.P("    }")
	g.P("    result[0] = 0; // Not found")
	g.P("    return 0;")
	g.P("}")
	g.P()

	// Generate helper functions for each message (including nested)
	for _, message := range file.Messages {
		generateMessageHelperFunctions(g, message)
	}
}

// generateMessageHelperFunctions generates helper functions for message and its nested messages
func generateMessageHelperFunctions(g *protogen.GeneratedFile, message *protogen.Message) {
	// First generate helper functions for nested messages
	for _, nested := range message.Messages {
		generateMessageHelperFunctions(g, nested)
	}

	// Then generate helper function for this message
	messageName := message.GoIdent.GoName
	enumName := "e" + messageName
	messagePrefix := getMessagePrefix(messageName)

	g.P("// Get ", messageName, " by ID from array")
	g.P("stock Get", messageName, "ById(id, const data[][", enumName, "], maxItems, result[", enumName, "]) {")
	g.P("    for (new i = 0; i < maxItems; i++) {")
	g.P("        if (data[i][e", messagePrefix, "_Id] == id) {")
	g.P("            for (new j = 0; j < ", enumName, "; j++) {")
	g.P("                result[j] = data[i][j];")
	g.P("            }")
	g.P("            return 1; // Found")
	g.P("        }")
	g.P("    }")
	g.P("    return 0; // Not found")
	g.P("}")
	g.P()
}
