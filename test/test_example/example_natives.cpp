
// native es_SomeRPC(const i_Id, const i_Name[], o_Result[]);
cell Natives::es_SomeRPC(AMX *amx, cell *params) {
    SomeRPCRequest request;
    SomeRPCResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);
	request.set_name(amx_GetCppString(amx, params[2]));

    // RPC call.
    Status status = API::Get()->ExampleServiceStub()->SomeRPC(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_SetCppString(amx, params[3], response.result(), 256);

	}
    return status.ok();
}
