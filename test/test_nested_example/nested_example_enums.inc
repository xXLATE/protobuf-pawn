// ---------- Messages ----------
// Helper enum for repeated strings
enum E_REPEATED_STRING
{
	E_REPEATED_STRING_Id,
	E_REPEATED_STRING_Text[256]
};

enum eAddress
{
	eA_Id,
	eA_Street[256],
	eA_City[256],
	eA_ZipCode
};

enum ePerson_PhoneNumber
{
	ePP_Id,
	ePP_Number[256],
	ePP_Type[256]
};

enum ePerson
{
	eP_Id,
	eP_Name[256],
	eP_Age,
	eP_AddressId,
	eP_EmailsRSId[64],
	eP_LuckyNumbers[64],
	eP_OldAddressesId[64],
	eP_PhoneNumbersId[64]
};

enum eCompany
{
	eC_Id,
	eC_Name[256],
	eC_EmployeesId[64],
	eC_HeadquartersId
};

