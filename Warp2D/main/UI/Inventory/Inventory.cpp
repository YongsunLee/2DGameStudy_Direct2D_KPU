#include "stdafx.h"
#include "Framework\Warp2DFramework.h"
#include "Framework\IndRes\IndRes.h"
#include "Scene\Scene.h"
#include "Object\item\Item.h"
#include "Inventory.h"


CUIInventory::CUIInventory()
{
}

CUIInventory::~CUIInventory()
{
}

bool CUIInventory::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		break;
	default:
		return true;
	}
	return(true);
}

bool CUIInventory::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:	break;
	case WM_MBUTTONDOWN:	break;
	case WM_RBUTTONDOWN:	break;
	case WM_MOUSEMOVE:		break;
	case WM_LBUTTONUP:		break;
	case WM_MBUTTONUP:		break;
	case WM_RBUTTONUP:		break;
	case WM_MOUSEWHEEL:		break;
	default:
		return false;
	}

	return(true);
}

void CUIInventory::DrawClient(ID2D1HwndRenderTarget * renderTarget)
{
	renderTarget->FillRectangle(m_rcClient, m_hbrushClient.Get());
	
	for (int i = 0; i < 2; ++i) {
		renderTarget->DrawBitmap(
			m_bmpFrame[i].Get()
			, m_rcItem
		);
	}

	if (m_pItem) {
		renderTarget->DrawBitmap(
			m_pItem->GetBitmap()
			, m_rcItem
		);
	}
}

void CUIInventory::DrawCaption(ID2D1HwndRenderTarget * renderTarget)
{
	renderTarget->FillRectangle(m_rcCaption, m_hbrushCaption.Get());

	renderTarget->DrawText(
		  m_strTitle.c_str()
		, static_cast<UINT>(m_strTitle.length())
		, m_dwTextFormat.Get()
		, m_rcCaption
		, m_hbrushText.Get()
	);

}

void CUIInventory::BuildObject(CScene * scene)
{
	auto framework = scene->Framework();
	auto indres = framework->GetIndRes();
	auto renderTarget = framework->GetRenderTarget();
	
	m_rcCaption = RectF(0, 0, 200, 20);
	m_rcClient = RectF(0, 0, 200, 220);

	// Origin �̵� ���� ���� �ʿ�
	m_ptOrigin = Point2F(560, 20);

	// Item
	for (int i = 0; i < 2; ++i) {
		m_rcItem = RectF(10, 10 + (i * 50), 50, 50 + (i * 50));
		LoadImageFromFile(
			indres->wicFactory()
			, renderTarget.Get()
			, L"Assets/Icon/Icon Frame.png"
			, &m_bmpFrame[i]
		);
	}

	// Brush �ʱ�ȭ �� ����
	renderTarget->CreateSolidColorBrush(ColorF{ ColorF::GreenYellow }, &m_hbrushCaption);
	renderTarget->CreateSolidColorBrush(ColorF{ ColorF::White }, &m_hbrushClient);
	renderTarget->CreateSolidColorBrush(ColorF{ ColorF::Black }, &m_hbrushText);

	// �����찡 ������ �ִ� ��Ʈ ��� ��밡��
	// But �����̴ϱ� �ٸ� ����� PC�� ��ġ�� ��Ʈ�� ����ؾߵȴ�.
	// �ذ����� ��������Բ� ����
	indres->dwFactory()->CreateTextFormat(
		L"�ü�ü"										
		, nullptr
		, DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_NORMAL
		, (m_rcCaption.bottom - m_rcCaption.top) * 0.8f
		, L"ko-kr"
		, &m_dwTextFormat
	);

	// s�� �����ϱ�
	// UDL ���� ������ ���ͳ�
	// "  "s -> string�̴�.
	// L"  "s -> wstring�̴�.
	m_strTitle = L"Inventory"s;

}

void CUIInventory::Update(float fTimeElapsed)
{
}
