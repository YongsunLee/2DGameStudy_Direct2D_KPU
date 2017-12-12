#pragma once

#include <functional>
#include <memory>
#include <map>

inline void emptyFunc() {}

/*
��� Ŭ���� �Ŵ���


*/
class CInputManager
{
public:
	struct Button
	{
		using void_fn = std::function<void()>;

		void_fn			up_event	{ emptyFunc }	;		// ������ ��
		void_fn			down_event	{ emptyFunc }	;		// ���� ��
//		void_fn			doing_event	{ emptyFunc }	;		// ������ ����?

		const string	tag							;		// �Һ� (���� ���� x, �̵��� �ȵ� �� ����)

		BYTE			togleKey	{ 0 }			;		// ���Ű (�ٿ�� 1,  1�̸� ���Լ��� �ѹ� ȣ��)

		Button() = delete;
		Button(string&& tag) noexcept : tag{ std::move(tag) } {}
		Button(string&& tag, void_fn&& up, void_fn&& down) noexcept
			: tag{ std::move(tag) }, up_event{ std::move(up) }, down_event{ std::move(down) } {}
	};

	void ProcessInput(bool (&arrByte)[256]);

	void bind(UCHAR key, CInputManager::Button&& btn);
	bool replace(std::string tag, UCHAR newkey);
	bool replace(std::string tag, UCHAR newkey, LPCSTR appName, LPCSTR filePath);

	void SaveAllKey(LPCSTR appName, LPCSTR filePath);
	void SaveKey(LPCSTR appName, std::string tag, LPCSTR filePath);

private:

	UCHAR m_pKeyBuffer[256];
	
	map<UCHAR, Button> m_mRegisterKeyList;

	constexpr static BYTE GetHighBit(BYTE key) { return key >> 7; }							// ���ȴ��� �ȴ��ȴ��� �Ǵ�
	constexpr static BYTE GetLowBit(BYTE key) { return key - GetHighBit(key) << 7; }		// Shift Ű ������ ������ �ʿ��� ��
};

namespace InputManager {
	// �±׸� ������ �����ϱ� �������ϱ� make_button(input_x) �ϸ� input_x �� �±װ� �ȴ�.
	#define make_button(name) CInputManager::Button name { #name };	
}
