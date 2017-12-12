#pragma once

#include <memory>



struct UserInfo
{
	float	maxHP;
	float	HP;

	float	att;
	float	def;

	float	dex;
	float	agi;

	UINT	level;
	UINT	exp;

	bool GetExp(int exp) { this->exp += exp; return LevelUp(); }
	bool LevelUp()
	{
		if (exp < level * 100) return false;
		exp -= level * 100;
		level++;

		maxHP += 12;
		HP = maxHP;
		att += 1.125f;
		def++;
		dex += 3;
		agi += 2;
		return true;
	}
	constexpr static UserInfo GetInfoFromLevel(UINT level) 
	{
		return {
			static_cast<float>(level * 12 + 100),
			static_cast<float>(level * 12 + 100),
			static_cast<float>(10 + level * 1.125f),
			static_cast<float>(5 + level),
			static_cast<float>(level * 3),
			static_cast<float>(5 + level * 2),
			level,
			0
		};
	}
};

struct Buff
{
	enum TYPE
	{
		  HP
		, ATT
		, DEF
		, DEX
		, AGI
		, CUSTOM
	};

	float	maxHP	= 0.f;

	float	att		= 0.f;
	float	def		= 0.f;

	float	dex		= 0.f;
	float	agi		= 0.f;
	
	Buff(float maxHP, float att, float def, float dex, float agi)
		: maxHP{ maxHP}
		, att{ att }
		, def{ def }
		, dex{ dex }
		, agi{ agi }
	{

	}

	template<typename ...Args>
	Buff(Args&& ...args)
	{
		InitBuff(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	void InitBuff(std::pair<TYPE, float>& type, Args&& ...args)
	{
		switch (type.first)
		{
		case HP:	maxHP	= type.second;		break;

		case ATT:	att		= type.second;		break;
		case DEF:	def		= type.second;		break;
							
		case DEX:	dex		= type.second;		break;
		case AGI:	agi		= type.second;		break;
		}
	
		InitBuff(std::forward<Args>(args)...);
	}

	void InitBuff() { }

	Buff& operator+=(const Buff& buff)
	{
		maxHP += buff.maxHP;
		att += buff.att;
		def += buff.def;
		dex += buff.dex;
		agi += buff.agi;
		return *this;
	}

};

constexpr std::pair<Buff::TYPE, float> BUFF(Buff::TYPE type, float data)
{
	return std::make_pair(type, data);
 }

inline Buff operator+(const Buff& lhs, const Buff& rhs)
{
	return
	{
		  lhs.maxHP + rhs.maxHP
		, lhs.att + rhs.att	
		, lhs.def + rhs.def	
		, lhs.dex + rhs.dex	
		, lhs.agi + rhs.agi	
	};
}
