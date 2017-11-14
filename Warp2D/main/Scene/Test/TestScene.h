#pragma once

#include "Camera/Camera.h"
#include "Scene/Scene.h"
#include "UI\Inventory\Inventory.h"

class CItem;
class CUIInventory;

class CTestScene :
	public CScene
{
public:
	using Base = CScene;
public:
	CTestScene();
	~CTestScene();

	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)		override;
	bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;

	bool OnCreate(wstring&& tag, CWarp2DFramework* pFramework) override;
	void Update(float fTimeElapsed) override;
	void Draw(ID2D1HwndRenderTarget * pd2dRenderTarget) override;

private:

	ComPtr<ID2D1SolidColorBrush>	m_pd2dsbrDefault{ nullptr };
	ComPtr<ID2D1SolidColorBrush>	m_pd2dGridBrush{ nullptr };
	D2D_POINT_2F					m_ptPlayer{ 0, 0 };

	// Camera
	//D2D_MATRIX_3X2_F				m_mtxLocal			{ Matrix3x2F::Identity() };
	CCamera							m_Camera;

	// Item
	unique_ptr<CItem>				m_upItem;

	// Inventory
	CUIInventory					m_uiInventory;

	// Player
	// CPlayer						m_Player;

};

