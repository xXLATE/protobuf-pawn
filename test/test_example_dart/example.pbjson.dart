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

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use corpusDescriptor instead')
const Corpus$json = {
  '1': 'Corpus',
  '2': [
    {'1': 'UNIVERSAL', '2': 0},
    {'1': 'WEB', '2': 1},
    {'1': 'IMAGES', '2': 2},
    {'1': 'LOCAL', '2': 3},
    {'1': 'NEWS', '2': 4},
    {'1': 'PRODUCTS', '2': 5},
    {'1': 'VIDEO', '2': 6},
  ],
};

/// Descriptor for `Corpus`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List corpusDescriptor = $convert.base64Decode(
    'CgZDb3JwdXMSDQoJVU5JVkVSU0FMEAASBwoDV0VCEAESCgoGSU1BR0VTEAISCQoFTE9DQUwQAx'
    'IICgRORVdTEAQSDAoIUFJPRFVDVFMQBRIJCgVWSURFTxAG');

@$core.Deprecated('Use donkeyDescriptor instead')
const Donkey$json = {
  '1': 'Donkey',
  '2': [
    {'1': 'hi', '3': 1, '4': 1, '5': 9, '10': 'hi'},
  ],
};

/// Descriptor for `Donkey`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List donkeyDescriptor = $convert.base64Decode(
    'CgZEb25rZXkSDgoCaGkYASABKAlSAmhp');

@$core.Deprecated('Use funkyDescriptor instead')
const Funky$json = {
  '1': 'Funky',
  '2': [
    {'1': 'monkey', '3': 1, '4': 1, '5': 11, '6': '.Funky.Monkey', '10': 'monkey'},
    {'1': 'dokey', '3': 2, '4': 1, '5': 11, '6': '.Donkey', '10': 'dokey'},
  ],
  '3': [Funky_Monkey$json],
};

@$core.Deprecated('Use funkyDescriptor instead')
const Funky_Monkey$json = {
  '1': 'Monkey',
  '2': [
    {'1': 'hi', '3': 1, '4': 1, '5': 9, '10': 'hi'},
  ],
};

/// Descriptor for `Funky`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List funkyDescriptor = $convert.base64Decode(
    'CgVGdW5reRIlCgZtb25rZXkYASABKAsyDS5GdW5reS5Nb25rZXlSBm1vbmtleRIdCgVkb2tleR'
    'gCIAEoCzIHLkRvbmtleVIFZG9rZXkaGAoGTW9ua2V5Eg4KAmhpGAEgASgJUgJoaQ==');

@$core.Deprecated('Use someRPCRequestDescriptor instead')
const SomeRPCRequest$json = {
  '1': 'SomeRPCRequest',
  '2': [
    {'1': 'id', '3': 1, '4': 1, '5': 5, '10': 'id'},
    {'1': 'name', '3': 2, '4': 1, '5': 9, '10': 'name'},
  ],
};

/// Descriptor for `SomeRPCRequest`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List someRPCRequestDescriptor = $convert.base64Decode(
    'Cg5Tb21lUlBDUmVxdWVzdBIOCgJpZBgBIAEoBVICaWQSEgoEbmFtZRgCIAEoCVIEbmFtZQ==');

@$core.Deprecated('Use someRPCResponseDescriptor instead')
const SomeRPCResponse$json = {
  '1': 'SomeRPCResponse',
  '2': [
    {'1': 'result', '3': 1, '4': 1, '5': 9, '10': 'result'},
  ],
};

/// Descriptor for `SomeRPCResponse`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List someRPCResponseDescriptor = $convert.base64Decode(
    'Cg9Tb21lUlBDUmVzcG9uc2USFgoGcmVzdWx0GAEgASgJUgZyZXN1bHQ=');

const $core.Map<$core.String, $core.dynamic> ExampleServiceBase$json = {
  '1': 'ExampleService',
  '2': [
    {'1': 'SomeRPC', '2': '.SomeRPCRequest', '3': '.SomeRPCResponse'},
    {'1': 'SomeRPClientStream', '2': '.SomeRPCRequest', '3': '.SomeRPCResponse', '5': true},
    {'1': 'SomeRPCServerStream', '2': '.SomeRPCRequest', '3': '.SomeRPCResponse', '6': true},
    {'1': 'SomeRPCBiDiStream', '2': '.SomeRPCRequest', '3': '.SomeRPCResponse', '5': true, '6': true},
  ],
};

@$core.Deprecated('Use exampleServiceDescriptor instead')
const $core.Map<$core.String, $core.Map<$core.String, $core.dynamic>> ExampleServiceBase$messageJson = {
  '.SomeRPCRequest': SomeRPCRequest$json,
  '.SomeRPCResponse': SomeRPCResponse$json,
};

/// Descriptor for `ExampleService`. Decode as a `google.protobuf.ServiceDescriptorProto`.
final $typed_data.Uint8List exampleServiceDescriptor = $convert.base64Decode(
    'Cg5FeGFtcGxlU2VydmljZRIsCgdTb21lUlBDEg8uU29tZVJQQ1JlcXVlc3QaEC5Tb21lUlBDUm'
    'VzcG9uc2USOQoSU29tZVJQQ2xpZW50U3RyZWFtEg8uU29tZVJQQ1JlcXVlc3QaEC5Tb21lUlBD'
    'UmVzcG9uc2UoARI6ChNTb21lUlBDU2VydmVyU3RyZWFtEg8uU29tZVJQQ1JlcXVlc3QaEC5Tb2'
    '1lUlBDUmVzcG9uc2UwARI6ChFTb21lUlBDQmlEaVN0cmVhbRIPLlNvbWVSUENSZXF1ZXN0GhAu'
    'U29tZVJQQ1Jlc3BvbnNlKAEwAQ==');

