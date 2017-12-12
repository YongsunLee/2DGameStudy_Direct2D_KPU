#include "stdafx.h"
#include "Input.h"

void CInputManager::ProcessInput(bool(&arrByte)[256])
{
	/*
		GetKeyboardState
		BYTE 256 ���� �ѱ�� ����Ű�� ���ȴ��� �ȴ��ȴ����� ���� �Ǵܰ��� �Ѱ���
	*/
	bool retval = false;
	BOOL bKeyState = GetKeyboardState(m_pKeyBuffer);
	UNREFERENCED_PARAMETER(bKeyState);

	// ����� �� Ű ����Ʈ�� ���ؼ� ����
	for (auto& data : m_mRegisterKeyList)
	{
		auto& key = data.first;
		auto& btn = data.second;
		
		// false�� ��쿡�� ����
		// false�� true�� ������ ����
		if (arrByte[key]) continue;
		arrByte[key] = true;

		// ���ȴ��� �Ǵ��ϴ� �Լ� (1 == ���ȴ�. 0 == �ȴ��ȴ�.)
		if (1 == GetHighBit(m_pKeyBuffer[key]))
		{
			retval = true;
			btn.togleKey = 1; // ���ȴٰ� ���� ���� �����ϱ� ���ؼ� (�ѹ���)
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
	// key ���� ��������� btn�� ������
	// �ƴϸ� ��ư�� ����� ������
	if (m_mRegisterKeyList.find(key) != end(m_mRegisterKeyList)) m_mRegisterKeyList.erase(key);
	m_mRegisterKeyList.insert(make_pair(key, move(btn)));
}

bool CInputManager::replace(string tag, UCHAR newkey)
{
	// Ű ����
	// tag? :: tag�� �Ȱ��� �̸��� btn�� ã�Ƽ� newkey�� �ٲپ��.
	// but tag�� ������ ���� �ƴϴ�. (�����ϰ� ���α׷��� �ؾ� �Ѵ�.)
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
	// ���� ���� ����
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

	// ���� ���� ����
	WritePrivateProfileStringA(appName, btn.tag.c_str(), to_string(key).c_str(), filePath);
}
