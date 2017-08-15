#include "stdafx.h"

bool bIsCueInitialized = false;
bool bIsAuraInitialized = false;
bool bIsDesiredSdkInitialized = false;



AURACUE_API void AuraCUE::Functions::Initialize(bool bShouldUseCorsair, bool bShouldUseExclusiveCueAccess, bool bShouldUseAura)
{
	if (bShouldUseCorsair && !bShouldUseExclusiveCueAccess)
	{
		CorsairPerformProtocolHandshake();
		if (bShouldUseExclusiveCueAccess)
		{
			CorsairRequestControl(CAM_ExclusiveLightingControl);
		}
		bIsCueInitialized = true;
	}

	if (bShouldUseAura)
	{
		std::cout << "AuraSDK currently not supported";
	}

	// This is not currently representative
	bIsDesiredSdkInitialized = true;
}

AURACUE_API void AuraCUE::Functions::CorsairShouldUseExclusiveAccess(bool bIsExclusive)
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

AURACUE_API bool AuraCUE::Functions::IsSdkInitialized(bool bShouldPrintToConsole)
{
	if (bShouldPrintToConsole)
	{
		std::cout << "Aura SDK: " << bIsAuraInitialized << std::endl
			<< "CUE SDK: " << bIsCueInitialized << std::endl << std::endl;
		return bIsDesiredSdkInitialized;
	}
	else
	{
		return bIsDesiredSdkInitialized;
	}
}

AURACUE_API std::vector<const char*> AuraCUE::Functions::GetCueDeviceModels(bool bShouldPrintToConsole)
{
	std::vector<const char*> cueDevices;
	int numberOfDevices = CorsairGetDeviceCount();
	

	for (int i = 0; i < numberOfDevices; i++)
	{
		const char* deviceModel = CorsairGetDeviceInfo(i)->model;
		cueDevices.push_back(deviceModel);

		if (bShouldPrintToConsole)
		{
			std::cout << "Device model: " << deviceModel << std::endl << std::endl;
		}
	}

	return cueDevices;
}

AURACUE_API AuraCUE::CueDevice AuraCUE::Functions::GetCueDevice(int deviceIndex)
{
	CueDevice device;
	device.deviceModel = GetCueDeviceModel(deviceIndex);
	device.deviceType = GetCueDeviceType(deviceIndex);
	device.capsMask = GetCueDeviceCapsMask(deviceIndex);
	device.logicalLayout = GetCueDeviceLogicalLayout(deviceIndex);
	device.physicalLayout = GetCueDevicePhysicalLayout(deviceIndex);
	return device;
}

AURACUE_API std::vector<AuraCUE::CueDevice> AuraCUE::Functions::GetCueDevices()
{
	int numberOfDevices = CorsairGetDeviceCount();
	std::vector<AuraCUE::CueDevice> devices;

	for (int i = 0; i < numberOfDevices; i++)
	{
		devices.push_back(GetCueDevice(i));
	}

	return devices;
}

AURACUE_API int AuraCUE::Functions::GetNumberOfCueDevices()
{
	return CorsairGetDeviceCount();
}

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
