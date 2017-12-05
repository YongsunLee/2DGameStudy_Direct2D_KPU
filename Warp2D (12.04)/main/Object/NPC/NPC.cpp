#include "stdafx.h"
#include "Framework/IndRes/IndRes.h"
#include "Object/Item/Item.h"
#include "NPC.h"

CNPC::CNPC(D2D_SIZE_U sz)
	: CObject(GetPositionByCoord(sz), g_rcItemRect)
	, m_szCoord{ sz }
	, m_UserInfo{ UserInfo::GetInfoFromLevel(1) }
{
}

CNPC::~CNPC()
{
}

void CNPC::Update(float fTimeElapsed)
{
	if (!IsActive())
	{
		m_fTick = 0.f;
		if ((m_fResurrectionTick += fTimeElapsed) >= g_fResurrectionTime)
		{
			m_UserInfo.HP = GetMaxHP();

			m_szCoord = m_szResurrectionPos;
			SetPosition(GetPositionByCoord(m_szCoord));
			m_Direction = Dir::bottom;

			m_fResurrectionTick = 0.f;
		}
		return;
	}

	m_fTick += (fTimeElapsed * 5.f);
	if (m_fTick > 4.f)
		m_fTick -= 4.f;
}

void CNPC::Draw(ID2D1HwndRenderTarget * RenderTarget)
{
	auto szSprite = m_bmpImage->GetSize();
	szSprite.width /= m_szSprite.width;
	szSprite.height /= m_szSprite.height;

	auto szSrc = SizeF(szSprite.width * static_cast<UINT>(m_fTick), szSprite.height * m_Direction);

	RenderTarget->DrawBitmap(
		m_bmpImage.Get()
		, m_rcSize + m_ptPoisition
		, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
		, RectF(szSrc.width, szSrc.height, szSrc.width + szSprite.width, szSrc.height + szSprite.height)
	);

	if (!IsActive())
	{
		RenderTarget->CreateSolidColorBrush(ColorF{ ColorF::Black, 0.6f }, &playerGrayBrush);

		// ����ϱ� ���ؼ��� AntialiasMode�� ������ �־�� �ȴ�.
		// �̷� �������
		// dxgi���� Anti�� �ϸ� ���ִ� ��� (d2d���� Anti�� ���� �ʴ� ��)
		RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

		RenderTarget->FillOpacityMask(
			m_bmpImage.Get()
			, playerGrayBrush.Get()
			, D2D1_OPACITY_MASK_CONTENT_GRAPHICS	// D2D Device Context ���ʹ� ���ܵȴ�.
			, m_rcSize + m_ptPoisition
			, RectF(szSrc.width, szSrc.height, szSrc.width + szSprite.width, szSrc.height + szSprite.height)
		);

		// �ϳ��ϳ� ��� Anti �ɾ��ִ� �� (Default)
		// Direct3D������ Per_Primitive �ʿ� x
		RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
}

void CNPC::DrawUI(ID2D1HwndRenderTarget * RenderTarget)
{
	auto rCNPC = m_rcSize + m_ptPoisition;
	auto rcUI = rCNPC;
	rcUI.top -= g_fTileHeight * 0.25f;
	rcUI.bottom = rCNPC.top;
	ComPtr<ID2D1SolidColorBrush> brush;
	RenderTarget->CreateSolidColorBrush(ColorF{ ColorF::Red }, &brush);

	float hpRatio = m_UserInfo.HP / m_UserInfo.maxHP;
	auto rcHP = rcUI;
	rcHP.right = Interpolation(rcHP.left, rcHP.right, hpRatio);
	RenderTarget->FillRectangle(rcHP, brush.Get());

	brush->SetColor(ColorF{ ColorF::DarkRed });
	RenderTarget->DrawRectangle(rcUI, brush.Get());
}

void CNPC::RegisterImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename, D2D_SIZE_U szSprite)
{
	LoadImageFromFile(
		indres->wicFactory()
		, RenderTarget
		, filename.c_str()
		, &m_bmpImage
	);
	m_szSprite = szSprite;
}

void CNPC::RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp, D2D_SIZE_U szSprite)
{
	m_bmpImage = bmp;
	m_szSprite = szSprite;
}

void CNPC::RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp, D2D_SIZE_U szSprite) noexcept
{
	m_bmpImage = move(bmp);
	m_szSprite = szSprite;
}

void CNPC::Move(Dir dir)
{
	if (!IsActive()) return;

	switch (m_Direction = dir)
	{
	case Dir::left:			if (m_szCoord.width > 0) m_szCoord.width -= 1;
		break;
	case Dir::top:			if (m_szCoord.height > 0) m_szCoord.height -= 1;
		break;
	case Dir::right:		if (m_szCoord.width < 50 - 1) m_szCoord.width += 1;
		break;
	case Dir::bottom:		if (m_szCoord.height < 50 - 1) m_szCoord.height += 1;
		break;
	}

	SetPosition(GetPositionByCoord(m_szCoord));
}

void CNPC::Interaction(CScene* pScene)
{
	// UI ����
	
	
	
}
