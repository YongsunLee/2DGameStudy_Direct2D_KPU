#pragma once
#include "Object\Object.h"

class CIndRes;

// 2�� �߻� Ŭ����
// ����, ��, ��Ÿ, �Һ� ��� ���� ������ ��
class CItem : public CObject
{
public:
	CItem(D2D_POINT_2F pt = Point2F(),
		D2D_RECT_F rc = RectF()
	);
	virtual ~CItem();

	virtual void Update(float fTimeElapsed) override;
	virtual void Draw(ID2D1HwndRenderTarget *RenderTarget) override;

	virtual void RegisterImage(
		CIndRes *indres,
		ID2D1HwndRenderTarget *RenderTarget, 
		path filename);

	virtual void RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp);
	virtual void RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept;


protected:
	ComPtr<ID2D1Bitmap1> m_bmpImage;
};