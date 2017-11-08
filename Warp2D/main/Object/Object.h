#pragma once

// 추상클래스 (맨위 클래스)
// 모든 클래스에서 쓸 수 있는 것들만 포함한다.
class CObject
{
public:
	CObject(D2D_POINT_2F pt = Point2F(), 
			D2D_RECT_F rc = RectF()
	);
	virtual ~CObject();

	virtual void Update(float fTimeElapsed) {};
	virtual void Draw(ID2D1HwndRenderTarget *RenderTarget) = 0;		// 순수 가상함수 모든 오브젝트는 Draw를 가져야 하므로

	void SetPosition(D2D_POINT_2F pt) { m_ptPoisition = pt; };
	D2D_POINT_2F GetPosition() const { return m_ptPoisition; };

	void Move(D2D_POINT_2F pt) { m_ptPoisition = m_ptPoisition + pt; };

	void SetSize(D2D_RECT_F rc) { m_rcSize = rc; };
	D2D_RECT_F GetSize() const { return m_rcSize; };

protected:
	D2D_POINT_2F m_ptPoisition;		// 위치
	D2D_RECT_F m_rcSize;			// 크기
	
};