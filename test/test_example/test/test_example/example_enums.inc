// ---------- Enums ----------
// enum Leading comment
enum Corpus
{
	UNIVERSAL = 0, 
	WEB = 1, 
	IMAGES = 2, 
	LOCAL = 3, 
	NEWS = 4, 
	PRODUCTS = 5, 
	VIDEO = 6
};
// ---------- Messages ----------
// Helper enum for repeated strings
enum E_REPEATED_STRING
{
	eRS_Id,
	eRS_Str[256]
};

// Some message
enum eDonkey
{
	eD_Id,
	// Some filed docs

	eD_Hi[256],
	bool:eD_IsCool,
	Float:eD_CoolFactor
};

enum eFunky_Monkey
{
	eFM_Id,
	eFM_Hi[256]
};

enum eFunky
{
	eF_Id,
	eF_MonkeyId,
	eF_DokeyId
};

enum eSomeRPCRequest
{
	eS_Id,
	eS_Id,
	eS_Name[256]
};

enum eSomeRPCResponse
{
	eS_Id,
	eS_Result[256]
};

