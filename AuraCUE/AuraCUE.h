#pragma once

#ifdef AURACUE_EXPORTS
#define AURACUE_API __declspec(dllexport)
#else
#define AURACUE_API __declspec(dllimport)
#endif


// Must be here, for some reason
#include "RogAuraService.h"
#include <vector>

namespace AuraCUE
{
	// CueDevice Struct
	AURACUE_API struct CueDevice
	{
		std::string deviceType;
		std::string deviceModel;
		std::string physicalLayout;
		std::string logicalLayout;
		int capsMask;
	};

	class Functions
	{
	public:
		// Initialises RGB APIs
		AURACUE_API void Initialize(bool bShouldUseCorsair, bool bShouldUseAura);
		// Enables/Disables Exclusive SDK Access To The CUE SDK
		AURACUE_API void CorsairShouldUseExclusiveAccess(bool bIsExclusive);
		// Checks To See If Both SDKs Have Been Initialised Successfully
		AURACUE_API bool IsSdkInitialized(bool bShouldPrintToConsole);
		// Returns Total Number Of RGB Devices Connected
		AURACUE_API int NumberOfRgbDevices(bool bShouldPrintToConsole);
		// Returns List Of Connected CUE Device Models
		// TODO -- 27/06/2017 -- Implement a method that returns total list of both SDK devices
		AURACUE_API std::vector<const char*> GetCueDeviceModels(bool bShouldPrintToConsole);
		// Returns CUE Device for index in CorsairDevices list
		// Device Index should not exceed the size of ^^ list
		AURACUE_API CueDevice GetCueDevice(int deviceIndex);
		AURACUE_API std::vector<CueDevice> GetCueDevices();
		// Returns Number of CUE Devices connected
		AURACUE_API int GetNumberOfCueDevices();
		// Returns Connected Aura Sync Device at index
		AURACUE_API RogData::Structs::AuraDevice GetAuraDevice(int deviceIndex);
	};
}