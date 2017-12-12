#include "stdafx.h"
#include "Input.h"

void CInputManager::ProcessInput(bool(&arrByte)[256])
{
	/*
		GetKeyboardState
		BYTE 256 개를 넘기면 가상키가 눌렸는지 안눌렸는지에 대한 판단값을 넘겨줌
	*/
	bool retval = false;
	BOOL bKeyState = GetKeyboardState(m_pKeyBuffer);
	UNREFERENCED_PARAMETER(bKeyState);

	// 등록해 둔 키 리스트에 대해서 루프
	for (auto& data : m_mRegisterKeyList)
	{
		auto& key = data.first;
		auto& btn = data.second;
		
		// false일 경우에만 실행
		// false를 true로 변경후 실행
		if (arrByte[key]) continue;
		arrByte[key] = true;

		// 눌렸는지 판단하는 함수 (1 == 눌렸다. 0 == 안눌렸다.)
		if (1 == GetHighBit(m_pKeyBuffer[key]))
		{
			retval = true;
			btn.togleKey = 1; // 눌렸다가 땟을 때만 실행하기 위해서 (한번만)
			btn.down_event();
		}
		else if (1 == btn.togleKey) 
		{
			retval = true;
			btn.togleKey = 0;
			btn.up_event();		
		}
	}
}

void CInputManager::bind(UCHAR key, CInputManager::Button && btn)
{
	// key 값이 비어있으면 btn을 만들어라
	// 아니면 버튼을 지우고 만들어라
	if (m_mRegisterKeyList.find(key) != end(m_mRegisterKeyList)) m_mRegisterKeyList.erase(key);
	m_mRegisterKeyList.insert(make_pair(key, move(btn)));
}

bool CInputManager::replace(string tag, UCHAR newkey)
{
	// 키 변경
	// tag? :: tag와 똑같은 이름의 btn을 찾아서 newkey로 바꾸어라.
	// but tag가 고유한 값이 아니다. (고유하게 프로그래밍 해야 한다.)
	auto p = find_if(begin(m_mRegisterKeyList), end(m_mRegisterKeyList), [&](const auto & p)
	{
		return p.second.tag == tag;
	});

	if (p == end(m_mRegisterKeyList))
		return false;

	// erase before key
	auto btn = move(m_mRegisterKeyList.at(p->first));
	m_mRegisterKeyList.erase(p->first);

	bind(newkey, move(btn));

	return true;
}

bool CInputManager::replace(std::string tag, UCHAR newkey, LPCSTR appName, LPCSTR filePath)
{
	auto retval = replace(tag, newkey);
	SaveKey(appName, tag, filePath);

	return retval;
}

void CInputManager::SaveAllKey(LPCSTR appName, LPCSTR filePath)
{
	// 설정 변경 유지
	for (auto& data : m_mRegisterKeyList)
	{
		auto& key = data.first;
		auto& btn = data.second;
		WritePrivateProfileStringA(appName, btn.tag.c_str(), to_string(key).c_str(), filePath);
	}
}

void CInputManager::SaveKey(LPCSTR appName, string tag, LPCSTR filePath)
{
	auto data = find_if(begin(m_mRegisterKeyList), end(m_mRegisterKeyList), [&](const auto & p)
	{
		return p.second.tag == tag;
	});

	if (data == end(m_mRegisterKeyList))
		return;

	auto& key = data->first;
	auto& btn = data->second;

	// 설정 변경 유지
	WritePrivateProfileStringA(appName, btn.tag.c_str(), to_string(key).c_str(), filePath);
}
