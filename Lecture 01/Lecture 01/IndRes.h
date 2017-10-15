#pragma once
class CIndRes
{
public:
	CIndRes();
	~CIndRes();

	bool Initialize();
	bool CreateHwndRenderTarget(HWND hWnd, ID2D1HwndRenderTarget** rendertarget, UINT width, UINT heigth)
	{
		D2D1_RENDER_TARGET_PROPERTIES props{};

		return SUCCEEDED(m_pd2dFactory->CreateHwndRenderTarget(
			props,
			HwndRenderTargetProperties(hWnd, SizeU(width, heigth)),
			rendertarget
		));
	}

private:
	ComPtr<ID2D1Factory3>		m_pd2dFactory;
	ComPtr<IDWriteFactory3>		m_pdwFactory;


	//ComPtr<IWICImagingFactory>	m_wicFacory;
};

