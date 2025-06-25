package generator

import (
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

	// Generate Pack/Unpack functions for each message
	for _, message := range file.Messages {
		generateMessagePackFunction(g, message)
		generateMessageUnpackFunction(g, message)
	}

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

// generateMessagePackFunction generates Pack function for a message
func generateMessagePackFunction(g *protogen.GeneratedFile, message *protogen.Message) {
	messageName := message.GoIdent.GoName
	enumName := "e" + messageName

	g.P("// ========== ", messageName, " Pack Function ==========")
	g.P("// Pack ", messageName, " structure into byte buffer")
	g.P("stock Pack", messageName, "(const data[", enumName, "], buffer[], maxSize) {")
	g.P("    new offset = 0;")
	g.P()

	// Generate packing code for each field
	for _, field := range message.Fields {
		generateFieldPackCode(g, field)
	}

	g.P("    return offset; // Return total bytes written")
	g.P("}")
	g.P()
}

// generateMessageUnpackFunction generates Unpack function for a message
func generateMessageUnpackFunction(g *protogen.GeneratedFile, message *protogen.Message) {
	messageName := message.GoIdent.GoName
	enumName := "e" + messageName

	g.P("// ========== ", messageName, " Unpack Function ==========")
	g.P("// Unpack ", messageName, " structure from byte buffer")
	g.P("stock Unpack", messageName, "(const buffer[], bufferSize, data[", enumName, "]) {")
	g.P("    new offset = 0;")
	g.P("    new fieldNumber, wireType;")
	g.P()

	g.P("    // Clear the data structure")
	for i, field := range message.Fields {
		fieldName := "e" + field.GoName
		if isStringField(field) {
			g.P("    data[", fieldName, "][0] = 0;")
		} else {
			g.P("    data[", fieldName, "] = 0;")
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
		generateFieldUnpackCode(g, field)
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

// generateFieldPackCode generates packing code for a specific field
func generateFieldPackCode(g *protogen.GeneratedFile, field *protogen.Field) {
	fieldNumber := field.Desc.Number()
	fieldName := "e" + field.GoName

	g.P("    // Field ", fieldNumber, ": ", field.Desc.Name())

	switch field.Desc.Kind() {
	case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Uint32Kind,
		protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Uint64Kind:
		g.P("    if (data[", fieldName, "] != 0) {")
		g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_VARINT, buffer, offset);")
		g.P("        EncodeVarint(data[", fieldName, "], buffer, offset);")
		g.P("    }")

	case protoreflect.FloatKind:
		g.P("    if (data[", fieldName, "] != 0.0) {")
		g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_FIXED32, buffer, offset);")
		g.P("        EncodeFixed32(_:data[", fieldName, "], buffer, offset);")
		g.P("    }")
	case protoreflect.DoubleKind:
		g.P("    if (data[", fieldName, "] != 0.0) {")
		g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_FIXED64, buffer, offset);")
		g.P("        EncodeFixed64(_:data[", fieldName, "], buffer, offset);")
		g.P("    }")

	case protoreflect.StringKind, protoreflect.BytesKind:
		g.P("    if (strlen(data[", fieldName, "]) > 0) {")
		g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);")
		g.P("        EncodeString(data[", fieldName, "], buffer, offset);")
		g.P("    }")

	case protoreflect.EnumKind:
		g.P("    if (data[", fieldName, "] != 0) {")
		g.P("        EncodeTag(", fieldNumber, ", WIRE_TYPE_VARINT, buffer, offset);")
		g.P("        EncodeVarint(_:data[", fieldName, "], buffer, offset);")
		g.P("    }")

	case protoreflect.MessageKind:
		g.P("    // TODO: Nested message packing for ", field.Message.GoIdent.GoName)

	default:
		g.P("    // TODO: Unsupported field type")
	}
	g.P()
}

// generateFieldUnpackCode generates unpacking code for a specific field
func generateFieldUnpackCode(g *protogen.GeneratedFile, field *protogen.Field) {
	fieldNumber := field.Desc.Number()
	fieldName := "e" + field.GoName

	g.P("            case ", fieldNumber, ": {")
	g.P("                // ", field.Desc.Name())

	switch field.Desc.Kind() {
	case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Uint32Kind,
		protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Uint64Kind:
		g.P("                if (wireType == WIRE_TYPE_VARINT) {")
		g.P("                    data[", fieldName, "] = DecodeVarint(buffer, offset, bufferSize);")
		g.P("                }")

	case protoreflect.FloatKind:
		g.P("                if (wireType == WIRE_TYPE_FIXED32) {")
		g.P("                    data[", fieldName, "] = Float:DecodeFixed32(buffer, offset);")
		g.P("                }")
	case protoreflect.DoubleKind:
		g.P("                if (wireType == WIRE_TYPE_FIXED64) {")
		g.P("                    data[", fieldName, "] = Float:DecodeFixed64(buffer, offset);")
		g.P("                }")

	case protoreflect.StringKind, protoreflect.BytesKind:
		g.P("                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {")
		g.P("                    DecodeString(buffer, offset, bufferSize, data[", fieldName, "], 256);")
		g.P("                }")

	case protoreflect.EnumKind:
		g.P("                if (wireType == WIRE_TYPE_VARINT) {")
		g.P("                    data[", fieldName, "] = ", field.Enum.GoIdent.GoName, ":DecodeVarint(buffer, offset, bufferSize);")
		g.P("                }")

	case protoreflect.MessageKind:
		g.P("                // TODO: Nested message unpacking for ", field.Message.GoIdent.GoName)

	default:
		g.P("                // TODO: Unsupported field type")
	}

	g.P("            }")
}

// Helper function to check if field is string type
func isStringField(field *protogen.Field) bool {
	return field.Desc.Kind() == protoreflect.StringKind || field.Desc.Kind() == protoreflect.BytesKind
}
