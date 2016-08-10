#include "addon.h"
#include <iostream>
#include <assert.h>
#include <Windows.h>
#include <ole2.h>
#include <rtscom.h>
#include <rtscom_i.c>
#include <comdef.h>

class EventHandler : public IStylusSyncPlugin
{
	HWND m_hWnd;
	LONG m_cRefCount = 1;
	IUnknown* m_punkFTMarshaller = nullptr;
public:
	EventHandler(HWND hWnd) : m_hWnd(hWnd)
	{
		auto hr = CoCreateFreeThreadedMarshaler(this, &m_punkFTMarshaller);
		assert(SUCCEEDED(hr));
	}
	~EventHandler()
	{
		m_punkFTMarshaller->Release();
	}

	STDMETHOD(StylusDown)(IRealTimeStylus* piSrcRtp, const StylusInfo* pStylusInfo, ULONG cPropCountPerPkt, LONG* pPacket, LONG** ppInOutPkt)
	{
		std::cout << "stylus down" << std::endl;
		return S_OK;
	}

	STDMETHOD(StylusUp)(IRealTimeStylus* piSrcRtp, const StylusInfo* pStylusInfo, ULONG cPropCountPerPkt, LONG* pPacket, LONG** ppInOutPkt)
	{
		std::cout << "stylus up" << std::endl;
		return S_OK;
	}

	STDMETHOD(Packets)(IRealTimeStylus* piSrcRtp, const StylusInfo* pStylusInfo, ULONG cPktCount, ULONG cPktBuffLength, LONG* pPackets, ULONG* pcInOutPkts, LONG** ppInOutPkts)
	{
		std::cout << "stylus move" << std::endl;
		return S_OK;
	}

	STDMETHOD(DataInterest)(RealTimeStylusDataInterest* pDataInterest)
	{
		*pDataInterest = (RealTimeStylusDataInterest)(RTSDI_Packets | RTSDI_Error);
		return S_OK;
	}

	STDMETHOD(RealTimeStylusEnabled)(IRealTimeStylus*, ULONG, const TABLET_CONTEXT_ID*) { return S_OK; }
	STDMETHOD(RealTimeStylusDisabled)(IRealTimeStylus*, ULONG, const TABLET_CONTEXT_ID*) { return S_OK; }
	STDMETHOD(StylusInRange)(IRealTimeStylus*, TABLET_CONTEXT_ID, STYLUS_ID) { return S_OK; }
	STDMETHOD(StylusOutOfRange)(IRealTimeStylus*, TABLET_CONTEXT_ID, STYLUS_ID) { return S_OK; }
	STDMETHOD(InAirPackets)(IRealTimeStylus*, const StylusInfo*, ULONG, ULONG, LONG*, ULONG*, LONG**) { return S_OK; }
	STDMETHOD(StylusButtonUp)(IRealTimeStylus*, STYLUS_ID, const GUID*, POINT*) { return S_OK; }
	STDMETHOD(StylusButtonDown)(IRealTimeStylus*, STYLUS_ID, const GUID*, POINT*) { return S_OK; }
	STDMETHOD(SystemEvent)(IRealTimeStylus*, TABLET_CONTEXT_ID, STYLUS_ID, SYSTEM_EVENT, SYSTEM_EVENT_DATA) { return S_OK; }
	STDMETHOD(TabletAdded)(IRealTimeStylus*, IInkTablet*) { return S_OK; }
	STDMETHOD(TabletRemoved)(IRealTimeStylus*, LONG) { return S_OK; }
	STDMETHOD(CustomStylusDataAdded)(IRealTimeStylus*, const GUID*, ULONG, const BYTE*) { return S_OK; }
	STDMETHOD(Error)(IRealTimeStylus*, IStylusPlugin*, RealTimeStylusDataInterest, HRESULT, LONG_PTR*) { return S_OK; }
	STDMETHOD(UpdateMapping)(IRealTimeStylus*) { return S_OK; }

	STDMETHOD_(ULONG, AddRef)()
	{
		return InterlockedIncrement(&m_cRefCount);
	}

	STDMETHOD_(ULONG, Release)()
	{
		auto cNewRefCount = InterlockedDecrement(&m_cRefCount);
		if (cNewRefCount == 0)
		{
			delete this;
		}
		return cNewRefCount;
	}

	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObj)
	{
		if ((riid == IID_IStylusSyncPlugin) || (riid == IID_IUnknown))
		{
			*ppvObj = this;
			AddRef();
			return S_OK;
		}
		else if (riid == IID_IMarshal)
		{
			return m_punkFTMarshaller->QueryInterface(riid, ppvObj);
		}
		*ppvObj = NULL;
		return E_NOINTERFACE;
	}
};

IRealTimeStylus* CreateRealtimeStylus(HWND hWnd)
{
	IRealTimeStylus* pRealTimeStylus = NULL;
	auto hr = CoCreateInstance(CLSID_RealTimeStylus, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pRealTimeStylus));
	assert(SUCCEEDED(hr));

	hr = pRealTimeStylus->put_HWND((HANDLE_PTR)hWnd);
	assert(SUCCEEDED(hr));

	return pRealTimeStylus;
}

void InterceptWindow(void *handle)
{
	auto hWnd = (HWND)handle;
	auto realTimeStylus = CreateRealtimeStylus(hWnd);
	auto handler = new EventHandler(hWnd);
	auto hr = realTimeStylus->AddStylusSyncPlugin(0, handler);
	assert(SUCCEEDED(hr));
	hr = realTimeStylus->put_Enabled(TRUE);
	std::cout << _com_error(hr).ErrorMessage() << std::endl;
	std::cout << std::hex << hr << std::endl;
	assert(SUCCEEDED(hr));
}

void UninterceptWindow(void *handle)
{

}

void Init()
{
}
