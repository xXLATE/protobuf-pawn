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

import 'dart:async' as $async;
import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

export 'package:protobuf/protobuf.dart' show GeneratedMessageGenericExtensions;

export 'example.pbenum.dart';

/// Some message
class Donkey extends $pb.GeneratedMessage {
  factory Donkey({
    $core.String? hi,
  }) {
    final result = create();
    if (hi != null) result.hi = hi;
    return result;
  }

  Donkey._();

  factory Donkey.fromBuffer($core.List<$core.int> data, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(data, registry);
  factory Donkey.fromJson($core.String json, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(json, registry);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Donkey', createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'hi')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  Donkey clone() => Donkey()..mergeFromMessage(this);
  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  Donkey copyWith(void Function(Donkey) updates) => super.copyWith((message) => updates(message as Donkey)) as Donkey;

  @$core.override
  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Donkey create() => Donkey._();
  @$core.override
  Donkey createEmptyInstance() => create();
  static $pb.PbList<Donkey> createRepeated() => $pb.PbList<Donkey>();
  @$core.pragma('dart2js:noInline')
  static Donkey getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Donkey>(create);
  static Donkey? _defaultInstance;

  /// Some filed docs
  @$pb.TagNumber(1)
  $core.String get hi => $_getSZ(0);
  @$pb.TagNumber(1)
  set hi($core.String value) => $_setString(0, value);
  @$pb.TagNumber(1)
  $core.bool hasHi() => $_has(0);
  @$pb.TagNumber(1)
  void clearHi() => $_clearField(1);
}

class Funky_Monkey extends $pb.GeneratedMessage {
  factory Funky_Monkey({
    $core.String? hi,
  }) {
    final result = create();
    if (hi != null) result.hi = hi;
    return result;
  }

  Funky_Monkey._();

  factory Funky_Monkey.fromBuffer($core.List<$core.int> data, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(data, registry);
  factory Funky_Monkey.fromJson($core.String json, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(json, registry);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Funky.Monkey', createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'hi')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  Funky_Monkey clone() => Funky_Monkey()..mergeFromMessage(this);
  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  Funky_Monkey copyWith(void Function(Funky_Monkey) updates) => super.copyWith((message) => updates(message as Funky_Monkey)) as Funky_Monkey;

  @$core.override
  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Funky_Monkey create() => Funky_Monkey._();
  @$core.override
  Funky_Monkey createEmptyInstance() => create();
  static $pb.PbList<Funky_Monkey> createRepeated() => $pb.PbList<Funky_Monkey>();
  @$core.pragma('dart2js:noInline')
  static Funky_Monkey getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Funky_Monkey>(create);
  static Funky_Monkey? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get hi => $_getSZ(0);
  @$pb.TagNumber(1)
  set hi($core.String value) => $_setString(0, value);
  @$pb.TagNumber(1)
  $core.bool hasHi() => $_has(0);
  @$pb.TagNumber(1)
  void clearHi() => $_clearField(1);
}

class Funky extends $pb.GeneratedMessage {
  factory Funky({
    Funky_Monkey? monkey,
    Donkey? dokey,
  }) {
    final result = create();
    if (monkey != null) result.monkey = monkey;
    if (dokey != null) result.dokey = dokey;
    return result;
  }

  Funky._();

  factory Funky.fromBuffer($core.List<$core.int> data, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(data, registry);
  factory Funky.fromJson($core.String json, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(json, registry);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Funky', createEmptyInstance: create)
    ..aOM<Funky_Monkey>(1, _omitFieldNames ? '' : 'monkey', subBuilder: Funky_Monkey.create)
    ..aOM<Donkey>(2, _omitFieldNames ? '' : 'dokey', subBuilder: Donkey.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  Funky clone() => Funky()..mergeFromMessage(this);
  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  Funky copyWith(void Function(Funky) updates) => super.copyWith((message) => updates(message as Funky)) as Funky;

  @$core.override
  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Funky create() => Funky._();
  @$core.override
  Funky createEmptyInstance() => create();
  static $pb.PbList<Funky> createRepeated() => $pb.PbList<Funky>();
  @$core.pragma('dart2js:noInline')
  static Funky getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Funky>(create);
  static Funky? _defaultInstance;

  @$pb.TagNumber(1)
  Funky_Monkey get monkey => $_getN(0);
  @$pb.TagNumber(1)
  set monkey(Funky_Monkey value) => $_setField(1, value);
  @$pb.TagNumber(1)
  $core.bool hasMonkey() => $_has(0);
  @$pb.TagNumber(1)
  void clearMonkey() => $_clearField(1);
  @$pb.TagNumber(1)
  Funky_Monkey ensureMonkey() => $_ensure(0);

  @$pb.TagNumber(2)
  Donkey get dokey => $_getN(1);
  @$pb.TagNumber(2)
  set dokey(Donkey value) => $_setField(2, value);
  @$pb.TagNumber(2)
  $core.bool hasDokey() => $_has(1);
  @$pb.TagNumber(2)
  void clearDokey() => $_clearField(2);
  @$pb.TagNumber(2)
  Donkey ensureDokey() => $_ensure(1);
}

class SomeRPCRequest extends $pb.GeneratedMessage {
  factory SomeRPCRequest({
    $core.int? id,
    $core.String? name,
  }) {
    final result = create();
    if (id != null) result.id = id;
    if (name != null) result.name = name;
    return result;
  }

  SomeRPCRequest._();

  factory SomeRPCRequest.fromBuffer($core.List<$core.int> data, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(data, registry);
  factory SomeRPCRequest.fromJson($core.String json, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(json, registry);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SomeRPCRequest', createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'id', $pb.PbFieldType.O3)
    ..aOS(2, _omitFieldNames ? '' : 'name')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  SomeRPCRequest clone() => SomeRPCRequest()..mergeFromMessage(this);
  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  SomeRPCRequest copyWith(void Function(SomeRPCRequest) updates) => super.copyWith((message) => updates(message as SomeRPCRequest)) as SomeRPCRequest;

  @$core.override
  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SomeRPCRequest create() => SomeRPCRequest._();
  @$core.override
  SomeRPCRequest createEmptyInstance() => create();
  static $pb.PbList<SomeRPCRequest> createRepeated() => $pb.PbList<SomeRPCRequest>();
  @$core.pragma('dart2js:noInline')
  static SomeRPCRequest getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SomeRPCRequest>(create);
  static SomeRPCRequest? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get id => $_getIZ(0);
  @$pb.TagNumber(1)
  set id($core.int value) => $_setSignedInt32(0, value);
  @$pb.TagNumber(1)
  $core.bool hasId() => $_has(0);
  @$pb.TagNumber(1)
  void clearId() => $_clearField(1);

  @$pb.TagNumber(2)
  $core.String get name => $_getSZ(1);
  @$pb.TagNumber(2)
  set name($core.String value) => $_setString(1, value);
  @$pb.TagNumber(2)
  $core.bool hasName() => $_has(1);
  @$pb.TagNumber(2)
  void clearName() => $_clearField(2);
}

class SomeRPCResponse extends $pb.GeneratedMessage {
  factory SomeRPCResponse({
    $core.String? result,
  }) {
    final result$ = create();
    if (result != null) result$.result = result;
    return result$;
  }

  SomeRPCResponse._();

  factory SomeRPCResponse.fromBuffer($core.List<$core.int> data, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(data, registry);
  factory SomeRPCResponse.fromJson($core.String json, [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(json, registry);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SomeRPCResponse', createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'result')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  SomeRPCResponse clone() => SomeRPCResponse()..mergeFromMessage(this);
  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  SomeRPCResponse copyWith(void Function(SomeRPCResponse) updates) => super.copyWith((message) => updates(message as SomeRPCResponse)) as SomeRPCResponse;

  @$core.override
  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SomeRPCResponse create() => SomeRPCResponse._();
  @$core.override
  SomeRPCResponse createEmptyInstance() => create();
  static $pb.PbList<SomeRPCResponse> createRepeated() => $pb.PbList<SomeRPCResponse>();
  @$core.pragma('dart2js:noInline')
  static SomeRPCResponse getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SomeRPCResponse>(create);
  static SomeRPCResponse? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get result => $_getSZ(0);
  @$pb.TagNumber(1)
  set result($core.String value) => $_setString(0, value);
  @$pb.TagNumber(1)
  $core.bool hasResult() => $_has(0);
  @$pb.TagNumber(1)
  void clearResult() => $_clearField(1);
}

class ExampleServiceApi {
  final $pb.RpcClient _client;

  ExampleServiceApi(this._client);

  $async.Future<SomeRPCResponse> someRPC($pb.ClientContext? ctx, SomeRPCRequest request) =>
    _client.invoke<SomeRPCResponse>(ctx, 'ExampleService', 'SomeRPC', request, SomeRPCResponse())
  ;
  $async.Future<SomeRPCResponse> someRPClientStream($pb.ClientContext? ctx, SomeRPCRequest request) =>
    _client.invoke<SomeRPCResponse>(ctx, 'ExampleService', 'SomeRPClientStream', request, SomeRPCResponse())
  ;
  $async.Future<SomeRPCResponse> someRPCServerStream($pb.ClientContext? ctx, SomeRPCRequest request) =>
    _client.invoke<SomeRPCResponse>(ctx, 'ExampleService', 'SomeRPCServerStream', request, SomeRPCResponse())
  ;
  $async.Future<SomeRPCResponse> someRPCBiDiStream($pb.ClientContext? ctx, SomeRPCRequest request) =>
    _client.invoke<SomeRPCResponse>(ctx, 'ExampleService', 'SomeRPCBiDiStream', request, SomeRPCResponse())
  ;
}


const $core.bool _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const $core.bool _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
