#pragma once

class CDInRes;

class CWarp2DFramework 
{
public:
	CWarp2DFramework() = default;

	void RegisterIndRes(shared_ptr<CDInRes> indres){ m_pIndRes = indres; }

	// FrameWork 만들 때
	void OnCreate(HWND hWnd, HINSTANCE hInstance, shared_ptr<CDInRes> indres);

	// Scene 만들 때
	void BuildObject();;

	void FrameAdvance();;

	// Draw 2D 그래픽을 그릴때 사용
	// Render 3D 그래픽을 그릴때 사용
	void Draw();
	void Update(float fTimeElapsed);

private:
	// FrameWork에 필요한 값들
	HWND							m_hWnd				{ nullptr };
	HINSTANCE						m_hInstance			{ nullptr };

	
	shared_ptr<CDInRes>				m_pIndRes			{ nullptr };
	ComPtr<ID2D1HwndRenderTarget>	m_pd2dRendertarget	{ nullptr };

private:
	// Scene을 만들지 않을것이므로 Scene에 필요한 값들을 여기에
	ComPtr<ID2D1SolidColorBrush>	m_pd2dBrush			{ nullptr };
	ComPtr<IDWriteTextFormat>		m_pdwTextFormat		{ nullptr };
	ComPtr<IDWriteTextLayout>		m_pdwTextLayout		{ nullptr };

	ComPtr<ID2D1Bitmap1>			m_pd2dbmpTest;
	float								curImage=0.f;

	float fPositionX = 100.f;
};