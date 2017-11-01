#include "stdafx.h"

#include "Framework/Warp2DFramework.h"
#include "Framework/IndRes/IndRes.h"

#include "TestScene.h"


CTestScene::CTestScene()
{
}


CTestScene::~CTestScene()
{
}

bool CTestScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEWHEEL:	
		break;

		// 프레임 워크 단에서 동작하기 위해서는 false
	default:
		return false;
	}

	return(true);
}

bool CTestScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:
	{
		// Camera
		// 윈도우 크기 
		auto rcClient = m_pFramework->GetClientSize();
		m_Camera.SetClientSize(Point2F(rcClient.right, rcClient.bottom));
	}
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEWHEEL:
		return OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		return OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	
		// 프레임 워크 단에서 동작하기 위해서는 false
	default:
		return false;
	}

	return true;
}

bool CTestScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'A':
			m_ptPlayer.x -= 10.f;
			break;
		case 'W':
			m_ptPlayer.y -= 10.f;
			break;
		case 'D':
			m_ptPlayer.x += 10.f;
			break;
		case 'S':
			m_ptPlayer.y += 10.f;
			break;
		case 'Z':
			m_Camera.Scale(m_Camera.GetScale() * 2.f);
			break;
		case 'X':
			m_Camera.Scale(m_Camera.GetScale() * 0.5f);
			break;

			// 프레임 워크 단에서 동작하기 위해서는 false
		default:
			return false;
		}
		break;
	
	default:
		return false;
	}
	return(true);
}

bool CTestScene::OnCreate(wstring && tag, CWarp2DFramework * pFramework)
{
	if (!Base::OnCreate(std::move(tag), pFramework)) return false;

	// Camera
	// 윈도우 크기 
	auto rcClient = pFramework->GetClientSize();
	m_Camera.SetClientSize(Point2F(rcClient.right, rcClient.bottom));

	auto rendertarget = pFramework->GetRenderTarget();

	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::Crimson }, &m_pd2dsbrDefault);
	
	/*
	auto dwFactoy = m_pIndRes->dwFactory();

	dwFactoy->CreateTextFormat(
	  L"Arial"
	, nullptr
	, DWRITE_FONT_WEIGHT_NORMAL
	, DWRITE_FONT_STYLE_NORMAL
	, DWRITE_FONT_STRETCH_NORMAL
	, 30
	, L"ko-kr"
	, &m_pdwTextFormat);

	m_pdwTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	m_pdwTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	dwFactoy->CreateTextLayout(
		  L"Hello, World!"
		, 13
		, m_pdwTextFormat.Get()
		, 300
		, 100
		, &m_pdwTextLayout);
	
	auto wicFactory = m_pIndRes->wicFactory();
	LoadImageFromFile(wicFactory
		, rendertarget.Get()
		, L"Hit1.png"
		, &m_pd2dbmpTest
	);
	*/
	
	m_Camera.SetPosition(m_ptPlayer);
	m_Camera.SetAnchor(Point2F(-0.5f, -0.5f));		// 조금 고치기
	
	return true;
}

void CTestScene::Update(float fTimeElapsed)
{
	m_Camera.SetPosition(m_ptPlayer);
}

void CTestScene::Draw(ID2D1HwndRenderTarget * pd2dRenderTarget)
{
	auto cameramtx = m_Camera.RegenerateViewMatrix();
	pd2dRenderTarget->SetTransform(cameramtx);

	pd2dRenderTarget->FillRectangle(RectF(-10, -10, 10, 10) + m_ptPlayer, m_pd2dsbrDefault.Get());


	pd2dRenderTarget->DrawRectangle(RectF(50, 50, 70, 70), m_pd2dsbrDefault.Get());

}
