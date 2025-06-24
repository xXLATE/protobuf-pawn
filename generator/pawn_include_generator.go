package generator

import (
	"fmt"
	"google.golang.org/protobuf/compiler/protogen"
	"google.golang.org/protobuf/reflect/protoreflect"
	"strings"
)

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
		for _, message := range file.Messages {
			genMessages(g, message)
		}
	}

	return g
}

func GenerateIncludeNativesFiles(gen *protogen.Plugin, file *protogen.File) *protogen.GeneratedFile {
	g := gen.NewGeneratedFile(file.GeneratedFilenamePrefix+"_natives.inc", file.GoImportPath)

	for _, service := range file.Services {
		genNatives(g, service)
	}

	for _, service := range file.Services {
		genCallbacks(g, service)
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
	if last {
		g.P("\t", value.Comments.Leading,
			"\t", value.Desc.Name(), " = ", value.Desc.Number())
	} else {
		g.P("\t", value.Comments.Leading,
			"\t", value.Desc.Name(), " = ", value.Desc.Number(), ", ")
	}
}

func genMessages(g *protogen.GeneratedFile, message *protogen.Message) {
	g.P(message.Comments.Leading, "enum e", message.GoIdent.GoName)
	g.P("{")
	last := len(message.Fields) - 1
	for idx, field := range message.Fields {
		if idx != last {
			g.P("\t", field.Comments.Leading, "\t", genField(field), ",")
		} else {
			g.P("\t", field.Comments.Leading, "\t", genField(field))
		}
	}
	g.P("};")
	g.P()
}

func genField(field *protogen.Field) string {
	var builder strings.Builder
	prefix, array, message := getFieldInfo(field)
	builder.WriteString(prefix)
	builder.WriteRune('e')
	builder.WriteString(field.GoName)
	if message {
		builder.WriteString("_msg")
	}
	for i := 0; i < array; i++ {
		builder.WriteString("[256]") //TODO: dowolne wielkosci tablic
	}
	if message {
		//TODO: problem z tablicą obiektów
		//builder.WriteRune('[')
		//builder.WriteString(field.Message.GoIdent.GoName)
		//builder.WriteRune(']')
	}
	return builder.String()
}

func genNatives(g *protogen.GeneratedFile, service *protogen.Service) {
	g.P("// ----- ", service.GoName, " Natives -----")
	g.P(service.Comments.Leading)
	for _, method := range service.Methods {
		g.P(method.Comments.Leading,
			"native bool:",
			getNativeName(service, method),
			"(", getNativeParams(method), ");",
			method.Comments.Trailing)
		g.P()
	}
}

func getNativeName(service *protogen.Service, method *protogen.Method) string {
	return fmt.Sprintf("%s_%s", strings.ToLower(extractCapitals(service.GoName)), method.GoName)
}

func getNativeParams(method *protogen.Method) string {
	var strBuilder strings.Builder

	//input params
	for _, param := range method.Input.Fields {
		genParam(&strBuilder, param, true)
		strBuilder.WriteString(", ")
	}

	//strBuilder params
	for _, param := range method.Output.Fields {
		genParam(&strBuilder, param, false)
		strBuilder.WriteString(", ")
	}

	out := strBuilder.String()
	if len(out) > 2 {
		return out[:len(out)-2]
	}
	return out
}

func getFieldInfo(param *protogen.Field) (prefix string, array int, message bool) {
	array = 0
	switch param.Desc.Kind() {
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

	//TODO: map support
	//if param.Desc.IsMap() {
	//
	//}

	return prefix, array, message
}

func genParam(builder *strings.Builder, param *protogen.Field, inputParam bool) {
	prefix, array, message := getFieldInfo(param)
	if array == 0 && !inputParam && !message {
		builder.WriteRune('&')
	}
	if inputParam {
		builder.WriteString(fmt.Sprintf("const %si_%s", prefix, param.GoName))
	} else {
		builder.WriteString(fmt.Sprintf("%so_%s", prefix, param.GoName))
	}
	for i := 0; i < array; i++ {
		builder.WriteString("[]")
	}
	if message {
		builder.WriteRune('[')
		builder.WriteString(param.Message.GoIdent.GoName)
		builder.WriteRune(']')
	}
}

func genCallbacks(g *protogen.GeneratedFile, service *protogen.Service) {

}
