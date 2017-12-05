#include "stdafx.h"
#include "Framework/Warp2DFramework.h"
#include "Framework/IndRes/IndRes.h"
#include "Scene/Scene.h"
#include "Object/Item/Item.h"
#include "Inventory.h"

CUIInventory::CUIInventory(CPlayer & player)
	: m_refPlayer { player }
	, m_reflstItem { player.m_lstItem }
{
}

CUIInventory::~CUIInventory()
{
}

void CUIInventory::DrawClient(ID2D1HwndRenderTarget * pd2dRenderTarget)
{
	pd2dRenderTarget->FillRectangle(m_rcClient, m_hbrClient.Get());

	auto pItem = begin(m_reflstItem);

	for (int iColumn = 0; iColumn < static_cast<int>(m_szItemMatrix.height); ++iColumn)
		for (int iRow = 0; iRow < static_cast<int>(m_szItemMatrix.width); ++iRow)
		{
			float fWidth = (m_rcItem.right - m_rcItem.left) * iRow + m_szItemBetweenMargin.width * max(0, iRow) + m_szItemOutlineMargin.width;
			float fHeight = (m_rcItem.bottom - m_rcItem.top) * iColumn + m_szItemBetweenMargin.height * max(0, iColumn) + m_szItemOutlineMargin.height;

			auto rc = RectF(fWidth + m_rcItem.left, fHeight + m_rcItem.top, fWidth + m_rcItem.right, fHeight + m_rcItem.bottom);

			pd2dRenderTarget->DrawBitmap(m_bmpFrame.Get(), rc);

			if (pItem != end(m_reflstItem))
			{
				pd2dRenderTarget->DrawBitmap(
					(*pItem)->GetBitmap()
					, rc
				);

				if ((*pItem)->ItemType() == CItem::TYPE::Consume)
				{
					auto str = to_wstring(static_cast<CConsumeItem*>((*pItem).get())->CurrentEa());
					pd2dRenderTarget->DrawText(str.c_str(), static_cast<UINT>(str.length()), m_dwItemEa.Get(), rc, m_hbrText.Get());
				}

				++pItem;
			}

		}
}

void CUIInventory::DrawCaption(ID2D1HwndRenderTarget * pd2dRenderTarget)
{
	pd2dRenderTarget->FillRectangle(m_rcCaption, m_hbrCaption.Get());
	pd2dRenderTarget->DrawText(
		  m_strTitle.c_str()
		, static_cast<UINT>(m_strTitle.length())
		, m_dwTextFormat.Get()
		, m_rcCaption
		, m_hbrText.Get());
}

void CUIInventory::BuildObject(CScene * scene)
{
	auto framework = scene->Framework();
	auto indres = framework->GetIndRes();
	auto rendertarget = framework->GetRenderTarget();

	m_rcCaption = RectF(0, 0, 200, 20);
	m_rcClient = RectF(0, 0, 200, 220);

	m_ptOrigin = Point2F(560, 20);

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
	
	indres->dwFactory()->CreateTextFormat(
		  L"Arial"
		, nullptr
		, DWRITE_FONT_WEIGHT_BOLD
		, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_NORMAL
		, (m_rcItem.bottom - m_rcItem.top) * 0.4f
		, L"ko-kr"
		, &m_dwItemEa);

	m_dwItemEa->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	m_dwItemEa->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

	m_strTitle = L"Inventory"s;
}

void CUIInventory::Update(float fTimeElapsed)
{

}

void CUIInventory::GetItem(unique_ptr<CItem>&& item) 
{
	if (item->ItemType() != CItem::TYPE::Consume)
		return m_reflstItem.push_back(std::move(item)); 

	auto consume = static_cast<CConsumeItem*>(item.get());
	auto p = find_if(begin(m_reflstItem), end(m_reflstItem), [&](const unique_ptr<CItem>& p)
	{
		auto con = dynamic_cast<CConsumeItem*>(p.get());
		if (!con) return false;
		return con->GetType() == consume->GetType();
	});

	if (p != end(m_reflstItem)) return static_cast<CConsumeItem*>((*p).get())->AdjustEa(1);
	return m_reflstItem.push_back(std::move(item));
}

bool CUIInventory::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CUIInventory::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		{ 
			if (!m_refPlayer.IsActive()) false;

			auto pt = Point2F(LOWORD(lParam), HIWORD(lParam)) - (m_ptOrigin + Point2F(0, m_rcCaption.bottom));
			if (!PtInRect(&m_rcClient, pt)) return false;
			
			auto pItem = begin(m_reflstItem);

			for (int iColumn = 0; iColumn < static_cast<int>(m_szItemMatrix.height); ++iColumn)
				for (int iRow = 0; iRow < static_cast<int>(m_szItemMatrix.width); ++iRow)
				{
					if (pItem == end(m_reflstItem)) break;

					float fWidth = (m_rcItem.right - m_rcItem.left) * iRow + m_szItemBetweenMargin.width * max(0, iRow) + m_szItemOutlineMargin.width;
					float fHeight = (m_rcItem.bottom - m_rcItem.top) * iColumn + m_szItemBetweenMargin.height * max(0, iColumn) + m_szItemOutlineMargin.height;

					auto rc = RectF(fWidth + m_rcItem.left, fHeight + m_rcItem.top, fWidth + m_rcItem.right, fHeight + m_rcItem.bottom);

					if (PtInRect(&rc, pt))
					{
						switch ((*pItem)->ItemType())
						{
						case CItem::TYPE::Unknown:

							break;
						case CItem::TYPE::Equipment:
							static_cast<CEquipmentItem*>(pItem->get())->Equipment(true);
							break;

						case CItem::TYPE::Consume:
						{
							auto consume = static_cast<CConsumeItem*>(pItem->get());
							consume->Consume(m_refPlayer);

							if (consume->CurrentEa() > 0) break;
							
							pItem = m_reflstItem.erase(pItem);
							continue;
						}	
						default:
							break;
						}
					}

					++pItem;
				}
		}
		break;
	default:
		return false;
	}

	return(true);
}

bool CUIInventory::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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
