#include "stdafx.h"
#include "Framework/Warp2DFramework.h"
#include "Framework/IndRes/IndRes.h"
#include "Scene/Scene.h"
#include "Equipment.h"

CUIEquipment::CUIEquipment(CPlayer & player)
	: m_refPlayer{ player }
	, m_reflstItem{ player.m_lstItem }
{
}

CUIEquipment::~CUIEquipment()
{
}

void CUIEquipment::DrawClient(ID2D1HwndRenderTarget * pd2dRenderTarget)
{
	pd2dRenderTarget->FillRectangle(m_rcClient, m_hbrClient.Get());

	pd2dRenderTarget->DrawBitmap(m_bmpFrame.Get(), m_ptWeapon + m_rcItem);
	pd2dRenderTarget->DrawBitmap(m_bmpFrame.Get(), m_ptShield + m_rcItem);
	
	if (m_upWeapon)
	{
		pd2dRenderTarget->DrawBitmap(
			  m_upWeapon->GetBitmap()
			, m_ptWeapon + m_rcItem 
		);
	}

	if (m_upShield)
	{
		pd2dRenderTarget->DrawBitmap(
			  m_upShield->GetBitmap()
			, m_ptShield + m_rcItem
		);
	}
}

void CUIEquipment::DrawCaption(ID2D1HwndRenderTarget * pd2dRenderTarget)
{
	pd2dRenderTarget->FillRectangle(m_rcCaption, m_hbrCaption.Get());
	pd2dRenderTarget->DrawText(
		  m_strTitle.c_str()
		, static_cast<UINT>(m_strTitle.length())
		, m_dwTextFormat.Get()
		, m_rcCaption
		, m_hbrText.Get());
}

void CUIEquipment::BuildObject(CScene * scene)
{
	auto framework = scene->Framework();
	auto indres = framework->GetIndRes();
	auto rendertarget = framework->GetRenderTarget();

	m_rcCaption = RectF(0, 0, 200, 20);
	m_rcClient = RectF(0, 0, 200, 120);

	m_ptOrigin = Point2F(60, 20);

	m_rcItem = RectF(0, 0, 40, 40);

	m_szItemMatrix = SizeU(4, 4);
	m_szItemOutlineMargin = SizeF(10, 20);

	auto fClientWidth = m_rcClient.right - m_rcClient.left;
	auto fItemWidths = (m_rcItem.right - m_rcItem.left) * m_szItemMatrix.width;

	m_szItemBetweenMargin.width = (m_szItemMatrix.width == 1 ? 0 
		: (fClientWidth - (fItemWidths + 2.f * m_szItemOutlineMargin.width)) 
		/ static_cast<float>(m_szItemMatrix.width - 1)
	);

	m_szItemBetweenMargin.height = m_szItemBetweenMargin.width;

	m_ptWeapon = Point2F(m_szItemOutlineMargin.width, m_szItemOutlineMargin.height);
	m_ptShield = Point2F(m_szItemOutlineMargin.width, m_szItemOutlineMargin.height + (m_rcItem.bottom - m_rcItem.top) + m_szItemBetweenMargin.height);

	LoadImageFromFile(
		  indres->wicFactory()
		, rendertarget.Get()
		, L"Graphics/Icon/Icon Frame.png"
		, &m_bmpFrame);

	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::Yellow }, &m_hbrCaption);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::DimGray }, &m_hbrClient);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::Black }, &m_hbrText);
	
	indres->dwFactory()->CreateTextFormat(
		  L"Arial"
		, nullptr
		, DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_NORMAL
		, (m_rcCaption.bottom - m_rcCaption.top) * 0.8f
		, L"ko-kr"
		, &m_dwTextFormat);

	m_strTitle = L"Equipment"s;
}

void CUIEquipment::Update(float fTimeElapsed)
{
	for (auto iter = begin(m_reflstItem); iter != end(m_reflstItem);)
	{
		auto p = iter->get();
		if (p->ItemType() != CItem::TYPE::Equipment)
		{
			++iter;
			continue;
		}

		auto equip = static_cast<CEquipmentItem*>(p);
		if (equip->IsEquipment())
		{
			auto item { move(*iter) };
			iter = m_reflstItem.erase(iter);
			Equipment(move(item));
			continue;
		}

		++iter;
	}
}

void CUIEquipment::ChangeEquipment()
{
	auto& buff = m_refPlayer.m_EquipBuff;
	buff = Buff{};

	if (m_upWeapon)	buff += m_upWeapon->GetBuff();
	if (m_upShield) buff += m_upShield->GetBuff();
}

void CUIEquipment::Equipment(unique_ptr<CItem>&& item)
{
	using EType = CEquipmentItem::TYPE;
	auto pItem = static_cast<CEquipmentItem*>(item.get());
	if (pItem->GetType() == EType::sword)
	{
		if (m_upWeapon)
		{
			m_upWeapon->Equipment(false);
			m_reflstItem.push_back(move(m_upWeapon));
		}

		//	옳지 않은 코딩의 예시
		m_upWeapon = reinterpret_cast<unique_ptr<CEquipmentItem>&&>(move(item));
		m_upWeapon->Equipment(true);
	}
	else if (pItem->GetType() == EType::shield)
	{
		if (m_upShield)
		{
			m_upShield->Equipment(false);
			m_reflstItem.push_back(move(m_upShield));
		}
		//	그나마 나은 코딩의 예시
		m_upShield = unique_ptr<CEquipmentItem>{ dynamic_cast<CEquipmentItem *>(item.release()) };
		m_upShield->Equipment(true);
	}

	ChangeEquipment();
}

void CUIEquipment::Disarm(CEquipmentItem::TYPE type)
{
	using EType = CEquipmentItem::TYPE;
	switch (type)
	{
	case EType::sword:
		m_upWeapon->Equipment(false);
		m_reflstItem.push_back(move(m_upWeapon));
		break;
	case EType::shield:
		m_upShield->Equipment(false);
		m_reflstItem.push_back(move(m_upShield));
		break;
	}
	ChangeEquipment();
}

bool CUIEquipment::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CUIEquipment::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		{ 
			if (!m_refPlayer.IsActive()) break;

			auto pt = Point2F(LOWORD(lParam), HIWORD(lParam)) - (m_ptOrigin + Point2F(0, m_rcCaption.bottom));
			if (!PtInRect(&m_rcClient, pt)) return false;

			auto rc = m_rcItem + m_ptWeapon;
			if (PtInRect(&rc, pt))
			{
				if (m_upWeapon) Disarm(m_upWeapon->GetType());
			}
			rc = m_rcItem + m_ptShield;
			if (PtInRect(&rc, pt))
			{
				if (m_upShield) Disarm(m_upShield->GetType());
			}
		}
		break;
	default:
		return false;
	}

	return(true);
}

bool CUIEquipment::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (!m_bViewUI) return false;

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
		return OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		return OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	default:
		return false;
	}
	return true;
}
