#pragma once

class CScene;

class CUIbase
{
public:
	CUIbase();
	virtual ~CUIbase();

	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void Draw(ID2D1HwndRenderTarget *renderTarget);

	virtual void DrawClient(ID2D1HwndRenderTarget *renderTarget);
	virtual void DrawCaption(ID2D1HwndRenderTarget *renderTarget);

	// �� UI�� ����ִ� scene�� ����
	// scene���� indres�� ��� �ֱ� �빮�� indres ������
	virtual void BuildObject(CScene *scene);

	virtual void Update(float fTimeElapsed) {};

protected:
	bool			m_bViewUI		{true};
	bool			m_bViewCaption	{true};
	bool			m_bViewClient	{true};

	D2D_POINT_2F	m_ptOrigin;

	D2D_RECT_F		m_rcClient;		// UIâ ����
	D2D_RECT_F		m_rcCaption;	// TitleBar
	
	wstring			m_strTitle;
};

