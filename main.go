// protobuf-pawn is a plugin for the Google protocol buffer compiler to generate
// Pawn code. Install it by building this program and making it accessible within
// your PATH with the name:
//	protobuf-pawn
//
// The 'pawn' suffix becomes part of the argument for the protocol compiler,
// such that it can be invoked as:
//	protoc --pawn_out=paths=source_relative:. path/to/file.proto
//
// This generates Pawn bindings for the protocol buffer defined by file.proto.
// With that input, the output will be written to:
//	path/to/file.pb.go
//
// See the README and documentation for protocol buffers to learn more:
//	https://developers.google.com/protocol-buffers/

package main

import (
	"flag"
	"protobuf-pawn/generator"

	"google.golang.org/protobuf/compiler/protogen"
)

func main() {
	var (
		flags        flag.FlagSet
		importPrefix = flags.String("import_prefix", "", "prefix to prepend to import paths")
	)
	importRewriteFunc := func(importPath protogen.GoImportPath) protogen.GoImportPath {
		switch importPath {
		case "context", "fmt", "math":
			return importPath
		}
		if *importPrefix != "" {
			return protogen.GoImportPath(*importPrefix) + importPath
		}
		return importPath
	}
	protogen.Options{
		ParamFunc:         flags.Set,
		ImportRewriteFunc: importRewriteFunc,
	}.Run(func(gen *protogen.Plugin) error {
		for _, f := range gen.Files {
			if !f.Generate {
				continue
			}
			generator.GenerateIncludeEnumFiles(gen, f)
			generator.GeneratePawnSerializationFile(gen, f)
		}
		gen.SupportedFeatures = generator.SupportedFeatures
		return nil
	})
}
