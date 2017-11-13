#include "stdafx.h"
#include "DInRes.h"

CDInRes::CDInRes()
{
}

CDInRes::~CDInRes()
{
}

bool CDInRes::Initialize()
{
	D2D1_FACTORY_OPTIONS options{};

#if defined(_DEBUG)
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	// D2DFactory
	if (FAILED(D2D1CreateFactory<ID2D1Factory3>(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &m_pd2dFactory)))
	{
		MessageBox(nullptr, TEXT("D2D Factory 생성에 실패하였습니다."), TEXT("Error"), MB_OK);
		return false;
	}
	
	// DWriteFactory
	if (FAILED(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(decltype(m_pdwFactory)::InterfaceType),
		&m_pdwFactory
	)))
	{
		MessageBox(nullptr, TEXT("Dwirte Factory 생성에 실패하였습니다."), TEXT("Error"), MB_OK);
		return false;
	}
	
	if ( FAILED (CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
	{
		MessageBox(nullptr, TEXT("COM 객체 초기화에 실패하였습니다."), TEXT("Error"), MB_OK);
		return false;
	}

	// IID는 MSDN 참조하자 이해할 필요 x
	// CLSCTX :: 특수한 이름이다. 
	// 특수한 목적에 사용 되는 여러가지 것들 필요하면 MSDN 참조
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, 
								nullptr, 
								CLSCTX_INPROC_SERVER, 
								IID_PPV_ARGS(&m_wicFacory))))
	{
		MessageBox(nullptr, TEXT("WIC 객체 생성에 실패하였습니다."), TEXT("Error"), MB_OK);
		return false;
	}

	return true;
}