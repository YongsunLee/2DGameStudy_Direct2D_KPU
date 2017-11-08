#pragma once

class CCamera
{
protected:
	D2D_POINT_2F					m_d2dptPosition = Point2F(0.f, 0.f);
	D2D_MATRIX_3X2_F				m_d2dmtxWorld = Matrix3x2F::Identity();
	D2D_MATRIX_3X2_F				m_d2dmtxView;

	D2D_POINT_2F					m_d2dptClientSize;
	bool							m_bControlLock = false;
	float							m_fScale = 1.f;

	D2D_POINT_2F					m_d2dptAnchor {0,0};

private:
	void SetMatrix();
	D2D1_POINT_2F GetCameraPosition(); 

public:
	CCamera();
	virtual ~CCamera();

	D2D1_POINT_2F GetAnchor() { return m_d2dptAnchor; }
	D2D1_POINT_2F GetPosition(){ return m_d2dptPosition; }
	virtual void Move(D2D1_POINT_2F& d2dptShift) { m_d2dptPosition = m_d2dptPosition + d2dptShift; }
	virtual void SetPosition(D2D1_POINT_2F& ptPosition) { m_d2dptPosition = ptPosition; }
	// (-1, -1) ¡Â ptAnchor ¡Â (+1, +1)
	virtual void SetAnchor(D2D1_POINT_2F& ptAnchor) { m_d2dptAnchor = ptAnchor; }

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void Scale(float Scale) { if (Scale > 0) m_fScale = Scale; }
	float GetScale(){ return m_fScale; }
	void AgjustScale(float Scale){ if (Scale > 0) m_fScale += Scale; }
	virtual void SetLookAt(D2D1_POINT_2F& d2dptLookAt) { }
	D2D_MATRIX_3X2_F GetCameraMatrix(){ SetMatrix(); return m_d2dmtxWorld; }
	D2D_MATRIX_3X2_F RegenerateViewMatrix();

	void SetClientSize(D2D_POINT_2F ptClientSize);
	bool CheckControlLock(){ return m_bControlLock; }

};
