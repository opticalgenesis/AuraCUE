#include "stdafx.h"

// TODO -- 25/06/2017 -- implement proper exception classes in place of std::cerr
// TODO -- 28/06/2017 -- perform SDK init checks in all methods

// Used to determine whether or not the Intitialize method has been run
// This MUST be true for SDK operations to proceed
bool bIsSdkInitialized = false;

// Specifically for AURA
bool bIsAuraInitialized = false;

// Specifically for CUE
bool bIsCueInitialized = false;


// First bool is CUE, second is AURA
AURACUE_API void AuraCUE::Functions::Initialize(bool bShouldUseCorsair, bool bShouldUseAura)
{
	if (bShouldUseCorsair)
	{
		CorsairPerformProtocolHandshake();
		// Throws CorsairHandshakeException if GetLastError isn't successful
		if (CorsairGetLastError() != CE_Success)
		{
			// Error thrown and bool remains/reset to false
			bIsCueInitialized = false;
			std::cout << GetCueError(CorsairGetLastError()) << std::endl;
		}
		else
		{
			// Handshake was a success, CUE is initialised
			bIsCueInitialized = true;
		}
	}

	if (bShouldUseAura)
	{
		// Gets wstring of all Aura model names
		std::vector<std::wstring> auraDevices;
		RogAuraService::Instance()->QueryDeviceNames(auraDevices);
		int numberOfAuraDevices = auraDevices.size();

		// No Aura devices found, skips init
		if (numberOfAuraDevices < 1)
		{
			std::cout << "No Aura devices detected, not initialising.\n";
		}
		// Aura devices found, initialising
		else
		{
			// Determines whether or not LEDs are on
			bool bAuraLedsAreOn = RogAuraService::Instance()->QuerySwitchState();
			if (bAuraLedsAreOn)
			{
				bIsAuraInitialized = true;
			}
			else
			{
				// LEDs must be on
				RogAuraService::Instance()->SetSwitchState(true);
				do
				{
					std::cout << "AURA LEDs are off, switching on...\n";
				}
				while (!bAuraLedsAreOn);
				bIsAuraInitialized = true;
			}
		}
	}

	// Checks if the specified SDKs were initialised
	if (bIsCueInitialized == bShouldUseCorsair && bIsAuraInitialized == bShouldUseAura)
	{
		bIsSdkInitialized = true;
	}
}

// TODO -- 28/06/2017 -- This doesn't seem to work
AURACUE_API void AuraCUE::Functions::CorsairShouldUseExclusiveAccess(bool bIsExclusive)
{
	if (bIsSdkInitialized)
	{
		if (bIsExclusive)
		{
			CorsairRequestControl(CAM_ExclusiveLightingControl);
		}
		else
		{
			CorsairReleaseControl(CAM_ExclusiveLightingControl);
		}
	}
	else
	{
		std::cerr << "Initialize() MUST be run for SDK operations to be used";
	}

	std::cout << GetCueError(CorsairGetLastError()) << std::endl;
}

AURACUE_API bool AuraCUE::Functions::IsSdkInitialized(bool bShouldPrintToConsole)
{
	if (bIsSdkInitialized)
	{
		std::cout << "Specified SDKs were successfully initialised.\n" << bIsSdkInitialized << std::endl;
		return bIsSdkInitialized;
	}
	else
	{
		if (bShouldPrintToConsole)
		{
			std::cout << "AURA: " << bIsAuraInitialized << std::endl << "CUE: " << bIsCueInitialized << std::endl;
		}
	}

	if (bIsCueInitialized || bIsAuraInitialized)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// TODO -- 28/06/2017 -- sort this out
AURACUE_API int AuraCUE::Functions::NumberOfRgbDevices(bool bShouldPrintToConsole)
{
	if (bIsSdkInitialized)
	{
		return GetNormalizedDevices().size();
	}
	return 0;
}

// DEPRECATED
// Returns a std::vector of connected CUE device models
AURACUE_API std::vector<const char*> AuraCUE::Functions::GetCueDeviceModels(bool bShouldPrintToConsole)
{
	std::vector<const char*> cueDevices;
	int numberOfCueDevices = CorsairGetDeviceCount();
	for (int i = 0; i < numberOfCueDevices; i++)
	{
		const char* deviceModel = CorsairGetDeviceInfo(i)->model;
		cueDevices.push_back(deviceModel);
	}

	return cueDevices;
}


// TODO -- 28/06/2017 -- Move to private function
AURACUE_API AuraCUE::CueDevice AuraCUE::Functions::GetCueDevice(int deviceIndex)
{
	CueDevice device;
	device.deviceModel = GetCueDeviceModel(deviceIndex);
	device.deviceType = GetCueDeviceType(deviceIndex);
	device.physicalLayout = GetCueDevicePhysicalLayout(deviceIndex);
	device.logicalLayout = GetCueDeviceLogicalLayout(deviceIndex);
	device.capsMask = GetCueDeviceCapsMask(deviceIndex);
	return device;
}

AURACUE_API std::vector<AuraCUE::CueDevice> AuraCUE::Functions::GetCueDevices()
{
	std::vector<AuraCUE::CueDevice> devices;
	int numberOfCueDevices = GetNumberOfCueDevices();
	for (int i = 0; i < numberOfCueDevices; i++)
	{
		CueDevice device = GetCueDevice(i);
		devices.push_back(device);
	}
	return devices;
}

// DEPRECATED
// Returns number of CUE enabled devices
// Replace by GetCueDevices$size
AURACUE_API int AuraCUE::Functions::GetNumberOfCueDevices()
{
	if (bIsCueInitialized)
	{
		return CorsairGetDeviceCount();
	}
	return 0;
}

// TODO -- 28/06/2017 -- Move to private function
AURACUE_API AuraCUE::AuraDevice AuraCUE::Functions::GetAuraDevice(int deviceIndex)
{
	AuraCUE::AuraDevice device;
	// Checks if dev has run Initialize()
	if (bIsAuraInitialized)
	{
		std::vector<std::wstring> auraDevices;
		RogAuraService::Instance()->QueryDeviceNames(auraDevices);
		device.modelName = auraDevices[deviceIndex];
		device.deviceType = GetAuraDeviceType(device.modelName);
		return device;
	}
	else
	{
		std::cerr << "Aura SDK not initialised.\n";
	}
	return device;
}

AURACUE_API std::vector<AuraCUE::AuraDevice> AuraCUE::Functions::GetAuraDevices()
{
	std::vector<AuraCUE::AuraDevice> devicestoReturn;
	std::vector<std::wstring> devices;
	RogAuraService::Instance()->QueryDeviceNames(devices);
	int numberOfAuraDevices = devices.size();
	for (int i = 0; i < numberOfAuraDevices; i++)
	{
		AuraCUE::AuraDevice device = GetAuraDevice(i);
		devicestoReturn.push_back(device);
	}
	return devicestoReturn;
}

AURACUE_API std::vector<AuraCUE::RgbDevice> AuraCUE::Functions::GetNormalizedDevices()
{
	std::vector<AuraCUE::RgbDevice> rgbDevices;
	std::vector<AuraCUE::AuraDevice> auraDevices = GetAuraDevices();
	std::vector<AuraCUE::CueDevice> cueDevices = GetCueDevices();

	for (size_t i = 0; i < auraDevices.size(); i++)
	{
		RgbDevice device;
		std::wstring modelName = auraDevices[i].modelName;
		RgbDeviceType deviceType = GetAuraRgbDeviceType(auraDevices[i].deviceType);
		RgbDeviceBrand deviceBrand = AuraCUE::RDB_ASUS;
		device.deviceName = modelName;
		device.deviceType = deviceType;
		device.deviceBrand = deviceBrand;
		rgbDevices.push_back(device);
	}

	for (size_t j = 0; j < cueDevices.size(); j++)
	{
		RgbDevice device;
		std::wstring modelName = StringToWstring(cueDevices[j].deviceModel);
		RgbDeviceType deviceType = GetCueDeviceRgbType(cueDevices[j].deviceType);
		RgbDeviceBrand deviceBrand = AuraCUE::RDB_CORSAIR;
		device.deviceName = modelName;
		device.deviceType = deviceType;
		device.deviceBrand = deviceBrand;
		rgbDevices.push_back(device);
	}

	return rgbDevices;
}

// Non exposed method
// Returns RDT for ADT
AuraCUE::RgbDeviceType GetAuraRgbDeviceType(AuraCUE::AuraDeviceType input)
{
	switch (input)
	{
	case AuraCUE::ADT_INVALID:
		return AuraCUE::RDT_INVALID;
		break;
	case AuraCUE::ADT_KEYBOARD:
		return AuraCUE::RDT_KEYBOARD;
		break;
	case AuraCUE::ADT_MOUSE:
		return AuraCUE::RDT_MOUSE;
		break;
	case AuraCUE::ADT_MOBO:
		return AuraCUE::RDT_MOBO;
		break;
	case AuraCUE::ADT_GPU:
		return AuraCUE::RDT_GPU;
		break;
	case AuraCUE::ADT_SLI:
		return AuraCUE::RDT_SLI;
		break;
	case AuraCUE::ADT_DESKTOP:
		return AuraCUE::RDT_DESKTOP;
		break;
	case AuraCUE::ADT_MIC:
		return AuraCUE::RDT_MIC;
		break;
	default:
		return AuraCUE::RDT_INVALID;
		break;
	}
}

// Non exposed method
// Returns RDT for CDT
AuraCUE::RgbDeviceType GetCueDeviceRgbType(std::string input)
{
	if (input == "Keyboard")
	{
		return AuraCUE::RDT_KEYBOARD;
	}
	else if (input == "Mouse")
	{
		return AuraCUE::RDT_MOUSE;
	}
	else if (input == "Headset")
	{
		return AuraCUE::RDT_HEADSET;
	}
	else
	{
		return AuraCUE::RDT_INVALID;
	}
}

// Fuck this method
AuraCUE::AuraDeviceType GetAuraDeviceType(std::wstring modelName)
{
	std::transform(modelName.begin(), modelName.end(), modelName.begin(), ::tolower);
	if (modelName.find(L"maximus")) 
	{
		return AuraCUE::ADT_MOBO;
	}
	else if (modelName.find(L"crosshair"))
	{
		return AuraCUE::ADT_MOBO;
	}
	else if (modelName.find(L"z270"))
	{
		return AuraCUE::ADT_MOBO;
	}
	else if (modelName.find(L"h270"))
	{
		return AuraCUE::ADT_MOBO;
	}
	else if (modelName.find(L"b250"))
	{
		return AuraCUE::ADT_MOBO;
	}
	else if (modelName.find(L"x99"))
	{
		return AuraCUE::ADT_MOBO;
	}
	else if (modelName.find(L"z170"))
	{
		return AuraCUE::ADT_MOBO;
	}
	else if (modelName.find(L"x370"))
	{
		return AuraCUE::ADT_MOBO;
	}
	else if (modelName.find(L"gtx"))
	{
		return AuraCUE::ADT_GPU;
	}
	else if (modelName.find(L"rx"))
	{
		return AuraCUE::ADT_GPU;
	}
	else if (modelName.find(L"sli"))
	{
		return AuraCUE::ADT_SLI;
	}
	else if (modelName.find(L"gr8"))
	{
		return AuraCUE::ADT_DESKTOP;
	}
	else if (modelName.find(L"claymore"))
	{
		return AuraCUE::ADT_KEYBOARD;
	}
	else if (modelName.find(L"spatha"))
	{
		return AuraCUE::ADT_MOUSE;
	}
	else if (modelName.find(L"gladius"))
	{
		return AuraCUE::ADT_MOUSE;
	}
	else if (modelName.find(L"evolve"))
	{
		return AuraCUE::ADT_MOUSE;
	}
	else if (modelName.find(L"impact"))
	{
		return AuraCUE::ADT_MOUSE;
	}
	else if (modelName.find(L"magnus"))
	{
		return AuraCUE::ADT_MIC;
	}
	else
	{
		return AuraCUE::ADT_INVALID;
	}
}

// Non exposed function
// Returns CUE Device model
std::string GetCueDeviceModel(int deviceIndex)
{
	const char* model = CorsairGetDeviceInfo(deviceIndex)->model;
	return std::string(model);
}

// Non exposed function
// Returns CUE Device type
std::string GetCueDeviceType(int deviceIndex)
{
	CorsairDeviceType deviceType = CorsairGetDeviceInfo(deviceIndex)->type;
	switch (deviceType)
	{
	case CDT_Unknown:
		return "Unknown";
	case CDT_Mouse:
		return "Mouse";
		break;
	case CDT_Keyboard:
		return "Keyboard";
		break;
	case CDT_Headset:
		return "Headset";
		break;
	default:
		return "Unknown Error";
		break;
	}
}

std::string GetCueDevicePhysicalLayout(int deviceIndex)
{
	CorsairPhysicalLayout layout = CorsairGetDeviceInfo(deviceIndex)->physicalLayout;
	switch (layout)
	{
	case CPL_Invalid:
		return "Invalid";
		break;
	case CPL_US:
		return "English (US)";
		break;
	case CPL_UK:
		return "English (UK)";
		break;
	case CPL_BR:
		return "Brazillian";
		break;
	case CPL_JP:
		return "Japanese";
		break;
	case CPL_KR:
		return "Korean";
		break;
	case CPL_Zones1:
		return "Mouse Zone 1";
		break;
	case CPL_Zones2:
		return "Mouse Zone 2";
		break;
	case CPL_Zones3:
		return "Mouse Zone 3";
		break;
	case CPL_Zones4:
		return "Mouse Zone 4";
		break;
	default:
		break;
	}
	return "";
}

// Non exposed function
// Returns CUE Device logical layout
std::string GetCueDeviceLogicalLayout(int deviceIndex)
{
	CorsairLogicalLayout layout = CorsairGetDeviceInfo(deviceIndex)->logicalLayout;
	switch (layout)
	{
	case CLL_Invalid:
		return "Invalid";
		break;
	case CLL_US_Int:
		return "US International";
		break;
	case CLL_NA:
		return "North America";
		break;
	case CLL_EU:
		return "Europe";
		break;
	case CLL_UK:
		return "United Kingdom";
		break;
	case CLL_BE:
		return "Belgium";
		break;
	case CLL_BR:
		return "Brazil";
		break;
	case CLL_CH:
		return "Switzerland";
		break;
	case CLL_CN:
		return "China";
		break;
	case CLL_DE:
		return "Germany";
		break;
	case CLL_ES:
		return "Spain";
		break;
	case CLL_FR:
		return "French";
		break;
	case CLL_IT:
		return "Italy";
		break;
	case CLL_ND:
		return "Netherlands";
		break;
	case CLL_RU:
		return "Russia";
		break;
	case CLL_JP:
		return "Japan";
		break;
	case CLL_KR:
		return "Korea";
		break;
	case CLL_TW:
		return "Taiwan";
		break;
	case CLL_MEX:
		return "Mexico";
		break;
	default:
		break;
	}
	return "";
}

// Non exposed function
// Returns CUE Device caps mask 
int GetCueDeviceCapsMask(int deviceIndex)
{
	return CorsairGetDeviceInfo(deviceIndex)->capsMask;
}

// Non exposed function
// Makes CUE errors human readable
std::string GetCueError(CorsairError err)
{
	switch (err)
	{
	case CE_Success:
		return "Success";
		break;
	case CE_ServerNotFound:
		return "CUE server not found";
		break;
	case CE_NoControl:
		return "No control";
		break;
	case CE_ProtocolHandshakeMissing:
		return "Initialize() not run";
		break;
	case CE_IncompatibleProtocol:
		return "Incompatible protocol";
		break;
	case CE_InvalidArguments:
		return "Invalid arguments";
		break;
	default:
		return "Unknown error";
		break;
	}
}

// Non exposed function
// Converts std::string to std::wstring for normalising of RGB enabled devices
std::wstring StringToWstring(std::string input)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(input);
}