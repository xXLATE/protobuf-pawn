// ---------- Messages ----------
// Request message for `MruVItemsService.GetItems`.
enum eGetItemsRequest
{
	// Limit of the returned items.
	eLimit
};

// Response message for `MruVItemsService.GetItems`.
enum eGetItemsResponse
{
	// List of items.
	eItems_msg[256]
};

// Request message for `MruVItemsService.GetItemTypes`.
enum eGetItemTypesRequest
{
	// Limit of the returned item types.
	eLimit
};

//Response message for `MruVItemsService.GetItemTypes`.
enum eGetItemTypesResponse
{
	//List of item types.
	eItemTypes_msg[256]
};

//Request message for `MruVItemsService.GetContainers`.
enum eGetContainersRequest
{
	//Limit of the returned containers.
	eLimit
};

//Response message for `MruVItemsService.GetContainers`.
enum eGetContainersResponse
{
	//List of containers.
	eContainers_msg[256]
};

//Request message for `MruVItemsService.GetContainerTypes`.
enum eGetContainerTypesRequest
{
	//Limit of the returned container types.
	eLimit
};

//Response message for `MruVItemsService.GetContainers`.
enum eGetContainerTypesResponse
{
	//List of container types.
	eContainerTypes_msg[256]
};

//Request message for `MruVItemsService.PutItem`.
enum ePutItemRequest
{
	//ID of container where to put item.
	eContainerId,
	//ID of item we wan to put in.
	eItemId,
	//Position used for sorting items
	eSlot
};

//Response message for `MruVItemsService.PutItem`
enum ePutItemResponse
{
	//Container with items inside.
	eInsideItem_msg
};

//Request message for `MruVItemsService.GetContainerItems`.
enum eGetContainerItemsRequest
{
	//ID of container with items.
	eContainerId,
	//Limit of the returned items.
	eLimit
};

//Request message for `MruVItemsService.GetContainerItems`.
enum eGetContainerItemsResponse
{
	//List of items inside containers.
	eItems_msg[256]
};

//Request message for `MruVItemsService.PullItem`.
enum ePullItemRequest
{
	//ID of the container from which we pull out a item.
	eContainerId,
	//ID of the item we want to pull out.
//That item must be inside the container.
	eItemId
};

//Request message for `MruVItemsService.SortItems`.
enum eSortItemsRequest
{
	//ID of the container which contain items.
	eContainerId,
	//Sorting mode.
	SortingMode:eSortBy
};

//Response message for `MruVItemsService.SortItems`.
enum eSortItemsResponse
{
	//Container with sorted items inside.
	eContainer_msg
};

//Request message for `MruVItemsService.GetNearestItems`.
enum eGetNearestItemsRequest
{
	//Position from which to calculate the distance
//Position position = 1;
//ID of container that contains items.
	eContainerId,
	//The distance over which the items are ignored.
	Float:eDistanceLimit
};

//Response message for `MruVItemsService.GetNearestItems`.
enum eGetNearestItemsResponse
{
	//List of items sorted from nearest to farthest.
	eItem_msg[256]
};

//Request message for `MruVItemsService.UseItem`.
enum eUseItemRequest
{
		eItemId
};

//Response message for `MruVItemsService.UseItem`.
enum eUseItemResponse
{
	//Is item usage was successful.
	eSuccess
};

