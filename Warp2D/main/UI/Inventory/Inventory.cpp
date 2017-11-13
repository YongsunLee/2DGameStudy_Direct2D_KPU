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

void CUIInventory::DrawClient(ID2D1HwndRenderTarget * renderTarget)
{
	renderTarget->FillRectangle(m_rcClient, m_hbrushClient.Get());
	
	renderTarget->DrawBitmap(
		m_bmpFrame.Get()
		, m_rcItem
	);

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

	// Origin 이동 연산 제작 필요
	m_ptOrigin = Point2F(560, 20);

	// Item
	m_rcItem = RectF(10, 10, 50, 50);
	LoadImageFromFile(
		  indres->wicFactory()
		, renderTarget.Get()
		, L"Assets/Icon/Icon Frame.png"
		, &m_bmpFrame
	);

	// Brush 초기화 및 색상
	renderTarget->CreateSolidColorBrush(ColorF{ ColorF::GreenYellow }, &m_hbrushCaption);
	renderTarget->CreateSolidColorBrush(ColorF{ ColorF::White }, &m_hbrushClient);
	renderTarget->CreateSolidColorBrush(ColorF{ ColorF::Black }, &m_hbrushText);

	// 윈도우가 가지고 있는 폰트 모두 사용가능
	// But 게임이니까 다른 사람의 PC에 설치된 폰트를 고려해야된다.
	// 해결방법은 도율선배님께 질문
	indres->dwFactory()->CreateTextFormat(
		L"궁서체"										
		, nullptr
		, DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_NORMAL
		, (m_rcCaption.bottom - m_rcCaption.top) * 0.8f
		, L"ko-kr"
		, &m_dwTextFormat
	);

	// s는 무엇일까
	// UDL 유저 디파인 리터널
	// "  "s -> string이다.
	// L"  "s -> wstring이다.
	m_strTitle = L"Inventory"s;

}

void CUIInventory::Update(float fTimeElapsed)
{
}
