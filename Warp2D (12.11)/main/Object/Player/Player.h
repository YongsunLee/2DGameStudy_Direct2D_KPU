#pragma once

#include "Object/Object.h"
#include "pch/Buff.h"

class CIndRes;
class CItem;

constexpr float g_fResurrectionTime = 5.f;



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
		if (!IsActive()) return;

		m_UserInfo.HP = max(0, m_UserInfo.HP - max(0, att - m_UserInfo.def));
		m_UserInfo.GetExp(rand() % 3 + 1);
	}

	bool IsMoveCool() const { return m_fMoveCool > m_fMaxMoveCool; }
	bool IsAttackCool() const { return m_fAttCool > m_fMaxAttCool; }

	void ActionAttack() { m_fAttCool = 0.f; }

private:

	ComPtr<ID2D1Bitmap1>		m_bmpImage;
	D2D_SIZE_U					m_szCoord;

	Dir							m_Direction;
	D2D_SIZE_U					m_szSprite;
	UINT						m_iSprite;

	float						m_fTick;
	float						m_fMoveCool;
	float						m_fAttCool;

	bool						m_bMove = false;

	const float					m_fMaxMoveCool = 0.2f;
	const float					m_fMaxAttCool = 0.5f;

public:
	const UserInfo& GetInfo() const { return m_UserInfo; }
	
	const D2D_SIZE_U GetAttCoord() const
	{
		D2D_SIZE_U retval = GetCoord();
		switch (m_Direction)
		{
		case CPlayer::left: if (retval.width > 0)	retval.width  -= 1;	break;
		case CPlayer::top:	if (retval.height > 0)	retval.height -= 1;	break;
		case CPlayer::right:						retval.width  += 1;	break;
		case CPlayer::bottom:						retval.height += 1;	break;
		}
		return retval;
	}

	float GetAtt() const { return m_UserInfo.att + m_EquipBuff.att; }
	float GetDef() const { return m_UserInfo.def + m_EquipBuff.def; }
	float GetMaxHP() const { return m_UserInfo.maxHP + m_EquipBuff.maxHP; }
	float GetHP() const { return m_UserInfo.HP; }

	void Healing(float hp) { m_UserInfo.HP = min(m_UserInfo.HP + hp, GetMaxHP()); }

	bool IsActive() const { return m_UserInfo.HP > 0; }
	Dir Look(Dir dir) { return m_Direction = dir; }

private:
	UserInfo					m_UserInfo;

public:
	friend class CUIInventory;
	friend class CUIEquipment;

private:
	list<unique_ptr<CItem>>		m_lstItem;
	Buff						m_EquipBuff;

private:

	D2D_SIZE_U					m_szResurrectionPos{ SizeU(25, 25) };
	float						m_fResurrectionTick = 0.f;

};
