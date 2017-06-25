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
		static AURACUE_API void Initialize(bool bShouldUseCorsair, bool bShouldUseAura);
		static AURACUE_API void CorsairShouldUseExclusiveAccess(bool bIsExclusive);
		static AURACUE_API bool IsCueInitialized();
		static AURACUE_API int NumberOfRgbDevices(bool bShouldPrintToConsole);
	};
}