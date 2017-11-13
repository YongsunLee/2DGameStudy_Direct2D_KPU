#include "stdafx.h"
#include "Warp2DFramework.h"
#include "DInRes.h"

void CWarp2DFramework::OnCreate(HWND hWnd, HINSTANCE hInstance, shared_ptr<CDInRes> indres) {
	RegisterIndRes(indres);
	m_hWnd = hWnd;
	m_hInstance = hInstance;

	m_pIndRes->CreateHwndRenderTarget(m_hWnd, &m_pd2dRendertarget, 800, 600);

	// renderTarget이 만들어진 이후에 불러야 한다.
	BuildObject();
}

// Scene 만들 때

void CWarp2DFramework::BuildObject() {
	m_pd2dRendertarget->CreateSolidColorBrush(ColorF{ ColorF::Crimson }, &m_pd2dBrush);
	
	// 여기부터

	// 여기있는 것들은 만들면 끝
	// 폰트 사이즈는 고정
	// 글자를 적는것은 매우 큰 비용을 이용하기 때문에
	// 적당히 사용하는 것이 좋다.
	auto dwFactory = m_pIndRes->dwfactory();
	
	dwFactory->CreateTextFormat(L"Arial",
								nullptr, 
								DWRITE_FONT_WEIGHT_BOLD,
								DWRITE_FONT_STYLE_ITALIC,
								DWRITE_FONT_STRETCH_NORMAL,
								30.f,
								L"Ko-kr",
								&m_pdwTextFormat);
	// 설정 필요
	// 가운데 정렬, 왼쪽정렬, 오른쪽 정렬 같은 것 (Text)
	// 문맥 위아래 ( Paragraph )
	m_pdwTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	m_pdwTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	// 여기까지가 Text를 만드는 기본 형식 복사해서 쓰면된다.
	
	// TextLayout---------
	// 이거도 못바꾸는 부분
	// 몇개는 아마 바뀔지도?
	// Hello, World!는 13글자 + 1(nullptr)
	dwFactory->CreateTextLayout(L"Hello, World!",
								13,
								m_pdwTextFormat.Get(),
								300,
								100,
								&m_pdwTextLayout);
	
	// 이게 Set할 수 있는게 더 많다.

	// Load Bitmap
	auto wicFactory = m_pIndRes->wicFactory();
	LoadImageFromFile(wicFactory,
					  m_pd2dRendertarget.Get(),
					  L"Chie_Stand_L.png",
					  &m_pd2dbmpTest
	);

}

void CWarp2DFramework::FrameAdvance() {
	Update(1.f / 60.f);		// 강제 1/60
	Draw();

	Sleep(16);				// 1/60
}

// Draw 2D 그래픽을 그릴때 사용
// Render 3D 그래픽을 그릴때 사용

void CWarp2DFramework::Draw() {
	// BeginDraw Void 형태
	m_pd2dRendertarget->BeginDraw();
	m_pd2dRendertarget->Clear(ColorF(ColorF::LightGray));

	m_pd2dRendertarget->DrawRectangle(RectF(-100 + fPositionX, 100, fPositionX, 200), m_pd2dBrush.Get());

	m_pd2dRendertarget->DrawTextLayout(Point2F(0, 0), m_pdwTextLayout.Get(), m_pd2dBrush.Get());
	
	// 다이렉트는 Window API 위에서 돌아간다.
	// Windows API에 DrawText가 define 되어있다.
	m_pd2dRendertarget->DrawText(L"Text", 4, m_pdwTextFormat.Get(), RectF(400, 500, 600, 600), m_pd2dBrush.Get());

	// 그래픽 그리기!
	auto bmpSize = m_pd2dbmpTest->GetSize();
	m_pd2dRendertarget->DrawBitmap(	m_pd2dbmpTest.Get(),
									RectF(100, 100, 100 + bmpSize.width * 0.1f, 100 + bmpSize.height),
									1.f,
									D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
									RectF(bmpSize.width * 0.1f * (int)curImage, 0, 
										  bmpSize.width * 0.1f * ((int)curImage + 1), bmpSize.height)
	);

	// EndDraw HRESULT 형식
	// 어떤 문제가 있으면 반환값을 준다. 
	// 판별식 추가
	m_pd2dRendertarget->EndDraw();
}

void CWarp2DFramework::Update(float fTimeElapsed) {
	fPositionX += (200.f * fTimeElapsed);

	if (fPositionX > 800.f) fPositionX -= 800.f;

	if ((curImage += fTimeElapsed * 10.f) >= 10.f)
		curImage -= 10.f;
}
