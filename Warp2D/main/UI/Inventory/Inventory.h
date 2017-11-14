#pragma once

#include "UI/UIbase.h"

class CItem;

class CUIInventory : public CUIbase
{
public:
	CUIInventory();
	~CUIInventory();
	
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void DrawClient(ID2D1HwndRenderTarget *renderTarget) override;
	virtual void DrawCaption(ID2D1HwndRenderTarget *renderTarget) override;

	// 이 UI를 들고있는 scene을 포함
	// scene에서 indres를 들고 있기 대문에 indres 미포함
	virtual void BuildObject(CScene *scene) override;

	virtual void Update(float fTimeElapsed) override;

	void GetItem(CItem *item) { m_pItem = item; };

private:

	ComPtr<ID2D1SolidColorBrush>	m_hbrushCaption;
	ComPtr<ID2D1SolidColorBrush>	m_hbrushClient;
	ComPtr<ID2D1SolidColorBrush>	m_hbrushText;

	ComPtr<ID2D1Bitmap1>			m_bmpFrame[2];
	D2D_RECT_F						m_rcItem;
	CItem*							m_pItem = nullptr;

	ComPtr<IDWriteTextFormat>		m_dwTextFormat;

};