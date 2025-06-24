// ----- MruVItemService Natives -----
// The MruV items service provides procedures for managing items and containers

//Create new item.
native bool:mvis_CreateItem(const i_Id, const i_ItemTypeId, const Float:i_Weight, const Float:i_Volume, &o_Id);

//Get item by id.
native bool:mvis_GetItem(const i_Id, &o_Id, &o_ItemTypeId, &Float:o_Weight, &Float:o_Volume);

//Delete item by id.
native bool:mvis_DeleteItem(const i_Id, &o_Id);

//Gets all items.
native bool:mvis_GetItems(const i_Limit, o_Items[][Item]);

//Create item type.
native bool:mvis_CreateItemType(const i_Id, const i_Name[], const i_Description[], const Float:i_BaseWeight, const Float:i_BaseVolume, const i_ModelName[], const i_ModelHash, &o_Id);

//Get item type by id.
native bool:mvis_GetItemType(const i_Id, &o_Id, o_Name[], o_Description[], &Float:o_BaseWeight, &Float:o_BaseVolume, o_ModelName[], &o_ModelHash);

//Delete item type by id.
native bool:mvis_DeleteItemType(const i_Id, &o_Id);

//Gets all item types.
native bool:mvis_GetItemTypes(const i_Limit, o_ItemTypes[][ItemType]);

//Create container.
native bool:mvis_CreateContainer(const i_Id, const i_TypeId, const i_ItemId, const i_ItemsInside, const i_Items[][InsideItem], &o_Id);

//Get container by id.
native bool:mvis_GetContainer(const i_Id, &o_Id, &o_TypeId, &o_ItemId, &o_ItemsInside, o_Items[][InsideItem]);

//Delete container by id.
native bool:mvis_DeleteContainer(const i_Id, &o_Id);

//Get all containers.
native bool:mvis_GetContainers(const i_Limit, o_Containers[][Container]);

//Create container type.
native bool:mvis_CreateContainerType(const i_Id, const i_ContainerItemTypeId, const i_MaxNumber, const Float:i_MaxVolume, const Float:i_MaxWeight, const i_ValidItemTypes[], &o_Id);

//Get container type by id.
native bool:mvis_GetContainerType(const i_Id, &o_Id, &o_ContainerItemTypeId, &o_MaxNumber, &Float:o_MaxVolume, &Float:o_MaxWeight, o_ValidItemTypes[]);

//Detele container type by id.
native bool:mvis_DeleteContainerType(const i_Id, &o_Id);

//Get all container types.
native bool:mvis_GetContainerTypes(const i_Limit, o_ContainerTypes[][ContainerType]);

//Get items inside a container.
native bool:mvis_GetContainerItems(const i_ContainerId, const i_Limit, o_Items[][InsideItem]);

//Pull item from container.
native bool:mvis_PullItem(const i_ContainerId, const i_ItemId, &o_Id, &o_ItemTypeId, &Float:o_Weight, &Float:o_Volume);

//Put item into container.
native bool:mvis_PutItem(const i_ContainerId, const i_ItemId, const i_Slot, o_InsideItem[InsideItem]);

//Sort items inside container.
//This procedure change order of items inside container.
native bool:mvis_SortItems(const i_ContainerId, const SortingMode:i_SortBy, o_Container[Container]);

//Retrieves from the container the list of items nearest to the given position.
native bool:mvis_GetNearestItems(const i_ContainerId, const Float:i_DistanceLimit, o_Item[][InsideItem]);

//Trigger action associated with the item usage.
native bool:mvis_UseItem(const i_ItemId, &o_Success);

