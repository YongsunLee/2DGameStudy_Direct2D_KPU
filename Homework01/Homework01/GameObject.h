#pragma once

class CGameObject
{
private:
	Vec3 m_nPosition;	// 위치
	Color4 m_nColor;	// 색상
	float m_nSpeed;		// 속도
	float m_nSize;		// 크기

	//float m_nMass;		// 질량 (보류)

public:
	CGameObject();
	~CGameObject();

	//생성 및 업데이트
	void Render(ComPtr<ID2D1HwndRenderTarget> renderTarget);
	void Update();

	// 이동 및 회전
	void Move(Vec3 v3Dir, float fSpeed);
	void Rotate(Vec3 v3Axis, float fAngle);

	// 위치, 색상, 속도, 크기
	void SetPosition(Vec3 v3Pos) { m_nPosition = v3Pos; };
	Vec3 GetPosition() { return m_nPosition; }

	void SetColor(Color4 c4Color) { m_nColor = c4Color; };
	Color4 GetColor() { return m_nColor; };

	void SetSpeed(float fSpeed) { m_nSpeed = fSpeed; };
	float GetSpeedd() { return m_nSpeed; };
	
	void SetSize(float fSize) { m_nSize = fSize; };
	float GetSize() { return m_nSize; };
};

