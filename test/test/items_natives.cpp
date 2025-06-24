
// native mvis_CreateItem(const i_Id, const i_ItemTypeId, const Float:i_Weight, const Float:i_Volume, &o_Id);
cell Natives::mvis_CreateItem(AMX *amx, cell *params) {
    Item request;
    ItemID response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);
	request.set_item_type_id(params[2]);
	request.set_weight(amx_ctof(params[3]));
	request.set_volume(amx_ctof(params[4]));

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->CreateItem(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[5], &addr);
		*addr = response.id();

	}
    return status.ok();
}

// native mvis_GetItem(const i_Id, &o_Id, &o_ItemTypeId, &Float:o_Weight, &Float:o_Volume);
cell Natives::mvis_GetItem(AMX *amx, cell *params) {
    ItemID request;
    Item response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetItem(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[2], &addr);
		*addr = response.id();
		amx_GetAddr(amx, params[3], &addr);
		*addr = response.item_type_id();
		amx_GetAddr(amx, params[4], &addr);
		float weight = response.weight();
		*addr = amx_ftoc(weight);
		amx_GetAddr(amx, params[5], &addr);
		float volume = response.volume();
		*addr = amx_ftoc(volume);

	}
    return status.ok();
}

// native mvis_DeleteItem(const i_Id, &o_Id);
cell Natives::mvis_DeleteItem(AMX *amx, cell *params) {
    ItemID request;
    ItemID response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->DeleteItem(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[2], &addr);
		*addr = response.id();

	}
    return status.ok();
}

// native mvis_GetItems(const i_Limit, o_Items[][Item]);
cell Natives::mvis_GetItems(AMX *amx, cell *params) {
    GetItemsRequest request;
    GetItemsResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_limit(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetItems(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		// todo: list

	}
    return status.ok();
}

// native mvis_CreateItemType(const i_Id, const i_Name[], const i_Description[], const Float:i_BaseWeight, const Float:i_BaseVolume, const i_ModelName[], const i_ModelHash, &o_Id);
cell Natives::mvis_CreateItemType(AMX *amx, cell *params) {
    ItemType request;
    ItemTypeID response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);
	request.set_name(amx_GetCppString(amx, params[2]));
	request.set_description(amx_GetCppString(amx, params[3]));
	request.set_base_weight(amx_ctof(params[4]));
	request.set_base_volume(amx_ctof(params[5]));
	request.set_model_name(amx_GetCppString(amx, params[6]));
	request.set_model_hash(params[7]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->CreateItemType(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[8], &addr);
		*addr = response.id();

	}
    return status.ok();
}

// native mvis_GetItemType(const i_Id, &o_Id, o_Name[], o_Description[], &Float:o_BaseWeight, &Float:o_BaseVolume, o_ModelName[], &o_ModelHash);
cell Natives::mvis_GetItemType(AMX *amx, cell *params) {
    ItemTypeID request;
    ItemType response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetItemType(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[2], &addr);
		*addr = response.id();
		amx_SetCppString(amx, params[3], response.name(), 256);
		amx_SetCppString(amx, params[4], response.description(), 256);
		amx_GetAddr(amx, params[5], &addr);
		float base_weight = response.base_weight();
		*addr = amx_ftoc(base_weight);
		amx_GetAddr(amx, params[6], &addr);
		float base_volume = response.base_volume();
		*addr = amx_ftoc(base_volume);
		amx_SetCppString(amx, params[7], response.model_name(), 256);
		amx_GetAddr(amx, params[8], &addr);
		*addr = response.model_hash();

	}
    return status.ok();
}

// native mvis_DeleteItemType(const i_Id, &o_Id);
cell Natives::mvis_DeleteItemType(AMX *amx, cell *params) {
    ItemTypeID request;
    ItemTypeID response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->DeleteItemType(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[2], &addr);
		*addr = response.id();

	}
    return status.ok();
}

// native mvis_GetItemTypes(const i_Limit, o_ItemTypes[][ItemType]);
cell Natives::mvis_GetItemTypes(AMX *amx, cell *params) {
    GetItemTypesRequest request;
    GetItemTypesResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_limit(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetItemTypes(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		// todo: list

	}
    return status.ok();
}

// native mvis_CreateContainer(const i_Id, const i_TypeId, const i_ItemId, const i_ItemsInside, const i_Items[][InsideItem], &o_Id);
cell Natives::mvis_CreateContainer(AMX *amx, cell *params) {
    Container request;
    ContainerID response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);
	request.set_type_id(params[2]);
	request.set_item_id(params[3]);
	request.set_items_inside(params[4]);
		// todo: list

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->CreateContainer(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[6], &addr);
		*addr = response.id();

	}
    return status.ok();
}

// native mvis_GetContainer(const i_Id, &o_Id, &o_TypeId, &o_ItemId, &o_ItemsInside, o_Items[][InsideItem]);
cell Natives::mvis_GetContainer(AMX *amx, cell *params) {
    ContainerID request;
    Container response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetContainer(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[2], &addr);
		*addr = response.id();
		amx_GetAddr(amx, params[3], &addr);
		*addr = response.type_id();
		amx_GetAddr(amx, params[4], &addr);
		*addr = response.item_id();
		amx_GetAddr(amx, params[5], &addr);
		*addr = response.items_inside();
		// todo: list

	}
    return status.ok();
}

// native mvis_DeleteContainer(const i_Id, &o_Id);
cell Natives::mvis_DeleteContainer(AMX *amx, cell *params) {
    ContainerID request;
    ContainerID response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->DeleteContainer(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[2], &addr);
		*addr = response.id();

	}
    return status.ok();
}

// native mvis_GetContainers(const i_Limit, o_Containers[][Container]);
cell Natives::mvis_GetContainers(AMX *amx, cell *params) {
    GetContainersRequest request;
    GetContainersResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_limit(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetContainers(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		// todo: list

	}
    return status.ok();
}

// native mvis_CreateContainerType(const i_Id, const i_ContainerItemTypeId, const i_MaxNumber, const Float:i_MaxVolume, const Float:i_MaxWeight, const i_ValidItemTypes[], &o_Id);
cell Natives::mvis_CreateContainerType(AMX *amx, cell *params) {
    ContainerType request;
    ContainerTypeID response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);
	request.set_container_item_type_id(params[2]);
	request.set_max_number(params[3]);
	request.set_max_volume(amx_ctof(params[4]));
	request.set_max_weight(amx_ctof(params[5]));
		// todo: list

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->CreateContainerType(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[7], &addr);
		*addr = response.id();

	}
    return status.ok();
}

// native mvis_GetContainerType(const i_Id, &o_Id, &o_ContainerItemTypeId, &o_MaxNumber, &Float:o_MaxVolume, &Float:o_MaxWeight, o_ValidItemTypes[]);
cell Natives::mvis_GetContainerType(AMX *amx, cell *params) {
    ContainerTypeID request;
    ContainerType response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetContainerType(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[2], &addr);
		*addr = response.id();
		amx_GetAddr(amx, params[3], &addr);
		*addr = response.container_item_type_id();
		amx_GetAddr(amx, params[4], &addr);
		*addr = response.max_number();
		amx_GetAddr(amx, params[5], &addr);
		float max_volume = response.max_volume();
		*addr = amx_ftoc(max_volume);
		amx_GetAddr(amx, params[6], &addr);
		float max_weight = response.max_weight();
		*addr = amx_ftoc(max_weight);
		// todo: list

	}
    return status.ok();
}

// native mvis_DeleteContainerType(const i_Id, &o_Id);
cell Natives::mvis_DeleteContainerType(AMX *amx, cell *params) {
    ContainerTypeID request;
    ContainerTypeID response;
    ClientContext context;
    
	// construct request from params
	request.set_id(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->DeleteContainerType(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[2], &addr);
		*addr = response.id();

	}
    return status.ok();
}

// native mvis_GetContainerTypes(const i_Limit, o_ContainerTypes[][ContainerType]);
cell Natives::mvis_GetContainerTypes(AMX *amx, cell *params) {
    GetContainerTypesRequest request;
    GetContainerTypesResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_limit(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetContainerTypes(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		// todo: list

	}
    return status.ok();
}

// native mvis_GetContainerItems(const i_ContainerId, const i_Limit, o_Items[][InsideItem]);
cell Natives::mvis_GetContainerItems(AMX *amx, cell *params) {
    GetContainerItemsRequest request;
    GetContainerItemsResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_container_id(params[1]);
	request.set_limit(params[2]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetContainerItems(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		// todo: list

	}
    return status.ok();
}

// native mvis_PullItem(const i_ContainerId, const i_ItemId, &o_Id, &o_ItemTypeId, &Float:o_Weight, &Float:o_Volume);
cell Natives::mvis_PullItem(AMX *amx, cell *params) {
    PullItemRequest request;
    Item response;
    ClientContext context;
    
	// construct request from params
	request.set_container_id(params[1]);
	request.set_item_id(params[2]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->PullItem(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		amx_GetAddr(amx, params[3], &addr);
		*addr = response.id();
		amx_GetAddr(amx, params[4], &addr);
		*addr = response.item_type_id();
		amx_GetAddr(amx, params[5], &addr);
		float weight = response.weight();
		*addr = amx_ftoc(weight);
		amx_GetAddr(amx, params[6], &addr);
		float volume = response.volume();
		*addr = amx_ftoc(volume);

	}
    return status.ok();
}

// native mvis_PutItem(const i_ContainerId, const i_ItemId, const i_Slot, o_InsideItem[InsideItem]);
cell Natives::mvis_PutItem(AMX *amx, cell *params) {
    PutItemRequest request;
    PutItemResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_container_id(params[1]);
	request.set_item_id(params[2]);
	request.set_slot(params[3]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->PutItem(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		// TODO: message

	}
    return status.ok();
}

// native mvis_SortItems(const i_ContainerId, const SortingMode:i_SortBy, o_Container[Container]);
cell Natives::mvis_SortItems(AMX *amx, cell *params) {
    SortItemsRequest request;
    SortItemsResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_container_id(params[1]);
	request.set_sort_by(static_cast<SortingMode>(params[2]));

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->SortItems(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		// TODO: message

	}
    return status.ok();
}

// native mvis_GetNearestItems(const i_ContainerId, const Float:i_DistanceLimit, o_Item[][InsideItem]);
cell Natives::mvis_GetNearestItems(AMX *amx, cell *params) {
    GetNearestItemsRequest request;
    GetNearestItemsResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_container_id(params[1]);
	request.set_distance_limit(amx_ctof(params[2]));

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->GetNearestItems(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;
		// todo: list

	}
    return status.ok();
}

// native mvis_UseItem(const i_ItemId, &o_Success);
cell Natives::mvis_UseItem(AMX *amx, cell *params) {
    UseItemRequest request;
    UseItemResponse response;
    ClientContext context;
    
	// construct request from params
	request.set_item_id(params[1]);

    // RPC call.
    Status status = API::Get()->MruVItemServiceStub()->UseItem(&context, request, &response);
    API::Get()->setLastStatus(status);
    
	// convert response to amx structure
	if(status.ok())
	{
		cell* addr = nullptr;

	}
    return status.ok();
}
