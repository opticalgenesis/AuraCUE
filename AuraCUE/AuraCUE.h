#pragma once

#ifdef AURACUE_EXPORTS
#define AURACUE_API __declspec(dllexport)
#else
#define AURACUE_API __declspec(dllimport)
#endif


namespace AuraCUE
{
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
		// Returns List Of Connected CUE Devices
		// TODO -- 27/06/2017 -- Implement a method that returns total list of both SDK devices
		AURACUE_API std::vector<const char*> GetCueModelNames();
	};
}