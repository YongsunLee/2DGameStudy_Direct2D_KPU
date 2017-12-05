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
	// high_resolution_clock 고품질 시계 (now, duration 같은 것들 포함)
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



	// 우선순위 큐
	// time_point :: STL의 std::Chrono::high_resolution_clock::time_point;
	// time_point 그냥 시간이다 (type 형식)
	// 몬스터가 주고나서 몇초 뒤에 대한 기록을 하기 위함
	// 죽은시간에 5초를 더해서 우선순위 큐에다가 넣는다. (넣을 때 미래의 시간을 넣는 것!)
	// 계속 층층히 겹친다.
	// 맨 앞에 데이터가 과거가 되는 시점 (쿨타임이 다 돌았다는 경우)에 몬스터를 살린다.
	// 우선순위 큐 자체가 비용이 크다. 여러개 만들지 말자.
	priority_queue<time_point>		m_dqRegenTimer;
};
