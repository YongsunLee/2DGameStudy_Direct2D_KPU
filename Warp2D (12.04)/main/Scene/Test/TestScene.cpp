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
		case VK_SPACE:
		{
			if (!m_Player.IsActive()) break;

			auto attCoord = m_Player.GetAttCoord();
			for (auto& p : m_lstMonster)
				if (p->IsActive() && p->GetCoord() == attCoord)
				{
					p->GetDamage(m_Player.GetAtt());

					// 리젠 설정
					if (!p->IsActive()) m_dqRegenTimer.push(chrono::high_resolution_clock::now() + 5s);
				}

			break;
		}
		case 'A':		m_Player.Move(CPlayer::Dir::left);
			break;
		case 'W':		m_Player.Move(CPlayer::Dir::top);
			break;
		case 'D':		m_Player.Move(CPlayer::Dir::right);
			break;
		case 'S':		m_Player.Move(CPlayer::Dir::bottom);
			break;

		case 'Z':		m_Camera.Scale(m_Camera.GetScale() * 2.f);
			break;
		case 'X':		m_Camera.Scale(m_Camera.GetScale() * 0.5f);
			break;

//		case 'H':		m_uiInventory.PutItem();
//			break;

		case 'I':		m_uiInventory.SwitchView();
			break;
		case 'E':		m_uiEquipment.SwitchView();
			break;
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

void CTestScene::Update(float fTimeElapsed)
{
	m_Camera.SetPosition(m_Player.GetPosition());
	m_uiInventory.Update(fTimeElapsed);
	m_uiEquipment.Update(fTimeElapsed);

	if (m_dqRegenTimer.size() > 0)
	{
		// now를 받는 것이 비용이 크기 때문에 (정말 엄청 비싸진 않다. 최적화에 엄청 공을 들일 필요는 없다.)
		// queue는 하나만 사용하는 것이 좋다. (이거도 비용이 크다.)
		auto now = chrono::high_resolution_clock::now();
		// 현재 시간이 가장 위에있는 것보다 크면?
		// 즉, 우선 순위 큐에 있는 맨 위의 값이 과거가 되면
		// 활용) 구조체, pair 와 같은 것을 넘겨서 이벤트 타입, 이벤트 시간을 통해서 Time Line을 사용 할 수 있다.
		// ex) 몇초 뒤에 이펙트가 터져야 된다. 와 같은 상황
		// ex) 부활 카운터 (5초뒤에 이동가능 등)
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
	// Scene에서 Monster의 AI를 관리하고 있다.
	// 몬스터의 종류가 늘어나면 좋은 형태가 아니다.
	// AI Manger와 Phycies Manager를 두어야 한다.
	// AI Manager가 행동, 물리 계산을 Phycies 행동에서 처리 (서로 상호작용)

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
			// 랜덤 이동
			monster->Move(static_cast<CMonster::Dir>(rand() % (CMonster::Dir::top - CMonster::Dir::bottom) + CMonster::Dir::bottom));
		}
		else
		{
			// 범위 내부에서 이동
			// 몬스터의 방향 정해지는 곳
			CMonster::Dir dir;
			if (abs(ptDist.x) > abs(ptDist.y))
				dir = ptDist.x > 0 ? CMonster::Dir::right : CMonster::Dir::left;
			else
				dir = ptDist.y > 0 ? CMonster::Dir::bottom : CMonster::Dir::top;
				
			// 때릴 수 있으면 때린다.
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
