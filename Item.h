#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>

namespace oop::projekt
{	

	

	enum class Rarity
	{
		Common,
		Rear,
		Epic,
		Legendery
	};


	inline std::string toString(Rarity rarity)
	{
		switch (rarity) {
		case Rarity::Common:
			return "Common";
		case Rarity::Rear:
			return "Rear";
		case Rarity::Epic:
			return "Epic";
		case Rarity::Legendery:
			return "Legendery";
		default:
			return "None";
		}
	}

	inline Rarity stringToRarity(const std::string& s) {
		if (s == "common") return Rarity::Common;
		if (s == "rare") return Rarity::Rear;
		if (s == "epic") return Rarity::Epic;
		if (s == "legendary") return Rarity::Legendery;
		return Rarity::Common;
	}

	enum class Item_Type
	{
		Armor,
		Weapon,
		Junk,
		Health_Potions
	};

	inline std::string toString(Item_Type type)
	{
		switch (type) {
		case Item_Type::Armor:
			return "Armor";
		case Item_Type::Weapon:
			return "Weapon";
		case Item_Type::Junk:
			return "Junk";
		case Item_Type::Health_Potions:
			return "Health_Potion";
		default:
			return "None";
		}
	}

	inline Item_Type stringToItemType(const std::string& s) {
		if (s == "Armor") return Item_Type::Armor;
		if (s == "Weapon") return Item_Type::Weapon;
		if (s == "Junk") return Item_Type::Junk;
		if (s == "Health_Potion") return Item_Type::Health_Potions;
		return Item_Type::Junk; 
	}

	template <typename T>
	class ItemTemplateStore
	{
	private:
		static std::map<int, T> templates;
		static std::map<Rarity, std::vector<int>> byrarity;
	public:
		static void loadFromFile(const std::string& path)
		{
			int id = 0;
			std::fstream file(path, std::ios::in);
			if (file.is_open())
			{
				std::string line;
				getline(file, line);
				while (getline(file, line))
				{
					T item = T::fromLine(line);
					templates.emplace(id, item);
					byrarity[item.getRarity()].push_back(id);
					id += 1;
				}
			}
		}
		static int getRandomId(Rarity rarity) 
		{
			if (byrarity[rarity].empty())
				return -1;
			int idx = rand() % byrarity[rarity].size();
			return byrarity[rarity][idx];
		}
		static std::unique_ptr<T> createItem(int id) { return std::make_unique<T>(templates.at(id)); }
		static int templateCount() { return static_cast<int>(templates.size()); }
	};

	template <typename T>
	std::map<Rarity, std::vector<int>> ItemTemplateStore<T>::byrarity;

	class Item : public ItemTemplateStore<Item>
	{
	protected:
		std::string item_name;
		Item_Type item_type;
		Rarity rarity;
		int item_ID;
	public:
		Item(std::string name, Item_Type type,Rarity item_rearity,  int ID) { item_name = name; item_type = type; rarity = item_rearity; item_ID = ID; }
		virtual ~Item() { ; }
		std::string getName() const { return item_name; }
		Item_Type getType() const { return item_type; }
		Rarity getRarity() const { return rarity; }
		int getID() const { return item_ID; }
		virtual std::string seeStats() const;
		static Item fromLine(const std::string& line);
	};

	enum class ArmorSlot
	{
		Helmet,
		Chest,
		Gloves,
		Pants,
		Boots
	};

	inline std::string toString(ArmorSlot armor_slot)
	{
		switch (armor_slot) {
		case ArmorSlot::Helmet:
			return "Helmet";
		case ArmorSlot::Chest:
			return "Chest";
		case ArmorSlot::Gloves:
			return "Gloves";
		case ArmorSlot::Pants:
			return "Pants";
		case ArmorSlot::Boots:
			return "Boots";
		default:
			return "None";
		}
	}

	inline ArmorSlot stringToArmorSlot(const std::string& s) {
		if (s == "helmet") return ArmorSlot::Helmet;
		if (s == "chest") return ArmorSlot::Chest;
		if (s == "gloves") return ArmorSlot::Gloves;
		if (s == "pants") return ArmorSlot::Pants;
		if (s == "boots") return ArmorSlot::Boots;
		return ArmorSlot::Helmet;
	}


	
	class Armor :public Item, public ItemTemplateStore<Armor>
	{
	private:
		int armor_defense;
		float armor_speed;
		ArmorSlot armor_type;
	public:
		Armor(std::string name, int ID, Rarity item_rearity, int defense, float speed, ArmorSlot armorSlot)
			: Item(name, Item_Type::Armor, item_rearity, ID),
			armor_defense(defense),
			armor_speed(speed),
			armor_type(armorSlot){ }
		int getArmorDefense() const { return armor_defense; }
		float getArmorSpeed() const { return armor_speed; }
		ArmorSlot getArmorSlot() const { return armor_type; }
		std::string seeStats() const override;
		static Armor fromLine(const std::string& line);
	};

	enum class WeaponSlot
	{
		Left_hand,
		Right_hand
	};

	inline std::string toString(WeaponSlot weapon_slot)
	{
		switch (weapon_slot) {
		case WeaponSlot::Left_hand:
			return "Left Hand";
		case WeaponSlot::Right_hand:
			return "Right Hand";
		default:
			return "None";
		}
	}

	inline WeaponSlot stringToWeaponSlot(const std::string& s) {
		if (s == "right_hand") return WeaponSlot::Right_hand;
		if (s == "left_hand") return WeaponSlot::Left_hand;
		return WeaponSlot::Right_hand;
	}


	class Weapon : public Item, public ItemTemplateStore<Weapon>
	{
	private:
		int weapon_defense;
		float weapon_speed;
		int weapon_attack;
		WeaponSlot LR_hand;
	public:
		Weapon(std::string name, int ID, Rarity item_rearity, int defense, float speed, int attack, WeaponSlot LR_hand)
			: Item(name, Item_Type::Weapon, item_rearity, ID),
			weapon_defense(defense), weapon_speed(speed), weapon_attack(attack), LR_hand(LR_hand) {
		}
		int getWeaponDefense() const { return weapon_defense; }
		float getWeaponSpeed() const { return weapon_speed; }
		int getWeaponAttack() const { return weapon_attack; }
		WeaponSlot getWeaponSlot() const { return LR_hand; }
		std::string seeStats() const override;
		static Weapon fromLine(const std::string& line);
	};
}