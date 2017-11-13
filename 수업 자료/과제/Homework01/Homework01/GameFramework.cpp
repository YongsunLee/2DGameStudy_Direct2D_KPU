#include "stdafx.h"
#include "GameObject.h"
#include "GameFramework.h"


CGameFramework::CGameFramework()
{
	m_pd2dFactory = nullptr;
	m_pdwFactory = nullptr;
	m_prenderTarget = nullptr;
	m_ptOldCursorPos = {0,0};
}


CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HWND hWnd)
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

	CreateHwndRenderTarget(hWnd, &m_prenderTarget, WINWIDTH, WINHEIGHT);

	return true;
}

void CGameFramework::BuildObjects()
{
	m_pGameObject = new CGameObject;

	m_pGameObject->SetPosition(Vec3{ 150.f,150.f,0.f });
	m_pGameObject->SetSize(50.f);
}

void CGameFramework::ReleaseObjects()
{
	
}

void CGameFramework::Render()
{
	m_prenderTarget->BeginDraw();
	m_prenderTarget->Clear(ColorF(ColorF::Black));

	m_pGameObject->Render(m_prenderTarget);

	// EndDraw
	// 어떤 문제가 있으면 반환값을 준다. 
	m_prenderTarget->EndDraw();
}

void CGameFramework::ProcessInput()
{
	
}

void CGameFramework::FrameAdvance()
{
	m_pGameObject->Update();
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//마우스 캡쳐를 하고 현재 마우스 위치를 가져온다.
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		//마우스 캡쳐를 해제한다.
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}