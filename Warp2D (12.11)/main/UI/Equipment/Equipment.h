#pragma once

#include "UI/UIBase.h"
#include "Object/Player/Player.h"
#include "Object/Item/Item.h"



class CUIEquipment
	: public CUIBase
{
public:
	CUIEquipment(CPlayer& player);
	~CUIEquipment();

	void DrawClient(ID2D1HwndRenderTarget* pd2dRenderTarget) override;
	void DrawCaption(ID2D1HwndRenderTarget* pd2dRenderTarget) override;
	
	void BuildObject(CScene* scene) override;
	
	void Update(float fTimeElapsed) override;
	void ChangeEquipment();
	void Equipment(unique_ptr<CItem>&& item);
	void Disarm(CEquipmentItem::TYPE type);

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

	D2D_POINT_2F						m_ptWeapon;
	D2D_POINT_2F						m_ptShield;

	D2D_RECT_F							m_rcItem;

	unique_ptr<CEquipmentItem>			m_upWeapon;
	unique_ptr<CEquipmentItem>			m_upShield;

	ComPtr<IDWriteTextFormat>			m_dwTextFormat;
};
