// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WINWIDTH 800
#define WINHEIGHT 600

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ ��Ÿ�� ��� �����Դϴ�.
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <chrono>
#include <memory>
using namespace std;

// ComPtr
#include <wrl/client.h>
using namespace Microsoft::WRL;

// Direct2D
#include <d2d1_3.h>
#include <dwrite_3.h>
using namespace D2D1;


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
struct Vec3 {
	float x;
	float y;
	float z;
};

struct Color4 {
	float r;
	float g;
	float b;
	float a;
};
