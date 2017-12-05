#pragma once
#include "Object/Object.h"
#include "pch/Buff.h"


class CIndRes;
class CPlayer;

class CItem 
	: public CObject
{

public:

	enum TYPE
	{
		  Unknown
		, Equipment
		, Consume
		, Normal
	};

	CItem(D2D_SIZE_U coord);
	virtual ~CItem() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Draw(ID2D1HwndRenderTarget* RenderTarget) override;

	virtual void RegisterImage(CIndRes* indres,
		ID2D1HwndRenderTarget* RenderTarget, path filename);

	virtual void RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp);
	virtual void RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept;

	ID2D1Bitmap1* GetBitmap() const { return m_bmpImage.Get(); }

	bool IsCollision(D2D_SIZE_U sz) const { return sz == m_szCoord; }
	const D2D_SIZE_U& GetCoord() const { return m_szCoord; }

	void ResetPosition() { SetPosition(Point2F()); }

	TYPE ItemType() const { return m_ItemType; }
protected:

	ComPtr<ID2D1Bitmap1>		m_bmpImage;
	D2D_SIZE_U					m_szCoord;

	TYPE						m_ItemType = Unknown;
};

class CEquipmentItem
	: public CItem
{
public:

	enum TYPE
	{
		  sword		= 0
		, sword_0
		, sword_1
		, shield	= 100
		, shield_0
	};
	
	friend unique_ptr<CEquipmentItem> make_Equip_Item(D2D_SIZE_U coord, CEquipmentItem::TYPE type, CIndRes* indres, ID2D1HwndRenderTarget* rendertarget);
	
	// 이러케 코딩하면 안대 알게써?
	// 모르게따고? 야!!!!!!
//	template<class _Ty, class... _Types, class = enable_if_t<!is_array<_Ty>::value>>
//	friend unique_ptr<_Ty> std::make_unique(_Types&&... _Args);

protected:

	template<typename... Args>
	CEquipmentItem(D2D_SIZE_U coord, TYPE type, Args&&... buffInfo)
		: CItem{ coord }
		, type{ GuessType(type) }
		, buff{ std::forward<Args>(buffInfo)... }
	{
		m_ItemType = CItem::TYPE::Equipment;
	}

public:

	CEquipmentItem() = delete;
	virtual ~CEquipmentItem() override;

public:

	bool IsEquipment() const { return m_bEqiupment; }
	void Equipment(bool equip) { m_bEqiupment = equip; }
	TYPE GetType() const { return type; }
	const Buff& GetBuff() const { return buff; }

	constexpr static TYPE GuessType(TYPE type)
	{
		if (type < shield) return sword;
		return shield;
	}

protected:

	const TYPE type;
	const Buff buff;
	bool m_bEqiupment{ false };
};

unique_ptr<CEquipmentItem> make_Equip_Item(D2D_SIZE_U coord, CEquipmentItem::TYPE type, CIndRes* indres, ID2D1HwndRenderTarget* rendertarget);



class CConsumeItem
	: public CItem
{
public:

	enum TYPE
	{
		  Healing
		, Mana
	};
	
	friend unique_ptr<CConsumeItem> make_Consume_Item(D2D_SIZE_U coord, CConsumeItem::TYPE type, CIndRes* indres, ID2D1HwndRenderTarget* rendertarget);
	
	// 이러케 코딩하면 안대 알게써?
	// 모르게따고? 야!!!!!!
//	template<class _Ty, class... _Types, class = enable_if_t<!is_array<_Ty>::value>>
//	friend unique_ptr<_Ty> std::make_unique(_Types&&... _Args);

protected:

	CConsumeItem(D2D_SIZE_U coord, TYPE type, const function<void(CPlayer&)>& fn, int num = 1)
		: CItem{ coord }
		, type{ type }
		, buff{ fn }
		, num{ num }
	{
		m_ItemType = CItem::TYPE::Consume;
	}

public:

	CConsumeItem() = delete;
	virtual ~CConsumeItem() override;

public:

	bool IsEquipment() const { return m_bEqiupment; }
	void Equipment(bool equip) { m_bEqiupment = equip; }
	TYPE GetType() const { return type; }
	void Consume(CPlayer& player) { buff(player); num--; }
	int CurrentEa() const { return num; }
	void AdjustEa(int n) { num += n; }

protected:

	int num;
	const TYPE type;
	const function<void(CPlayer&)> buff;
	bool m_bEqiupment{ false };

};

unique_ptr<CConsumeItem> make_Consume_Item(D2D_SIZE_U coord, CConsumeItem::TYPE type, CIndRes* indres, ID2D1HwndRenderTarget* rendertarget);
