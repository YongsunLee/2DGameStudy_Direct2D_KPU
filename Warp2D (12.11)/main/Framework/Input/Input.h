#pragma once

#include <functional>
#include <memory>
#include <map>

inline void emptyFunc() {}

/*
명령 클래스 매니저


*/
class CInputManager
{
public:
	struct Button
	{
		using void_fn = std::function<void()>;

		void_fn			up_event	{ emptyFunc }	;		// 눌렸을 때
		void_fn			down_event	{ emptyFunc }	;		// 땟을 때
//		void_fn			doing_event	{ emptyFunc }	;		// 누르는 도중?

		const string	tag							;		// 불변 (복사 생성 x, 이동도 안될 수 있음)

		BYTE			togleKey	{ 0 }			;		// 토글키 (다운시 1,  1이면 업함수를 한번 호출)

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

	constexpr static BYTE GetHighBit(BYTE key) { return key >> 7; }							// 눌렸는지 안눌렸는지 판단
	constexpr static BYTE GetLowBit(BYTE key) { return key - GetHighBit(key) << 7; }		// Shift 키 같은거 누를때 필요한 것
};

namespace InputManager {
	// 태그를 일일이 지정하기 귀찮으니까 make_button(input_x) 하면 input_x 가 태그가 된다.
	#define make_button(name) CInputManager::Button name { #name };	
}
