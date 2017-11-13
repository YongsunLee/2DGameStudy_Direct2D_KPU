#pragma once

class CGameObject
{
private:
	Vec3 m_nPosition;	// ��ġ
	Color4 m_nColor;	// ����
	float m_nSpeed;		// �ӵ�
	float m_nSize;		// ũ��

	//float m_nMass;		// ���� (����)

public:
	CGameObject();
	~CGameObject();

	//���� �� ������Ʈ
	void Render(ComPtr<ID2D1HwndRenderTarget> renderTarget);
	void Update();

	// �̵� �� ȸ��
	void Move(Vec3 v3Dir, float fSpeed);
	void Rotate(Vec3 v3Axis, float fAngle);

	// ��ġ, ����, �ӵ�, ũ��
	void SetPosition(Vec3 v3Pos) { m_nPosition = v3Pos; };
	Vec3 GetPosition() { return m_nPosition; }

	void SetColor(Color4 c4Color) { m_nColor = c4Color; };
	Color4 GetColor() { return m_nColor; };

	void SetSpeed(float fSpeed) { m_nSpeed = fSpeed; };
	float GetSpeedd() { return m_nSpeed; };
	
	void SetSize(float fSize) { m_nSize = fSize; };
	float GetSize() { return m_nSize; };
};

