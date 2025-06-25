//
//  Generated code. Do not modify.
//  source: test/example.proto
//
// @dart = 3.3

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names
// ignore_for_file: curly_braces_in_flow_control_structures
// ignore_for_file: deprecated_member_use_from_same_package, library_prefixes
// ignore_for_file: non_constant_identifier_names

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

/// enum Leading comment
class Corpus extends $pb.ProtobufEnum {
  static const Corpus UNIVERSAL = Corpus._(0, _omitEnumNames ? '' : 'UNIVERSAL');
  static const Corpus WEB = Corpus._(1, _omitEnumNames ? '' : 'WEB');
  static const Corpus IMAGES = Corpus._(2, _omitEnumNames ? '' : 'IMAGES');
  static const Corpus LOCAL = Corpus._(3, _omitEnumNames ? '' : 'LOCAL');
  static const Corpus NEWS = Corpus._(4, _omitEnumNames ? '' : 'NEWS');
  static const Corpus PRODUCTS = Corpus._(5, _omitEnumNames ? '' : 'PRODUCTS');
  static const Corpus VIDEO = Corpus._(6, _omitEnumNames ? '' : 'VIDEO');

  static const $core.List<Corpus> values = <Corpus> [
    UNIVERSAL,
    WEB,
    IMAGES,
    LOCAL,
    NEWS,
    PRODUCTS,
    VIDEO,
  ];

  static final $core.List<Corpus?> _byValue = $pb.ProtobufEnum.$_initByValueList(values, 6);
  static Corpus? valueOf($core.int value) =>  value < 0 || value >= _byValue.length ? null : _byValue[value];

  const Corpus._(super.value, super.name);
}


const $core.bool _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
