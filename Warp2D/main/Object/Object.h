#pragma once

// �߻�Ŭ���� (���� Ŭ����)
// ��� Ŭ�������� �� �� �ִ� �͵鸸 �����Ѵ�.
class CObject
{
public:
	CObject(D2D_POINT_2F pt = Point2F(), 
			D2D_RECT_F rc = RectF()
	);
	virtual ~CObject();

	virtual void Update(float fTimeElapsed) {};
	virtual void Draw(ID2D1HwndRenderTarget *RenderTarget) = 0;		// ���� �����Լ� ��� ������Ʈ�� Draw�� ������ �ϹǷ�

	void SetPosition(D2D_POINT_2F pt) { m_ptPoisition = pt; };
	D2D_POINT_2F GetPosition() const { return m_ptPoisition; };

	void Move(D2D_POINT_2F pt) { m_ptPoisition = m_ptPoisition + pt; };

	void SetSize(D2D_RECT_F rc) { m_rcSize = rc; };
	D2D_RECT_F GetSize() const { return m_rcSize; };

protected:
	D2D_POINT_2F m_ptPoisition;		// ��ġ
	D2D_RECT_F m_rcSize;			// ũ��
	
};