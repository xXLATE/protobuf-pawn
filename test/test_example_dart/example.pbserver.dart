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

import 'example.pb.dart' as $0;
import 'example.pbjson.dart';

export 'example.pb.dart';

abstract class ExampleServiceBase extends $pb.GeneratedService {
  $async.Future<$0.SomeRPCResponse> someRPC($pb.ServerContext ctx, $0.SomeRPCRequest request);
  $async.Future<$0.SomeRPCResponse> someRPClientStream($pb.ServerContext ctx, $0.SomeRPCRequest request);
  $async.Future<$0.SomeRPCResponse> someRPCServerStream($pb.ServerContext ctx, $0.SomeRPCRequest request);
  $async.Future<$0.SomeRPCResponse> someRPCBiDiStream($pb.ServerContext ctx, $0.SomeRPCRequest request);

  $pb.GeneratedMessage createRequest($core.String methodName) {
    switch (methodName) {
      case 'SomeRPC': return $0.SomeRPCRequest();
      case 'SomeRPClientStream': return $0.SomeRPCRequest();
      case 'SomeRPCServerStream': return $0.SomeRPCRequest();
      case 'SomeRPCBiDiStream': return $0.SomeRPCRequest();
      default: throw $core.ArgumentError('Unknown method: $methodName');
    }
  }

  $async.Future<$pb.GeneratedMessage> handleCall($pb.ServerContext ctx, $core.String methodName, $pb.GeneratedMessage request) {
    switch (methodName) {
      case 'SomeRPC': return someRPC(ctx, request as $0.SomeRPCRequest);
      case 'SomeRPClientStream': return someRPClientStream(ctx, request as $0.SomeRPCRequest);
      case 'SomeRPCServerStream': return someRPCServerStream(ctx, request as $0.SomeRPCRequest);
      case 'SomeRPCBiDiStream': return someRPCBiDiStream(ctx, request as $0.SomeRPCRequest);
      default: throw $core.ArgumentError('Unknown method: $methodName');
    }
  }

  $core.Map<$core.String, $core.dynamic> get $json => ExampleServiceBase$json;
  $core.Map<$core.String, $core.Map<$core.String, $core.dynamic>> get $messageJson => ExampleServiceBase$messageJson;
}

