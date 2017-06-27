#include "stdafx.h"

// TODO -- 25/06/2017 -- implement proper exception classes in place of std::cerr

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

	std::cout << CorsairGetLastError << std::endl;
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