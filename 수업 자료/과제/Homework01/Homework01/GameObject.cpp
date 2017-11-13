#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
{
	m_nPosition = {0.f,0.f,0.f};
	m_nColor = {0.f,0.f,0.f,0.f};
	m_nSpeed = 0.f;
	m_nSize = 0.f;
}

CGameObject::~CGameObject()
{
}

void CGameObject::Render(ComPtr<ID2D1HwndRenderTarget> renderTarget)
{
	ComPtr<ID2D1SolidColorBrush> brush;
	renderTarget->CreateSolidColorBrush(ColorF{ ColorF::MintCream }, &brush);

	renderTarget->DrawRectangle(RectF(m_nPosition.x - m_nSize, 
									  m_nPosition.y - m_nSize, 
									  m_nPosition.x + m_nSize, 
									  m_nPosition.y + m_nSize), brush.Get());
}

void CGameObject::Update()
{
	Move(Vec3{1.f,0,0}, 1.f);
}

void CGameObject::Move(Vec3 v3Dir, float fSpeed)
{
	SetPosition(Vec3{ m_nPosition.x + v3Dir.x * fSpeed, m_nPosition.y + v3Dir.y *fSpeed, m_nPosition.z + v3Dir.z * fSpeed });
}

void CGameObject::Rotate(Vec3 v3Axis, float fAngle)
{
}
