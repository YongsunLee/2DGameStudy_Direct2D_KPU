#include "stdafx.h"
#include "Warp2DFramework.h"
#include "DInRes.h"

void CWarp2DFramework::OnCreate(HWND hWnd, HINSTANCE hInstance, shared_ptr<CDInRes> indres) {
	RegisterIndRes(indres);
	m_hWnd = hWnd;
	m_hInstance = hInstance;

	m_pIndRes->CreateHwndRenderTarget(m_hWnd, &m_pd2dRendertarget, 800, 600);

	// renderTarget�� ������� ���Ŀ� �ҷ��� �Ѵ�.
	BuildObject();
}

// Scene ���� ��

void CWarp2DFramework::BuildObject() {
	m_pd2dRendertarget->CreateSolidColorBrush(ColorF{ ColorF::Crimson }, &m_pd2dBrush);
	
	// �������

	// �����ִ� �͵��� ����� ��
	// ��Ʈ ������� ����
	// ���ڸ� ���°��� �ſ� ū ����� �̿��ϱ� ������
	// ������ ����ϴ� ���� ����.
	auto dwFactory = m_pIndRes->dwfactory();
	
	dwFactory->CreateTextFormat(L"Arial",
								nullptr, 
								DWRITE_FONT_WEIGHT_BOLD,
								DWRITE_FONT_STYLE_ITALIC,
								DWRITE_FONT_STRETCH_NORMAL,
								30.f,
								L"Ko-kr",
								&m_pdwTextFormat);
	// ���� �ʿ�
	// ��� ����, ��������, ������ ���� ���� �� (Text)
	// ���� ���Ʒ� ( Paragraph )
	m_pdwTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	m_pdwTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	// ��������� Text�� ����� �⺻ ���� �����ؼ� ����ȴ�.
	
	// TextLayout---------
	// �̰ŵ� ���ٲٴ� �κ�
	// ��� �Ƹ� �ٲ�����?
	// Hello, World!�� 13���� + 1(nullptr)
	dwFactory->CreateTextLayout(L"Hello, World!",
								13,
								m_pdwTextFormat.Get(),
								300,
								100,
								&m_pdwTextLayout);
	
	// �̰� Set�� �� �ִ°� �� ����.

	// Load Bitmap
	auto wicFactory = m_pIndRes->wicFactory();
	LoadImageFromFile(wicFactory,
					  m_pd2dRendertarget.Get(),
					  L"Chie_Stand_L.png",
					  &m_pd2dbmpTest
	);

}

void CWarp2DFramework::FrameAdvance() {
	Update(1.f / 60.f);		// ���� 1/60
	Draw();

	Sleep(16);				// 1/60
}

// Draw 2D �׷����� �׸��� ���
// Render 3D �׷����� �׸��� ���

void CWarp2DFramework::Draw() {
	// BeginDraw Void ����
	m_pd2dRendertarget->BeginDraw();
	m_pd2dRendertarget->Clear(ColorF(ColorF::LightGray));

	m_pd2dRendertarget->DrawRectangle(RectF(-100 + fPositionX, 100, fPositionX, 200), m_pd2dBrush.Get());

	m_pd2dRendertarget->DrawTextLayout(Point2F(0, 0), m_pdwTextLayout.Get(), m_pd2dBrush.Get());
	
	// ���̷�Ʈ�� Window API ������ ���ư���.
	// Windows API�� DrawText�� define �Ǿ��ִ�.
	m_pd2dRendertarget->DrawText(L"Text", 4, m_pdwTextFormat.Get(), RectF(400, 500, 600, 600), m_pd2dBrush.Get());

	// �׷��� �׸���!
	auto bmpSize = m_pd2dbmpTest->GetSize();
	m_pd2dRendertarget->DrawBitmap(	m_pd2dbmpTest.Get(),
									RectF(100, 100, 100 + bmpSize.width * 0.1f, 100 + bmpSize.height),
									1.f,
									D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
									RectF(bmpSize.width * 0.1f * (int)curImage, 0, 
										  bmpSize.width * 0.1f * ((int)curImage + 1), bmpSize.height)
	);

	// EndDraw HRESULT ����
	// � ������ ������ ��ȯ���� �ش�. 
	// �Ǻ��� �߰�
	m_pd2dRendertarget->EndDraw();
}

void CWarp2DFramework::Update(float fTimeElapsed) {
	fPositionX += (200.f * fTimeElapsed);

	if (fPositionX > 800.f) fPositionX -= 800.f;

	if ((curImage += fTimeElapsed * 10.f) >= 10.f)
		curImage -= 10.f;
}
