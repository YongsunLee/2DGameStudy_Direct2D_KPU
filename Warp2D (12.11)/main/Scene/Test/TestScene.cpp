#include "stdafx.h"

#include "Framework/Warp2DFramework.h"
#include "Framework/IndRes/IndRes.h"

#include "Object/Item/Item.h"

#include "TestScene.h"


CTestScene::CTestScene()
	: m_Player { SizeU(25, 25) }
	, m_uiInventory { m_Player }
	, m_uiEquipment { m_Player }
{
}


CTestScene::~CTestScene()
{
	m_InputManaget.SaveAllKey("TestScene", "./config.ini");
}

bool CTestScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
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
		break;
	default:
		return false;
	}

	return(true);
}

bool CTestScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_uiInventory.OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam)) return true;
	if (m_uiEquipment.OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam)) return true;

	switch (nMessageID)
	{
	case WM_SIZE:
		{
			auto rcClient = m_pFramework->GetClientSize();
			m_Camera.SetClientSize(Point2F(rcClient.right, rcClient.bottom));
		}
		break;

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

bool CTestScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{

		default:
			return false;
		}
		break;
	default:
		return false;
	}
	return(true);
}

bool CTestScene::OnCreate(wstring && tag, CWarp2DFramework * pFramework)
{
	if (!Base::OnCreate(std::move(tag), pFramework)) return false;

	auto rcClient = m_pFramework->GetClientSize();
	m_Camera.SetClientSize(Point2F(rcClient.right, rcClient.bottom));

	auto rendertarget = m_pFramework->GetRenderTarget();

	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::Crimson }, &m_pd2dsbrDefault);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::MistyRose }, &m_pd2dsbrTileA);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::LightPink }, &m_pd2dsbrTileB);

	m_Camera.SetPosition(m_Player.GetPosition());
	m_Camera.SetAnchor(Point2F(0.f, 0.f));

	m_Player.RegisterImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/player.png", SizeU(4, 4));

	for (int i = 0; i < 30; ++i)
		make_monster();

	static uniform_int_distribution<> pos_random{ 0,49 };
	static uniform_int_distribution<> img_random{ 0,5 };
	static default_random_engine reng(random_device{}());

	for (int i = 0; i < 30; ++i)
	{
	retryItem:
		auto coord = SizeU(pos_random(reng), pos_random(reng));
		for (const auto& p : m_lstItem) if (p->IsCollision(coord)) goto retryItem;

		unique_ptr<CItem> item;
		auto img = img_random(reng);

		if (img < 1)
		{
			item = make_unique<CItem>(coord);
			path imgPath;
			switch (img)
			{
			case 0:	imgPath = "Graphics/Icon/string.png";
				break;	
			case 1:	imgPath = "Graphics/Icon/Healing Potion.png";
				break;
			case 2:	imgPath = "Graphics/Icon/Mana Potion.png";
				break;
			}
			item->RegisterImage(m_pIndRes.get(), rendertarget.Get(), imgPath);
		}
		else if (img < 3)
		{
			auto type = static_cast<CConsumeItem::TYPE>(img - 1);
			item = make_Consume_Item(coord, type, m_pIndRes.get(), rendertarget.Get());
		}
		else
		{
			auto type = static_cast<CEquipmentItem::TYPE>(img - 3 + 1);
			if (type == 3) type = CEquipmentItem::TYPE::shield_0;
			item = make_Equip_Item(coord, type, m_pIndRes.get(), rendertarget.Get());
		}

		m_lstItem.push_back(move(item));
	}

	for (int i = 0; i < 10; ++i)
	{
	retryTrap:
		auto item = make_unique<CItem>(SizeU(pos_random(reng), pos_random(reng)));
		for (const auto& p : m_lstItem) if (p->IsCollision(item->GetCoord())) goto retryTrap;
		for (const auto& p : m_lstTrap) if (p->IsCollision(item->GetCoord())) goto retryTrap;

		item->RegisterImage(m_pIndRes.get(), rendertarget.Get(), 
			"Graphics/Icon/Wonder stone.png");
		m_lstTrap.push_back(move(item));
	}

	m_uiInventory.BuildObject(this);
	m_uiEquipment.BuildObject(this);

	return true;
}

void CTestScene::BindKey()
{
	using BTN = CInputManager::Button;
	using namespace InputManager;

	make_button(move_left);
	move_left.down_event	= [&]() { m_Player.Move(CPlayer::Dir::left); };
	make_button(move_top);
	move_top.down_event		= [&]() { m_Player.Move(CPlayer::Dir::top); };
	make_button(move_right);
	move_right.down_event	= [&]() { m_Player.Move(CPlayer::Dir::right); };
	make_button(move_bottom);
	move_bottom.down_event	= [&]() { m_Player.Move(CPlayer::Dir::bottom); };
	
	make_button(zoom_in);
	zoom_in.up_event	= [&]() { m_Camera.Scale(m_Camera.GetScale() * 2.f); };
	make_button(zoom_out);
	zoom_out.up_event	= [&]() { m_Camera.Scale(m_Camera.GetScale() * 0.5f); };

	make_button(show_inventory);
	show_inventory.up_event	= [&]() { m_uiInventory.SwitchView(); };
	make_button(show_equipment);
	show_equipment.up_event	= [&]() { m_uiEquipment.SwitchView(); };

	make_button(attack);
	attack.down_event = [&]()
	{
		if (!m_Player.IsAttackCool()) return;		// 쿨타임
		if (!m_Player.IsActive()) return;			// 공격 했다.

		auto attCoord = m_Player.GetAttCoord();
		for (auto& p : m_lstMonster)
			if (p->IsActive() && p->GetCoord() == attCoord)
			{
				p->GetDamage(m_Player.GetAtt());
				m_Player.ActionAttack();
				// 리젠 설정
				if (!p->IsActive()) m_dqRegenTimer.push(chrono::high_resolution_clock::now() + 5s);
			}
	};
	;
	m_InputManaget.bind(GetPrivateProfileIntA("TestScene", "move_left"		, 'A', "./config.ini"), move(move_left));
	m_InputManaget.bind(GetPrivateProfileIntA("TestScene", "move_top"		, 'W', "./config.ini"), move(move_top));
	m_InputManaget.bind(GetPrivateProfileIntA("TestScene", "move_right"		, 'D', "./config.ini"), move(move_right));
	m_InputManaget.bind(GetPrivateProfileIntA("TestScene", "move_bottom"	, 'S', "./config.ini"), move(move_bottom));

	m_InputManaget.bind(GetPrivateProfileIntA("TestScene", "zoom_in"		, 'Z', "./config.ini"), move(zoom_in));
	m_InputManaget.bind(GetPrivateProfileIntA("TestScene", "zoom_out"		, 'X', "./config.ini"), move(zoom_out));

	m_InputManaget.bind(GetPrivateProfileIntA("TestScene", "show_inventory"	, 'I', "./config.ini"), move(show_inventory));
	m_InputManaget.bind(GetPrivateProfileIntA("TestScene", "show_equipment"	, 'E', "./config.ini"), move(show_equipment));

	m_InputManaget.bind(GetPrivateProfileIntA("TestScene", "attack"			, VK_SPACE, "./config.ini"), move(attack));

//	m_InputManaget.replace("show_equipment"s, 'E');
}

void CTestScene::Update(float fTimeElapsed)
{
	m_Camera.SetPosition(m_Player.GetPosition());
	m_uiInventory.Update(fTimeElapsed);
	m_uiEquipment.Update(fTimeElapsed);

	if (m_dqRegenTimer.size() > 0)
	{
		auto now = chrono::high_resolution_clock::now();
		if (now > m_dqRegenTimer.top())
		{
			m_dqRegenTimer.pop();
			make_monster();
		}
	}

	m_Player.Update(fTimeElapsed);
	
	for (auto p = begin(m_lstMonster); p != end(m_lstMonster); )
	{
		auto& monster = *p;
		if (!monster->IsActive()) { p = m_lstMonster.erase(p); continue; }
		monster->Update(fTimeElapsed);
		MonsterAI(monster);
		++p;
	}

	for (auto p = begin(m_lstItem); p != end(m_lstItem); ++p)
		if ((*p)->IsCollision(m_Player.GetCoord()))
		{
			m_uiInventory.GetItem(move(*p));
			m_lstItem.erase(p);
			break;
		}

	for (auto p = begin(m_lstTrap); p != end(m_lstTrap); ++p)
	if ((*p)->IsCollision(m_Player.GetCoord()))
		{
			m_Player.GetDamage(rand() % 10 + 10);
			m_lstTrap.erase(p);
			break;
		}	
}

void CTestScene::MonsterAI(std::unique_ptr<CMonster> & monster)
{
	if (monster->Action())
	{
		auto ptDist = Point2F(
			  static_cast<FLOAT>(m_Player.GetCoord().width) - static_cast<FLOAT>(monster->GetCoord().width)
			, static_cast<FLOAT>(m_Player.GetCoord().height) - static_cast<FLOAT>(monster->GetCoord().height)
		);
		float fRoughDist = ptDist.x * ptDist.x + ptDist.y * ptDist.y;

		constexpr float fPlayerRecognitionArea = 5.f;

		if (fRoughDist > fPlayerRecognitionArea * fPlayerRecognitionArea)
		{
			monster->Move(static_cast<CMonster::Dir>(rand() % (CMonster::Dir::top - CMonster::Dir::bottom) + CMonster::Dir::bottom));
		}
		else
		{
			CMonster::Dir dir;
			if (abs(ptDist.x) > abs(ptDist.y))
				dir = ptDist.x > 0 ? CMonster::Dir::right : CMonster::Dir::left;
			else
				dir = ptDist.y > 0 ? CMonster::Dir::bottom : CMonster::Dir::top;
				

			if (abs(ptDist.x) + abs(ptDist.y) == 1)
			{
				monster->Look(dir);
				auto attCoord = monster->GetAttCoord();
				if (m_Player.GetCoord() == attCoord)
				{
					m_Player.GetDamage(monster->GetAtt());
				}
			}
			else monster->Move(dir);

		}
	}
}

void CTestScene::Draw(ID2D1HwndRenderTarget * pd2dRenderTarget)
{
	auto cameramtx = m_Camera.RegenerateViewMatrix();
	pd2dRenderTarget->SetTransform(cameramtx);
	
	for (int x = 0; x < 50; ++x)
		for (int y = 0; y < 50; ++y)
			pd2dRenderTarget->FillRectangle(
				  GetPositionByCoord(SizeU(x,y)) + g_rcItemRect
				, (x + y) % 2 ? m_pd2dsbrTileA.Get() : m_pd2dsbrTileB.Get());

	for (const auto& p : m_lstItem)
		p->Draw(pd2dRenderTarget);
	for (const auto& p : m_lstTrap)
		p->Draw(pd2dRenderTarget);

	m_Player.Draw(pd2dRenderTarget);

	for (const auto& p : m_lstMonster)
		p->Draw(pd2dRenderTarget);

	// HUD
	m_Player.DrawUI(pd2dRenderTarget);

	for (const auto& p : m_lstMonster)
		p->DrawUI(pd2dRenderTarget);

	// UI
	m_uiInventory.Draw(pd2dRenderTarget);
	m_uiEquipment.Draw(pd2dRenderTarget);
}

unique_ptr<CMonster>& CTestScene::make_monster()
{
	static uniform_int_distribution<> pos_random{ 0,49 };
	static default_random_engine reng(random_device{}());

retry:
	auto coord = SizeU(pos_random(reng), pos_random(reng));

	if (m_Player.GetCoord() == coord) goto retry;
	for (const auto& p : m_lstMonster) if (p->GetCoord() == coord) goto retry;

	auto monster = make_unique<CMonster>(coord);
	monster->RegisterImage(m_pIndRes.get(), m_pFramework->GetRenderTarget().Get(), "Graphics/goblin.png", SizeU(4, 4));
	m_lstMonster.push_back(move(monster));
	return m_lstMonster.back();
}
