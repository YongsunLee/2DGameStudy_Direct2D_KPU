#pragma once

#include "UI/UIBase.h"
#include "Object/Player/Player.h"

class CItem;

class CUIInventory
	: public CUIBase
{
public:
	CUIInventory(CPlayer& player);
	~CUIInventory();

	void DrawClient(ID2D1HwndRenderTarget* pd2dRenderTarget) override;
	void DrawCaption(ID2D1HwndRenderTarget* pd2dRenderTarget) override;
	
	void BuildObject(CScene* scene) override;
	
	void Update(float fTimeElapsed) override;
	void GetItem(unique_ptr<CItem>&& item);
	void PutItem() { if (m_reflstItem.size() > 0) m_reflstItem.pop_back(); }
	
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)		override;
	bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;

private:
	CPlayer&							m_refPlayer;
	list<unique_ptr<CItem>>&			m_reflstItem;

	ComPtr<ID2D1SolidColorBrush>		m_hbrCaption;
	ComPtr<ID2D1SolidColorBrush>		m_hbrClient;
	ComPtr<ID2D1SolidColorBrush>		m_hbrText;

	ComPtr<ID2D1Bitmap1>				m_bmpFrame;

	D2D_SIZE_F							m_szItemOutlineMargin;
	D2D_SIZE_F							m_szItemBetweenMargin;
	D2D_SIZE_U							m_szItemMatrix;

	D2D_RECT_F							m_rcItem;

	ComPtr<IDWriteTextFormat>			m_dwTextFormat;
	ComPtr<IDWriteTextFormat>			m_dwItemEa;
};
