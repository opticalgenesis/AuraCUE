#pragma once

#include <string>
#include <vector>


class RogAuraService
{
public:
	static RogAuraService* Instance();


	bool QueryDevceNames(std::vector<std::wstring>& deviceNameList);
	bool QueryEffectCapList(std::vector<std::wstring>& effectIdList, std::vector<std::wstring>& effectNameList);
	bool QueryLedList(std::vector<std::wstring>& ledIdList, std::vector<std::wstring>& ledNameList);

	bool QueryCurrentEffect(std::wstring& effectId);
	bool SetEffect(std::wstring effectId);

	bool QueryHslFromLed(std::wstring ledId, float& h, float& s, float& l);
	bool SetHslToLed(std::wstring ledId, float h, float s, float l);

	bool QuerySwitchState();
	bool SetSwitchState(bool on);

	bool Apply();


private:
	RogAuraService();
	~RogAuraService();

	static RogAuraService* m_instance;
};

namespace RogEnums
{	
	enum AuraDeviceType
	{
		ADT_KEYBOARD,
		ADT_MOUSE,
		ADT_MOBO,
		ADT_GPU,
		ADT_SLI,
		ADT_DESKTOP
	};
}

namespace RogStructs
{
	struct AuraDevice
	{
		std::string deviceName;
		RogEnums::AuraDeviceType deviceType;
	};
}