#include "stdafx.h"
#include "Framework/IndRes/IndRes.h"
#include "Item.h"

CItem::CItem(D2D_SIZE_U sz)
	: CObject(Point2F(sz.width * g_fTileWidth, sz.height * g_fTileHeight), g_rcItemRect)
	, m_szCoord { sz }
{
}

CItem::~CItem()
{
}

void CItem::Update(float fTimeElapsed)
{

}

void CItem::Draw(ID2D1HwndRenderTarget * RenderTarget)
{
	RenderTarget->DrawBitmap(
		  m_bmpImage.Get()
		, m_rcSize + m_ptPoisition
	);
}

void CItem::RegisterImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename)
{
	LoadImageFromFile(
		  indres->wicFactory()
		, RenderTarget
		, filename.c_str()
		, &m_bmpImage
	);

}

void CItem::RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp)
{
	m_bmpImage = bmp;
}

void CItem::RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept
{
	m_bmpImage = move(bmp);
}



CEquipmentItem::~CEquipmentItem()
{
}

unique_ptr<CEquipmentItem> make_Equip_Item(D2D_SIZE_U coord, CEquipmentItem::TYPE type, CIndRes* indres, ID2D1HwndRenderTarget* rendertarget)
{
	using EType = CEquipmentItem::TYPE;
	using BT = Buff::TYPE;

	unique_ptr<CEquipmentItem> retval;

	switch (type)
	{
	case EType::sword_0:
		retval = unique_ptr<CEquipmentItem>{ new CEquipmentItem{ coord, type, BUFF(BT::ATT, 10) } };
		retval->RegisterImage(indres, rendertarget, "Graphics/Icon/Bastard Sword.png");
		break;
	case EType::sword_1:
		retval = unique_ptr<CEquipmentItem>{ new CEquipmentItem{ coord, type, BUFF(BT::ATT, 20), BUFF(BT::HP, 5) } };
		retval->RegisterImage(indres, rendertarget, "Graphics/Icon/Gramr.png");
		break;
	case EType::shield_0:
		retval = unique_ptr<CEquipmentItem>{ new CEquipmentItem{ coord, type, BUFF(BT::DEF, 10), BUFF(BT::HP, 10) } };
		retval->RegisterImage(indres, rendertarget, "Graphics/Icon/Round Shield.png");
		break;
	}
	return retval;
}
