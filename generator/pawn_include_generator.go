package generator

import (
	"fmt"
	"strings"

	"google.golang.org/protobuf/compiler/protogen"
	"google.golang.org/protobuf/reflect/protoreflect"
)

// Constants for array sizes
const MAX_ARRAY_SIZE = 64

func GenerateIncludeEnumFiles(gen *protogen.Plugin, file *protogen.File) *protogen.GeneratedFile {
	g := gen.NewGeneratedFile(file.GeneratedFilenamePrefix+"_enums.inc", file.GoImportPath)

	//genGeneratedHeader(gen, g)

	if len(file.Enums) > 0 {
		g.P("// ---------- Enums ----------")
		for _, enum := range file.Enums {
			genEnum(g, enum)
		}
	}

	if len(file.Messages) > 0 {
		g.P("// ---------- Messages ----------")

		// First generate helper enums for repeated fields
		genRepeatedHelperEnums(g, file)

		// Then generate message enums
		for _, message := range file.Messages {
			genMessages(g, message)
		}
	}

	return g
}

func genEnum(g *protogen.GeneratedFile, enum *protogen.Enum) {
	g.P(enum.Comments.Leading, "enum ", enum.Desc.Name())
	g.P("{")
	for idx, value := range enum.Values {
		genEnumValue(g, value, idx == len(enum.Values)-1)
	}
	g.P("};")
}

func genEnumValue(g *protogen.GeneratedFile, value *protogen.EnumValue, last bool) {
	// Add comment on separate line if it exists
	if value.Comments.Leading != "" {
		g.P("\t", value.Comments.Leading)
	}
	// Add enum value with consistent tabulation
	if last {
		g.P("\t", value.Desc.Name(), " = ", value.Desc.Number())
	} else {
		g.P("\t", value.Desc.Name(), " = ", value.Desc.Number(), ", ")
	}
}

// genRepeatedHelperEnums generates helper enums only for repeated strings
func genRepeatedHelperEnums(g *protogen.GeneratedFile, file *protogen.File) {
	// Only generate E_REPEATED_STRING for repeated string fields
	// Other types (int, float, bool) can use direct arrays
	g.P("// Helper enum for repeated strings")
	g.P("enum E_REPEATED_STRING")
	g.P("{")
	g.P("\tE_REPEATED_STRING_Id,")
	g.P("\tE_REPEATED_STRING_Text[256]")
	g.P("};")
	g.P()
}

func genMessages(g *protogen.GeneratedFile, message *protogen.Message) {
	// Generate nested messages first
	for _, nested := range message.Messages {
		genMessages(g, nested)
	}

	// Generate main message enum
	g.P(message.Comments.Leading, "enum e", message.GoIdent.GoName)
	g.P("{")

	// Add ID field for this message
	messagePrefix := getMessagePrefix(message.GoIdent.GoName)
	g.P("\te", messagePrefix, "_Id,")

	// Generate fields
	fieldIndex := 1
	totalFields := len(message.Fields)

	for _, field := range message.Fields {
		// Add comment on separate line if it exists
		if field.Comments.Leading != "" {
			g.P("\t", field.Comments.Leading)
		}
		// Add field with consistent tabulation
		if fieldIndex < totalFields {
			g.P("\t", genFieldNew(field, message.GoIdent.GoName), ",")
		} else {
			g.P("\t", genFieldNew(field, message.GoIdent.GoName))
		}
		fieldIndex++
	}
	g.P("};")
	g.P()
}

// genFieldNew generates field according to new ID-based architecture
func genFieldNew(field *protogen.Field, messageName string) string {
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
			builder.WriteString("RSId[" + fmt.Sprintf("%d", MAX_ARRAY_SIZE) + "]") // Array of repeated string IDs
		case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Uint32Kind,
			protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Uint64Kind,
			protoreflect.Sfixed32Kind, protoreflect.Fixed32Kind,
			protoreflect.Sfixed64Kind, protoreflect.Fixed64Kind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("[" + fmt.Sprintf("%d", MAX_ARRAY_SIZE) + "]") // Array of integers directly
		case protoreflect.FloatKind, protoreflect.DoubleKind:
			builder.WriteString("Float:")
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("[" + fmt.Sprintf("%d", MAX_ARRAY_SIZE) + "]") // Array of floats directly
		case protoreflect.BoolKind:
			builder.WriteString("bool:")
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("[" + fmt.Sprintf("%d", MAX_ARRAY_SIZE) + "]") // Array of bools directly
		case protoreflect.MessageKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("Id[" + fmt.Sprintf("%d", MAX_ARRAY_SIZE) + "]") // Array of message IDs
		case protoreflect.EnumKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("[" + fmt.Sprintf("%d", MAX_ARRAY_SIZE) + "]") // Array of enums directly
		default:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("Id[" + fmt.Sprintf("%d", MAX_ARRAY_SIZE) + "]")
		}
	} else {
		// Single fields
		switch field.Desc.Kind() {
		case protoreflect.BoolKind:
			builder.WriteString("bool:")
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
		case protoreflect.StringKind, protoreflect.BytesKind:
			// Message prefix
			prefix := "e" + getMessagePrefix(messageName) + "_"
			builder.WriteString(prefix)
			builder.WriteString(fieldName)
			builder.WriteString("[256]")
		case protoreflect.FloatKind, protoreflect.DoubleKind:
			builder.WriteString("Float:")
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

func genField(field *protogen.Field) string {
	var builder strings.Builder

	// Handle repeated fields with separate enum
	if field.Desc.IsList() {
		builder.WriteString(string(field.Desc.Name()))
		builder.WriteString("ArrayID") // ID to repeated array enum
		return builder.String()
	}

	// Handle nested messages with ID reference
	if field.Desc.Kind() == protoreflect.MessageKind {
		builder.WriteString(string(field.Desc.Name()))
		builder.WriteString("ID") // ID to nested message
		return builder.String()
	}

	// Handle regular fields with existing logic
	prefix, array, message := getFieldInfo(field)
	builder.WriteString(prefix)
	builder.WriteRune('e')
	builder.WriteString(field.GoName)
	if message {
		builder.WriteString("_msg")
	}
	for i := 0; i < array; i++ {
		builder.WriteString("[" + fmt.Sprintf("%d", MAX_ARRAY_SIZE) + "]") // Use configurable array size
	}
	return builder.String()
}

func getFieldInfo(param *protogen.Field) (prefix string, array int, message bool) {
	array = 0
	switch param.Desc.Kind() {
	case protoreflect.BoolKind:
		prefix = "bool:"
	case protoreflect.EnumKind:
		prefix = param.Enum.GoIdent.GoName + ":"
	case protoreflect.FloatKind, protoreflect.DoubleKind:
		prefix = "Float:"
	case protoreflect.StringKind, protoreflect.BytesKind:
		array += 1
	case protoreflect.MessageKind:
		message = true
	case protoreflect.GroupKind:
		//deprecated
	case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Uint32Kind,
		protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Uint64Kind,
		protoreflect.Sfixed32Kind, protoreflect.Fixed32Kind,
		protoreflect.Sfixed64Kind, protoreflect.Fixed64Kind:
	}

	if param.Desc.IsList() {
		array += 1
	}

	// Map support can be added in the future if needed
	// Maps are not commonly used in Pawn applications
	return prefix, array, message
}
