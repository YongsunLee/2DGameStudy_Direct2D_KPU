#include "stdafx.h"
#include "Framework/IndRes/IndRes.h"
#include "Monster.h"

CMonster::CMonster(D2D_SIZE_U sz)
	: CObject(GetPositionByCoord(sz), g_rcItemRect)
	, m_szCoord{ sz }
	, m_UserInfo{ UserInfo::GetInfoFromLevel(1) }
{
}

CMonster::~CMonster()
{
}
void CMonster::Update(float fTimeElapsed)
{
	m_fTick += (fTimeElapsed * 5.f);
	if (m_fTick > 4.f)
		m_fTick -= 4.f;

	m_fActionTick += fTimeElapsed;
	if (m_fActionTick > 1.f)
	{
		m_bAction = true;
		m_fActionTick = 0.f;
	}
}

void CMonster::Draw(ID2D1HwndRenderTarget * RenderTarget)
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
}

void CMonster::DrawUI(ID2D1HwndRenderTarget * RenderTarget)
{
	auto rcPlayer = m_rcSize + m_ptPoisition;
	auto rcUI = rcPlayer;
	rcUI.top -= g_fTileHeight * 0.25f;
	rcUI.bottom = rcPlayer.top;
	ComPtr<ID2D1SolidColorBrush> brush;
	RenderTarget->CreateSolidColorBrush(ColorF{ ColorF::Red }, &brush);

	float hpRatio = m_UserInfo.HP / m_UserInfo.maxHP;
	auto rcHP = rcUI;
	rcHP.right = Interpolation(rcHP.left, rcHP.right, hpRatio);
	RenderTarget->FillRectangle(rcHP, brush.Get());

	brush->SetColor(ColorF{ ColorF::DarkRed });
	RenderTarget->DrawRectangle(rcUI, brush.Get());
}

void CMonster::RegisterImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename, D2D_SIZE_U szSprite)
{
	LoadImageFromFile(
		indres->wicFactory()
		, RenderTarget
		, filename.c_str()
		, &m_bmpImage
	);
	m_szSprite = szSprite;
}

void CMonster::RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp, D2D_SIZE_U szSprite)
{
	m_bmpImage = bmp;
	m_szSprite = szSprite;
}

void CMonster::RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp, D2D_SIZE_U szSprite) noexcept
{
	m_bmpImage = move(bmp);
	m_szSprite = szSprite;
}

void CMonster::Move(Dir dir)
{
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
