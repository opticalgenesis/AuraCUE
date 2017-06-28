#pragma once

#include <string>
#include <vector>

class RogAuraService
{
public:
	static RogAuraService* Instance();


	bool QueryDeviceNames(std::vector<std::wstring>& deviceNameList);
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
