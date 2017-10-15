#pragma once

class CGameObject;

class CGameFramework
{
private:
	ComPtr<ID2D1Factory3>		m_pd2dFactory;
	ComPtr<IDWriteFactory3>		m_pdwFactory;
	//ComPtr<IWICImagingFactory>	m_wicFacory;
	
	ComPtr<ID2D1HwndRenderTarget> m_prenderTarget;
	CGameObject *m_pGameObject;

	POINT m_ptOldCursorPos;

public:
	CGameFramework();
	~CGameFramework();

	bool CreateHwndRenderTarget(HWND hWnd, ID2D1HwndRenderTarget** rendertarget, UINT width, UINT heigth)
	{
		D2D1_RENDER_TARGET_PROPERTIES props{};

		props.minLevel = D2D1_FEATURE_LEVEL_9;

		return SUCCEEDED(m_pd2dFactory->CreateHwndRenderTarget(
			props,
			HwndRenderTargetProperties(hWnd, SizeU(width, heigth)),
			rendertarget
		));
	}

	bool OnCreate(HWND hWnd);

	void BuildObjects();
	void ReleaseObjects();

	void Render();

	void ProcessInput();
	void FrameAdvance();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

};

