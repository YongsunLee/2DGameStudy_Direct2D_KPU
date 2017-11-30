#pragma once

#include "Object/Object.h"
#include "pch/Buff.h"

class CIndRes;

class CMonster : public CObject
{
public:
	enum Dir { left = 1, top = 3, right = 2, bottom = 0 };

	CMonster(D2D_SIZE_U pt);
	virtual ~CMonster() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Draw(ID2D1HwndRenderTarget* RenderTarget) override;
	virtual void DrawUI(ID2D1HwndRenderTarget* RenderTarget) override;

	virtual void RegisterImage(CIndRes* indres,
		ID2D1HwndRenderTarget* RenderTarget, path filename, D2D_SIZE_U szSprite);

	virtual void RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp, D2D_SIZE_U szSprite);
	virtual void RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp, D2D_SIZE_U szSprite) noexcept;

	void Move(Dir dir);

	const D2D_SIZE_U& GetCoord() const { return m_szCoord; }

	void GetDamage(float att)
	{
		m_UserInfo.HP = max(0, m_UserInfo.HP - max(0, att - m_UserInfo.def));
		m_UserInfo.GetExp(rand() % 3 + 1);
	}

private:
	ComPtr<ID2D1Bitmap1>		m_bmpImage;
	D2D_SIZE_U					m_szCoord;

	Dir							m_Direction;
	D2D_SIZE_U					m_szSprite;
	UINT						m_iSprite;

	float						m_fTick;

public:
	const UserInfo& GetInfo() const { return m_UserInfo; }

private:
	// 유저 Level 1과 슬라임 Level 1이 같으면 안된다.
	UserInfo					m_UserInfo;

public:
	bool Action() { 
		bool retval = m_bAction;
		if (m_bAction) m_bAction = false;
		return retval; 
	}

	const D2D_SIZE_U GetAttCoord() const {
		D2D_SIZE_U retval = GetCoord();
		switch (m_Direction)
		{
		case CMonster::left:
			if (retval.width > 0)
				retval.width -= 1;
			break;
		case CMonster::top:
			retval.height += 1;
			break;
		case CMonster::right:
			retval.width += 1;
			break;
		case CMonster::bottom:
			if (retval.height > 0)
				retval.height += 1;
			break;
		}
		return retval;
	}

	float GetAtt() const { return m_UserInfo.att; }
	float GetDef() const { return m_UserInfo.def; }


private:
	// 움직임 감지
	bool						m_bAction{ false };
	float						m_fActionTick{ 0 };
};