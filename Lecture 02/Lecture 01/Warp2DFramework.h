#pragma once

class CDInRes;

class CWarp2DFramework 
{
public:
	CWarp2DFramework() = default;

	void RegisterIndRes(shared_ptr<CDInRes> indres){ m_pIndRes = indres; }

	// FrameWork ���� ��
	void OnCreate(HWND hWnd, HINSTANCE hInstance, shared_ptr<CDInRes> indres);

	// Scene ���� ��
	void BuildObject();;

	void FrameAdvance();;

	// Draw 2D �׷����� �׸��� ���
	// Render 3D �׷����� �׸��� ���
	void Draw();
	void Update(float fTimeElapsed);

private:
	// FrameWork�� �ʿ��� ����
	HWND							m_hWnd				{ nullptr };
	HINSTANCE						m_hInstance			{ nullptr };

	
	shared_ptr<CDInRes>				m_pIndRes			{ nullptr };
	ComPtr<ID2D1HwndRenderTarget>	m_pd2dRendertarget	{ nullptr };

private:
	// Scene�� ������ �������̹Ƿ� Scene�� �ʿ��� ������ ���⿡
	ComPtr<ID2D1SolidColorBrush>	m_pd2dBrush			{ nullptr };
	ComPtr<IDWriteTextFormat>		m_pdwTextFormat		{ nullptr };
	ComPtr<IDWriteTextLayout>		m_pdwTextLayout		{ nullptr };

	ComPtr<ID2D1Bitmap1>			m_pd2dbmpTest;
	float								curImage=0.f;

	float fPositionX = 100.f;
};