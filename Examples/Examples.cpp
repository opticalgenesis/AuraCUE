#include "stdafx.h"

using namespace AuraCUE;

int main()
{
	InitiailizeCompleteSdk();
    return 0;
}

void InitiailizeCompleteSdk()
{
	Functions funcs;
	funcs.Initialize(true, true);
	std::vector<AuraDevice> auraDevices = funcs.GetAuraDevices();
	std::vector<CueDevice> cueDevices = funcs.GetCueDevices();
	std::vector<RgbDevice> rgbDevices = funcs.GetNormalizedDevices();

	for (size_t i = 0; i < auraDevices.size(); i++)
	{
		std::wcout << "Aura Devices:\n\n" << "Model: " << auraDevices[i].modelName << std::endl << "Type: " << auraDevices[i].deviceType << std::endl << std::endl;
	}

	for (size_t j = 0; j < cueDevices.size(); j++)
	{
		std::cout << "Cue Devices:\n\n" << "Model: " << cueDevices[j].deviceModel << std::endl << "Type: " << cueDevices[j].deviceType << std::endl
			<< "Physical layout: " << cueDevices[j].physicalLayout << std::endl << "Logical layout: " << cueDevices[j].logicalLayout
			<< std::endl << "Caps mask: " << cueDevices[j].capsMask << std::endl << std::endl;
	}

	for (size_t k = 0; k < rgbDevices.size(); k++)
	{
		std::wcout << "RGB Devices:\n\n" << "Model: " << rgbDevices[k].deviceName << std::endl << "Type: " << rgbDevices[k].deviceType
			<< std::endl << "Brand: " << rgbDevices[k].deviceBrand << std::endl << std::endl;
	}
}

void InitializeCueSdk()
{
	Functions funcs;
	funcs.Initialize(true, false);
	std::vector<CueDevice> cueDevices = funcs.GetCueDevices();

	for (size_t j = 0; j < cueDevices.size(); j++)
	{
		std::cout << "Cue Devices:\n\n" << "Model: " << cueDevices[j].deviceModel << std::endl << "Type: " << cueDevices[j].deviceType << std::endl
			<< "Physical layout: " << cueDevices[j].physicalLayout << std::endl << "Logical layout: " << cueDevices[j].logicalLayout
			<< std::endl << "Caps mask: " << cueDevices[j].capsMask << std::endl << std::endl;
	}
}

void InitializeAuraSdk()
{
	Functions funcs;
	funcs.Initialize(false, true);
	std::vector<AuraDevice> auraDevices = funcs.GetAuraDevices();

	for (size_t i = 0; i < auraDevices.size(); i++)
	{
		std::wcout << "Aura Devices:\n\n" << "Model: " << auraDevices[i].modelName << std::endl << "Type: "
			<< auraDevices[i].deviceType << std::endl << std::endl;
	}
}

