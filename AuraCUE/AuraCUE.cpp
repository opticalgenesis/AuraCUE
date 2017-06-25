#include "stdafx.h"

// TODO -- 25/06/2017 -- implement proper exception classes in place of std::cerr

// Used to determine whether or not the Intitialize method has been run
// This MUST be true for SDK operations to proceed
bool bIsSdkInitialized = false;

// First bool is CUE, second is AURA
AURACUE_API void AuraCUE::Functions::Initialize(bool bShouldUseCorsair, bool bShouldUseAura)
{

	bool bCueIsInitialized;
	bool bAuraIsInitialized;

	CorsairPerformProtocolHandshake();
	// Throws CorsairHandshakeException if GetLastError isn't successful
	if (CorsairGetLastError() != CE_Success)
	{
		// Error thrown and bool remains/reset to false
		bCueIsInitialized = false;
	}
	else
	{
		// Handshake was a success, CUE is initialised
		bCueIsInitialized = true;
	}

	bool bAreAuraLedsOn = RogAuraService::Instance()->QuerySwitchState();

	if (bAreAuraLedsOn)
	{
		bAuraIsInitialized = true;
	}
	else
	{
		std::cout << "Aura LEDs are off, turning on...";
		RogAuraService::Instance()->SetSwitchState(true);
		bAuraIsInitialized = true;
	}
	if (bCueIsInitialized && bAreAuraLedsOn)
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
}

AURACUE_API bool AuraCUE::Functions::IsCueInitialized()
{
	return bIsSdkInitialized;
}

AURACUE_API int AuraCUE::Functions::NumberOfRgbDevices(bool bShouldPrintToConsole)
{
	if (bIsSdkInitialized)
	{
		int numberOfDevices = 0;
		std::vector<std::wstring> auraDevices;
		RogAuraService::Instance()->QueryDevceNames(auraDevices);
		int numberOfAuraDevices = sizeof(auraDevices);
		numberOfDevices += numberOfAuraDevices;
		int numberOfCueDevices = CorsairGetDeviceCount();
		numberOfDevices += numberOfCueDevices;
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