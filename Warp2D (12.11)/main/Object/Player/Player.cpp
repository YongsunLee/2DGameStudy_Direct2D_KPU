#include "stdafx.h"
#include "Framework/IndRes/IndRes.h"
#include "Object/Item/Item.h"
#include "Player.h"

CPlayer::CPlayer(D2D_SIZE_U sz)
	: CObject(GetPositionByCoord(sz), g_rcItemRect)
	, m_szCoord{ sz }
	, m_UserInfo { UserInfo::GetInfoFromLevel(1) }
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(float fTimeElapsed)
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

	if (m_bMove)
	{
		m_bMove = false;
		m_fMoveCool = 0.f;
	}

	m_fAttCool += fTimeElapsed;
	m_fMoveCool += fTimeElapsed;

	if ((m_fTick += fTimeElapsed * 5.f) > 4.f)
		m_fTick -= 4.f;
}

void CPlayer::Draw(ID2D1HwndRenderTarget * RenderTarget)
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
		ComPtr<ID2D1SolidColorBrush> hbr;
		RenderTarget->CreateSolidColorBrush(ColorF{ ColorF::Black, 0.6f }, &hbr);
		RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

		RenderTarget->FillOpacityMask(
			  m_bmpImage.Get()
			, hbr.Get()
			, D2D1_OPACITY_MASK_CONTENT_GRAPHICS
			, m_rcSize + m_ptPoisition
			, RectF(szSrc.width, szSrc.height, szSrc.width + szSprite.width, szSrc.height + szSprite.height)
		
		);

		RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
}

void CPlayer::DrawUI(ID2D1HwndRenderTarget * RenderTarget)
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

void CPlayer::RegisterImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename, D2D_SIZE_U szSprite)
{
	LoadImageFromFile(
		  indres->wicFactory()
		, RenderTarget
		, filename.c_str()
		, &m_bmpImage
	);
	m_szSprite = szSprite;
}

void CPlayer::RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp, D2D_SIZE_U szSprite)
{
	m_bmpImage = bmp;
	m_szSprite = szSprite;
}

void CPlayer::RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp, D2D_SIZE_U szSprite) noexcept
{
	m_bmpImage = move(bmp);
	m_szSprite = szSprite;
}

void CPlayer::Move(Dir dir)
{
	if (!IsActive()) return;
	if (!IsMoveCool()) return;
	m_bMove = true;
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
