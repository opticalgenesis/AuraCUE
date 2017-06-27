#include "RogAuraService.h"
#include "stdafx.h"
#include <atlbase.h>


MIDL_INTERFACE("EBA0F532-3726-465B-9D21-E05C70C7263D")
IAuraDevice : public IDispatch
{
public:
	virtual /* [id] */ HRESULT STDMETHODCALLTYPE QuerySDKVersionNumber(
		/* [retval][out] */ BSTR *versionNum) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE QueryDeviceNameList(
		/* [retval][out] */ SAFEARRAY * *deviceNameList) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE QueryEffectCapList(
		/* [out] */ SAFEARRAY * *effectIdList,
		/* [out] */ SAFEARRAY * *effectNameList) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE QueryLedList(
		/* [out] */ SAFEARRAY * *ledIdList,
		/* [out] */ SAFEARRAY * *ledNameList) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE QueryCurrentEffect(
		/* [retval][out] */ BSTR *effectId) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetEffect(
		/* [in] */ BSTR effectId) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE QueryHslFromLed(
		/* [in] */ BSTR ledId,
		/* [out] */ float *h,
		/* [out] */ float *s,
		/* [out] */ float *l) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetHslToLed(
		/* [in] */ BSTR ledId,
		/* [in] */ float h,
		/* [in] */ float s,
		/* [in] */ float l) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE QuerySwitchState(
		/* [retval][out] */ VARIANT_BOOL *isOn) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSwitchState(
		/* [in] */ VARIANT_BOOL on) = 0;

	virtual /* [id] */ HRESULT STDMETHODCALLTYPE Apply(void) = 0;

};

const IID IID_IAuraDevice;

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)
MIDL_DEFINE_GUID(IID, IID_IAuraDevice, 0xEBA0F532, 0x3726, 0x465B, 0x9D, 0x21, 0xE0, 0x5C, 0x70, 0xC7, 0x26, 0x3D);



RogAuraService* RogAuraService::m_instance = nullptr;


RogAuraService* RogAuraService::Instance()
{
	if (!m_instance)
	{
		m_instance = new RogAuraService;
	}

	return m_instance;
}

RogAuraService::RogAuraService()
{
}


RogAuraService::~RogAuraService()
{
}

bool RogAuraService::QueryDevceNames(std::vector<std::wstring>& deviceNameList)
{
	bool _result = false;

	deviceNameList.clear();


	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		SAFEARRAY* _deviceNamesArray;
		hr = iAuraDevice->QueryDeviceNameList(&_deviceNamesArray);

		if (hr == S_OK)
		{
			BSTR* _pDeviceNames;
			hr = SafeArrayAccessData(_deviceNamesArray, (void**)&_pDeviceNames);
			if (hr == S_OK)
			{
				long _lowerBound, _upperBound;
				SafeArrayGetLBound(_deviceNamesArray, 1, &_lowerBound);
				SafeArrayGetUBound(_deviceNamesArray, 1, &_upperBound);


				long _deviceCount = _upperBound - _lowerBound + 1;
				for (int i = 0; i < _deviceCount; ++i)
				{
					deviceNameList.push_back(std::wstring(_pDeviceNames[i], SysStringLen(_pDeviceNames[i])));
				}
				SafeArrayUnaccessData(_deviceNamesArray);

				_result = true;
			}
			SafeArrayDestroy(_deviceNamesArray);
		}
	}

	return _result;
}


bool RogAuraService::QueryEffectCapList(std::vector<std::wstring>& effectIdList, std::vector<std::wstring>& effectNameList)
{
	bool _result = false;

	effectIdList.clear();
	effectNameList.clear();


	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		SAFEARRAY* _effectIdArray;
		SAFEARRAY* _effectNameArray;
		hr = iAuraDevice->QueryEffectCapList(&_effectIdArray, &_effectNameArray);

		if (hr == S_OK)
		{
			BSTR* _pIdArray;
			BSTR* _pNameArray;
			hr = SafeArrayAccessData(_effectIdArray, (void**)&_pIdArray);
			hr = SafeArrayAccessData(_effectNameArray, (void**)&_pNameArray);
			if (hr == S_OK)
			{
				long _lowerBound, _upperBound;
				SafeArrayGetLBound(_effectIdArray, 1, &_lowerBound);
				SafeArrayGetUBound(_effectIdArray, 1, &_upperBound);
				long _effectCapCount = _upperBound - _lowerBound + 1;
				for (int i = 0; i < _effectCapCount; ++i)
				{
					effectIdList.push_back(std::wstring(_pIdArray[i], SysStringLen(_pIdArray[i])));
					effectNameList.push_back(std::wstring(_pNameArray[i], SysStringLen(_pNameArray[i])));
				}
				SafeArrayUnaccessData(_effectIdArray);
				SafeArrayUnaccessData(_effectNameArray);

				_result = true;
			}
			SafeArrayDestroy(_effectIdArray);
			SafeArrayDestroy(_effectNameArray);
		}
	}

	return _result;
}

bool RogAuraService::QueryLedList(std::vector<std::wstring>& ledIdList, std::vector<std::wstring>& ledNameList)
{
	bool _result = false;

	ledIdList.clear();
	ledNameList.clear();


	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		SAFEARRAY* _psaIdArray;
		SAFEARRAY* _psaNameArray;
		hr = iAuraDevice->QueryLedList(&_psaIdArray, &_psaNameArray);

		if (hr == S_OK)
		{
			BSTR* _pIdArray;
			BSTR* _pNameArray;
			hr = SafeArrayAccessData(_psaIdArray, (void**)&_pIdArray);
			hr = SafeArrayAccessData(_psaNameArray, (void**)&_pNameArray);
			if (hr == S_OK)
			{
				long _lowerBound, _upperBound;
				SafeArrayGetLBound(_psaIdArray, 1, &_lowerBound);
				SafeArrayGetUBound(_psaIdArray, 1, &_upperBound);
				long _ledCount = _upperBound - _lowerBound + 1;
				for (int i = 0; i < _ledCount; ++i)
				{
					ledIdList.push_back(std::wstring(_pIdArray[i], SysStringLen(_pIdArray[i])));
					ledNameList.push_back(std::wstring(_pNameArray[i], SysStringLen(_pNameArray[i])));
				}
				SafeArrayUnaccessData(_psaIdArray);
				SafeArrayUnaccessData(_psaNameArray);

				_result = true;
			}
			SafeArrayDestroy(_psaIdArray);
			SafeArrayDestroy(_psaNameArray);
		}
	}

	return _result;
}

bool RogAuraService::QueryCurrentEffect(std::wstring& effectId)
{
	bool _result = false;

	effectId = L"";


	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		BSTR _bstrEffectId;
		hr = iAuraDevice->QueryCurrentEffect(&_bstrEffectId);

		if (hr == S_OK)
		{
			std::wstring _ws(_bstrEffectId, SysStringLen(_bstrEffectId));
			effectId = _ws;

			_result = true;
		}
	}

	return _result;
}

bool RogAuraService::SetEffect(std::wstring effectId)
{
	bool _result = false;

	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		BSTR _bstrEffectId = SysAllocStringLen(effectId.data(), effectId.size());
		hr = iAuraDevice->SetEffect(_bstrEffectId);

		if (hr == S_OK)
			_result = true;
	}

	return _result;
}

bool RogAuraService::QueryHslFromLed(std::wstring ledId, float& h, float& s, float& l)
{
	bool _result = false;

	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		BSTR _bstrLedId = SysAllocStringLen(ledId.data(), ledId.size());
		hr = iAuraDevice->QueryHslFromLed(_bstrLedId, &h, &s, &l);

		if (hr == S_OK)
			_result = true;
	}

	return _result;
}

bool RogAuraService::SetHslToLed(std::wstring ledId, float h, float s, float l)
{
	bool _result = false;

	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		BSTR _bstrLedId = SysAllocStringLen(ledId.data(), ledId.size());
		hr = iAuraDevice->SetHslToLed(_bstrLedId, h, s, l);

		if (hr == S_OK)
			_result = true;
	}

	return _result;
}

bool RogAuraService::QuerySwitchState()
{
	bool _result = false;

	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		VARIANT_BOOL _isOn;
		hr = iAuraDevice->QuerySwitchState(&_isOn);

		if (hr == S_OK)
			_result = (VARIANT_TRUE == _isOn);
	}


	return _result;
}

bool RogAuraService::SetSwitchState(bool on)
{
	bool _result = false;

	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		VARIANT_BOOL _on = on ? VARIANT_TRUE : VARIANT_FALSE;
		hr = iAuraDevice->SetSwitchState(_on);

		if (hr == S_OK)
			_result = true;
	}

	return _result;
}

bool RogAuraService::Apply()
{
	bool _result = false;

	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(OLESTR("AsRogAuraService.AuraDevice"), &clsid);
	IAuraDevice *iAuraDevice;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IAuraDevice), (LPVOID*)&iAuraDevice);

	if (hr == S_OK)
	{
		hr = iAuraDevice->Apply();

		if (hr == S_OK)
			_result = true;
	}

	return _result;
}
