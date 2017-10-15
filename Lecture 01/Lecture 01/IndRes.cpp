#include "stdafx.h"
#include "IndRes.h"

CIndRes::CIndRes()
{
}

CIndRes::~CIndRes()
{
}

bool CIndRes::Initialize()
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



	return true;
}