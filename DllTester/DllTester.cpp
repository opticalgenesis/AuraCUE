#include "stdafx.h"

using namespace AuraCUE;

int main()
{
	Functions funcs;
	funcs.Initialize(true, false);
	std::vector<AuraCUE::RgbDevice> devices = funcs.GetNormalizedDevices();
	for (size_t i = 0; i < devices.size(); i++)
	{
		std::wcout << "Devices:\n" << "Model: " << devices[i].deviceName << std::endl << "Type: " << devices[i].deviceType << std::endl
			<< "Brand: " << devices[i].deviceBrand << std::endl << std::endl;
	}
    return 1;
}

