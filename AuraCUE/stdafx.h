#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here
#include "AuraCUE.h"
#include <iostream>
#include <CUESDK.h>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>
#include <algorithm>
#include <atlbase.h>
#include <exception>
#include <stdexcept>
#include <CUESDKGlobal.h>
#include <AURALightingSDK.h>
#include <CorsairLedIdEnum.h>


std::string GetCueError(CorsairError err);

std::string GetCueDeviceModel(int deviceIndex);
std::string GetCueDeviceType(int deviceIndex);
std::string GetCueDevicePhysicalLayout(int deviceIndex);
std::string GetCueDeviceLogicalLayout(int deviceIndex);
int GetCueDeviceCapsMask(int deviceIndex);

AuraCUE::AuraDeviceType GetAuraDeviceType(std::wstring);

std::wstring StringToWstring(std::string input);

AuraCUE::RgbDeviceType GetAuraRgbDeviceType(AuraCUE::AuraDeviceType input);
AuraCUE::RgbDeviceType GetCueDeviceRgbType(std::string input);