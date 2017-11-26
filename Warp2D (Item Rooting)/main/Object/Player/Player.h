#pragma once

#include "Object/Object.h"
class CIndRes;

struct UserStatus {
	float						maxHP;
	float						HP;

	float						atk;
	float						def;
	float						agi;
	float						dex;

	UINT						level;
	UINT						exp;

	bool GetExp(int exp) {
		this->exp += exp;
		return LevelUp();
	}

	bool LevelUp()
	{
		if (exp < level * 100) return false;
		exp -= level * 100;
		level++;
		
		maxHP += 12;
		HP = maxHP;
		atk += 1.125f;
		def++;
		dex += 3;
		agi += 2;
		return true;
	}
	static UserStatus GetStatusFromLevel(UINT level) {
		UserStatus status;
		status.level = level;
		status.exp = 0;
		status.maxHP = level * 12 + 100;
		status.HP = status.maxHP;

		status.atk = 10 +level * 1.125f;
		status.def = 5 + level;
		status.dex = level * 3.f;
		status.agi = 5 + level * 2.0f;
		return status;
	}

};

class CPlayer
	: public CObject
{
public:
	enum Dir { left = 1, top = 3, right = 2, bottom = 0 };

	CPlayer(D2D_SIZE_U pt);
	virtual ~CPlayer() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Draw(ID2D1HwndRenderTarget* RenderTarget) override;

	virtual void RegisterImage(CIndRes* indres,
		ID2D1HwndRenderTarget* RenderTarget, path filename, D2D_SIZE_U szSprite);

	virtual void RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp, D2D_SIZE_U szSprite);
	virtual void RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp, D2D_SIZE_U szSprite) noexcept;

	void Move(Dir dir);

	const D2D_SIZE_U& GetCoord() const	{ return m_szCoord; }

private:

	ComPtr<ID2D1Bitmap1>		m_bmpImage;
	D2D_SIZE_U					m_szCoord;

	Dir							m_Direction;
	D2D_SIZE_U					m_szSprite;
	UINT						m_iSprite;

	float						m_fTick;

	// Lecture 07
public:
	const UserStatus& GetStatus() const { return m_UserStatus; }
	virtual void DrawUI(ID2D1HwndRenderTarget* RenderTarget) override;

	void GetDamage(float atk) 
	{
		m_UserStatus.HP = max(0, m_UserStatus.HP - max(0, atk - m_UserStatus.def));
		
		// exp가 변화가 있으면 다음에는 무조건 LevelUp을 확인해야함.
		m_UserStatus.GetExp((rand() % 3) + 1);
	}

private:
	UserStatus					m_UserStatus;
};
