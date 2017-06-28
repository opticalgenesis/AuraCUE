#pragma once

#include <string>
#include <vector>

namespace RogData
{
	class Enums
	{
	public:
		enum AuraDeviceType
		{
			ADT_INVALID,
			ADT_KEYBOARD,
			ADT_MOUSE,
			ADT_MOBO,
			ADT_GPU,
			ADT_SLI,
			ADT_DESKTOP,
			ADT_MIC
		};
	};

	class Structs
	{
	public:
		struct AuraDevice
		{
			std::wstring modelName;
			Enums::AuraDeviceType deviceType;
		};
	};
}

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
