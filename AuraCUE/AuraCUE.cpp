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

std::string GetCueError(CorsairError err);

std::string GetCueDeviceModel(int deviceIndex);
std::string GetCueDeviceType(int deviceIndex);
std::string GetCueDevicePhysicalLayout(int deviceIndex);
std::string GetCueDeviceLogicalLayout(int deviceIndex);
int GetCueDeviceCapsMask(int deviceIndex);

RogEnums::AuraDeviceType GetAuraDeviceModel(std::wstring modelName);

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
		}
		else
		{
			// Handshake was a success, CUE is initialised
			bIsCueInitialized = true;
		}
	}

	if (bShouldUseAura)
	{
		std::vector<std::wstring> auraDevices;
		int numberOfAuraDevices = sizeof(RogAuraService::Instance()->QueryDevceNames(auraDevices));
		if (numberOfAuraDevices < 2)
		{
			std::cout << "No Aura devices detected, not initialising.\n";
		}
		else
		{
			bool bAuraLedsAreOn = RogAuraService::Instance()->QuerySwitchState();
			if (bAuraLedsAreOn)
			{
				bIsAuraInitialized = true;
			}
			else
			{
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

	if (bIsCueInitialized == bShouldUseCorsair && bIsAuraInitialized == bShouldUseAura)
	{
		bIsSdkInitialized = true;
	}
}

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

AURACUE_API int AuraCUE::Functions::NumberOfRgbDevices(bool bShouldPrintToConsole)
{
	if (bIsSdkInitialized)
	{
		int numberOfDevices = 0;
		int numberOfAuraDevices = 0;
		int numberOfCueDevices = 0;

		if (bShouldPrintToConsole)
		{
			std::cout << "Number of AURA devices: " << numberOfAuraDevices << std::endl
				<< "Number of CUE devices: " << numberOfCueDevices << std::endl;
		}
		return numberOfDevices;
	}
	else
	{
		std::cerr << "Initialize() MUST be run for SDK operations to proceed.\n";
	}
}

AURACUE_API std::vector<const char*> AuraCUE::Functions::GetCueDeviceModels(bool bShouldPrintToConsole)
{
	std::vector<const char*> cueDevices;
	int numberOfCueDevices = CorsairGetDeviceCount();
	for (size_t i = 0; i < numberOfCueDevices; i++)
	{
		const char* deviceModel = CorsairGetDeviceInfo(i)->model;
		cueDevices.push_back(deviceModel);
	}

	return cueDevices;
}

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

AURACUE_API int AuraCUE::Functions::GetNumberOfCueDevices()
{
	if (bIsCueInitialized)
	{
		return CorsairGetDeviceCount();
	}
}

AURACUE_API RogStructs::AuraDevice AuraCUE::Functions::GetAuraDevice(int deviceIndex)
{
	if (bIsAuraInitialized)
	{
		std::vector<std::wstring> devices;
		RogAuraService::Instance()->QueryDevceNames(devices);

		RogStructs::AuraDevice auraDevice;

	}
}

// I fucking hate this method
RogEnums::AuraDeviceType GetAuraDeviceModel(std::wstring modelName)
{

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