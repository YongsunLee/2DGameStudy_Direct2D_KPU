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
		MessageBox(nullptr, TEXT("D2D Factory ������ �����Ͽ����ϴ�."), TEXT("Error"), MB_OK);
		return false;
	}
	
	// DWriteFactory
	if (FAILED(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(decltype(m_pdwFactory)::InterfaceType),
		&m_pdwFactory
	)))
	{
		MessageBox(nullptr, TEXT("Dwirte Factory ������ �����Ͽ����ϴ�."), TEXT("Error"), MB_OK);
		return false;
	}
	
	if ( FAILED (CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
	{
		MessageBox(nullptr, TEXT("COM ��ü �ʱ�ȭ�� �����Ͽ����ϴ�."), TEXT("Error"), MB_OK);
		return false;
	}

	// IID�� MSDN �������� ������ �ʿ� x
	// CLSCTX :: Ư���� �̸��̴�. 
	// Ư���� ������ ��� �Ǵ� �������� �͵� �ʿ��ϸ� MSDN ����
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, 
								nullptr, 
								CLSCTX_INPROC_SERVER, 
								IID_PPV_ARGS(&m_wicFacory))))
	{
		MessageBox(nullptr, TEXT("WIC ��ü ������ �����Ͽ����ϴ�."), TEXT("Error"), MB_OK);
		return false;
	}

	return true;
}