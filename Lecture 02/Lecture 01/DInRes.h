#pragma once
// ��� ������ �� ����ϱ� ������
// �̱������� ����ų�
// ����� �����͸� ����ϴ� ���� �ٶ���
class CDInRes
{
public:
	CDInRes();
	~CDInRes();

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

	ID2D1Factory3*		d2dfactory()	const { return m_pd2dFactory.Get(); };
	IDWriteFactory3*	dwfactory()		const { return m_pdwFactory.Get(); };
	IWICImagingFactory* wicFactory()	const { return m_wicFacory.Get(); };


private:
	ComPtr<ID2D1Factory3>		m_pd2dFactory;
	ComPtr<IDWriteFactory3>		m_pdwFactory;
	ComPtr<IWICImagingFactory>	m_wicFacory;
};

