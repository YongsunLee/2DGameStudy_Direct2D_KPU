#pragma once

#include "Scene/Scene.h"
#include "Camera/Camera.h"

#include "Object/Player/Player.h"
#include "Object/Monster/Monster.h"

#include "UI/Inventory/Inventory.h"
#include "UI/Equipment/Equipment.h"



class CItem;

class CTestScene :
	public CScene
{
public:

	using Base = CScene;
	// high_resolution_clock ��ǰ�� �ð� (now, duration ���� �͵� ����)
	using time_point = chrono::high_resolution_clock::time_point;

public:
	CTestScene();
	~CTestScene();

	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)		override;
	bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;

	bool OnCreate(wstring&& tag, CWarp2DFramework* pFramework) override;
	void Update(float fTimeElapsed) override;
	void MonsterAI(unique_ptr<CMonster> & monster);
	void Draw(ID2D1HwndRenderTarget * pd2dRenderTarget) override;

	unique_ptr<CMonster>& make_monster();

private:

	ComPtr<ID2D1SolidColorBrush>	m_pd2dsbrDefault	{ nullptr }	;
	ComPtr<ID2D1SolidColorBrush>	m_pd2dsbrTileA		{ nullptr }	;
	ComPtr<ID2D1SolidColorBrush>	m_pd2dsbrTileB		{ nullptr }	;

	CPlayer							m_Player;
	CCamera							m_Camera;

	list<unique_ptr<CItem>>			m_lstItem;
	list<unique_ptr<CItem>>			m_lstTrap;

	list<unique_ptr<CMonster>>		m_lstMonster;

	CUIInventory					m_uiInventory;
	CUIEquipment					m_uiEquipment;

	// UI List
	list<unique_ptr<CUIBase>>		m_uiList;



	// �켱���� ť
	// time_point :: STL�� std::Chrono::high_resolution_clock::time_point;
	// time_point �׳� �ð��̴� (type ����)
	// ���Ͱ� �ְ��� ���� �ڿ� ���� ����� �ϱ� ����
	// �����ð��� 5�ʸ� ���ؼ� �켱���� ť���ٰ� �ִ´�. (���� �� �̷��� �ð��� �ִ� ��!)
	// ��� ������ ��ģ��.
	// �� �տ� �����Ͱ� ���Ű� �Ǵ� ���� (��Ÿ���� �� ���Ҵٴ� ���)�� ���͸� �츰��.
	// �켱���� ť ��ü�� ����� ũ��. ������ ������ ����.
	priority_queue<time_point>		m_dqRegenTimer;
};
