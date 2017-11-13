#include "stdafx.h"
#include "UIbase.h"


CUIbase::CUIbase()
{
}


CUIbase::~CUIbase()
{
}

void CUIbase::Draw(ID2D1HwndRenderTarget * renderTarget)
{
	// MergeRect :: rect 2개를 받아서 합치는 함수 by 도율 선배님
	// MergeRect(m_rcCaption, m_rcClient)

	// Caption
	renderTarget->SetTransform(Matrix3x2F::Translation(m_ptOrigin.x, m_ptOrigin.y));
	renderTarget->PushAxisAlignedClip(
		m_rcCaption, 
		D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	DrawCaption(renderTarget);

	renderTarget->PopAxisAlignedClip();

	// Client
	renderTarget->SetTransform(Matrix3x2F::Translation(m_ptOrigin.x, m_ptOrigin.y + m_rcCaption.bottom));
	renderTarget->PushAxisAlignedClip(
		m_rcClient, 
		D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	DrawClient(renderTarget);

	renderTarget->PopAxisAlignedClip();

}

void CUIbase::DrawClient(ID2D1HwndRenderTarget * renderTarget)
{
}

void CUIbase::DrawCaption(ID2D1HwndRenderTarget * renderTarget)
{
	// Brush와 폰트가 필요한데 추상 클래스에서는 구현하지 않는다.
}

void CUIbase::BuildObject(CScene * scene)
{
}
