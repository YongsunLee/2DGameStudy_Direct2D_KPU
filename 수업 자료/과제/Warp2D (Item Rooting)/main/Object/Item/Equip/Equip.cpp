#include "stdafx.h"
#include "Framework\IndRes\IndRes.h"
#include "Equip.h"

CItemEquip::CItemEquip()
{
}

CItemEquip::CItemEquip(D2D_SIZE_U sz)
	: CItem(sz)
{

}

CItemEquip::~CItemEquip()
{
}

void CItemEquip::Update(float fTimeElapsed)
{
}

void CItemEquip::Draw(ID2D1HwndRenderTarget * RenderTarget)
{
	CItem::Draw(RenderTarget);
}

void CItemEquip::RegisterImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename)
{
	CItem::RegisterImage(indres, RenderTarget, filename);
}

void CItemEquip::RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp)
{
	m_bmpImage = bmp;
}

void CItemEquip::RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept
{
	m_bmpImage = move(bmp);
}
