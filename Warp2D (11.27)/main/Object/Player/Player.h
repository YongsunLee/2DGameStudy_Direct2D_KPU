#pragma once

#include "Object/Object.h"
#include "pch/Buff.h"

class CIndRes;
class CItem;

class CPlayer
	: public CObject
{
public:
	enum Dir { left = 1, top = 3, right = 2, bottom = 0 };

	CPlayer(D2D_SIZE_U pt);
	virtual ~CPlayer() override;

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
		m_UserInfo.HP = max(0, m_UserInfo.HP - max(0, att - GetDef()));
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

	float GetAtt() const { return m_UserInfo.att + m_EquipBuff.att; }
	float GetDef() const { return m_UserInfo.def + m_EquipBuff.def; }

	const D2D_SIZE_U GetAttCoord() const {
		D2D_SIZE_U retval = GetCoord();
		switch (m_Direction)
		{
		case CPlayer::left:
			if (retval.width > 0)
				retval.width -= 1;
			break;
		case CPlayer::top:
			if (retval.height > 0)
				retval.height -= 1;
			break;
		case CPlayer::right:
			retval.width += 1;
			break;
		case CPlayer::bottom:
				retval.height += 1;
			break;
		}
		return retval;
	}

private:
	UserInfo					m_UserInfo;

public:
	friend class CUIInventory;
	friend class CUIEquipment;

private:
	list<unique_ptr<CItem>>		m_lstItem;
	Buff						m_EquipBuff;

};
