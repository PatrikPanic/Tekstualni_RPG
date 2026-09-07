#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include "Exception.h"

namespace oop::projekt
{	

	

	// Rijetkost predmeta. Odreduje koliko je predmet jak i koliko se cesto
	// pojavljuje kod protivnika - jaci protivnici nose rjedu opremu.
	enum class Rarity
	{
		Common,
		Rare,
		Epic,
		Legendary
	};


	inline std::string toString(Rarity rarity)
	{
		switch (rarity) {
		case Rarity::Common:
			return "Common";
		case Rarity::Rare:
			return "Rare";
		case Rarity::Epic:
			return "Epic";
		case Rarity::Legendary:
			return "Legendary";
		default:
			return "None";
		}
	}

	inline Rarity stringToRarity(const std::string& s) {
		if (s == "common") return Rarity::Common;
		if (s == "rare") return Rarity::Rare;
		if (s == "epic") return Rarity::Epic;
		if (s == "legendary") return Rarity::Legendary;
		return Rarity::Common;
	}

	// Vrsta predmeta. Junk predmeti nemaju ucinak na statistike i sluze samo
	// kao plijen, dok se Armor i Weapon mogu opremiti.
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
	// ItemTemplateStore - genericko skladiste predlozaka predmeta. Za svaku
	// vrstu predmeta (Item, Armor, Weapon) postoji zaseban skup predlozaka,
	// jer su clanovi staticki, a klasa je predlozak. Predlosci se ucitavaju
	// iz tekstualne datoteke, pa se novi predmeti dodaju bez izmjene koda.
	class ItemTemplateStore
	{
	private:
		static std::map<int, T> templates;
		static std::map<Rarity, std::vector<int>> byrarity;
	public:
		// Ucitava predloske iz datoteke. Prvi redak je zaglavlje i preskace se.
		//
		// Oznake se nastavljaju na vec ucitane predloske, a ne krecu od nule.
		// Kad bi krenule od nule, ponovni poziv bi dodijelio postojece oznake,
		// pa emplace ne bi prepisao stari zapis i dio predlozaka bi se izgubio.
		//
		// Broj ucitanih zapisa broji se zasebno, jer je mapa templates staticka
		// i moze vec sadrzavati predloske ucitane ranije.
		static void loadFromFile(const std::string& path)
		{
			int id = static_cast<int>(templates.size());
			int loaded = 0;
			std::fstream file(path, std::ios::in);
			if (!file.is_open())
				throw FileNotFoundException(path);

			std::string line;
			getline(file, line);
			while (getline(file, line))
			{
				if (line.empty())
					continue;
				T item = T::fromLine(line);
				templates.emplace(id, item);
				byrarity[item.getRarity()].push_back(id);
				id += 1;
				loaded += 1;
			}

			if (loaded == 0)
				throw InvalidDataException(path, "no valid item templates found");
		}
		// Bira nasumicnu oznaku predloska zadane rijetkosti.
		// Vraca -1 ako za tu rijetkost nije ucitan nijedan predlozak.
		static int getRandomId(Rarity rarity) 
		{
			if (byrarity[rarity].empty())
				return -1;
			int idx = rand() % byrarity[rarity].size();
			return byrarity[rarity][idx];
		}
		// Vraca novu kopiju predloska sa zadanom oznakom, ili nullptr ako
		// predlozak ne postoji. Uvijek se vraca kopija, kako izmjena jednog
		// predmeta u igri ne bi promijenila i sam predlozak.
		// Vraca novu kopiju predloska sa zadanom oznakom, ili nullptr ako
		// predlozak ne postoji. Provjera je nuzna jer getRandomId vraca -1
		// kada za trazenu rijetkost nije ucitan nijedan predlozak.
		static std::unique_ptr<T> createItem(int id)
		{
			typename std::map<int, T>::const_iterator found = templates.find(id);
			if (found == templates.end())
				return nullptr;
			return std::make_unique<T>(found->second);
		}
		static int templateCount() { return static_cast<int>(templates.size()); }
	};

	template <typename T>
	std::map<int, T> ItemTemplateStore<T>::templates;

	template <typename T>
	std::map<Rarity, std::vector<int>> ItemTemplateStore<T>::byrarity;

	// Item - osnovni predmet u igri. Cuva naziv, vrstu, rijetkost i oznaku.
	// Iz njega se izvode Armor i Weapon, pa destruktor mora biti virtualan
	// kako bi se izvedeni predmeti ispravno unistili preko pokazivaca na Item.
	class Item : public ItemTemplateStore<Item>
	{
	protected:
		std::string item_name;
		Item_Type item_type;
		Rarity rarity;
		int item_ID;
	public:
		Item(std::string name, Item_Type type,Rarity item_rarity,  int ID) { item_name = name; item_type = type; rarity = item_rarity; item_ID = ID; }
		virtual ~Item() { ; }
		std::string getName() const { return item_name; }
		Item_Type getType() const { return item_type; }
		Rarity getRarity() const { return rarity; }
		int getID() const { return item_ID; }
		virtual std::string seeStats() const;
		static Item fromLine(const std::string& line);
	};

	// Mjesto na kojem se oklop nosi. Vrijednosti se koriste i kao indeksi u
	// spremniku oklopa, pa im se redoslijed ne smije mijenjati.
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


	
	// Armor - dio oklopa. Uz podatke iz Item klase nosi obranu, bonus brzine
	// i mjesto na kojem se nosi.
	class Armor :public Item, public ItemTemplateStore<Armor>
	{
	private:
		int armor_defense;
		float armor_speed;
		ArmorSlot armor_type;
	public:
		Armor(std::string name, int ID, Rarity item_rarity, int defense, float speed, ArmorSlot armorSlot)
			: Item(name, Item_Type::Armor, item_rarity, ID),
			armor_defense(defense),
			armor_speed(speed),
			armor_type(armorSlot){ }
		int getArmorDefense() const { return armor_defense; }
		float getArmorSpeed() const { return armor_speed; }
		ArmorSlot getArmorSlot() const { return armor_type; }
		std::string seeStats() const override;
		static Armor fromLine(const std::string& line);
	};

	// Ruka u kojoj se oruzje drzi. Kao i kod oklopa, vrijednosti sluze kao
	// indeksi u spremniku oruzja.
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


	// Weapon - oruzje. Osim napada moze davati i obranu, pa se stitovi
	// prikazuju kao oruzje u lijevoj ruci.
	class Weapon : public Item, public ItemTemplateStore<Weapon>
	{
	private:
		int weapon_defense;
		float weapon_speed;
		int weapon_attack;
		WeaponSlot weapon_slot;
	public:
		Weapon(std::string name, int ID, Rarity item_rarity, int defense, float speed, int attack, WeaponSlot weapon_slot)
			: Item(name, Item_Type::Weapon, item_rarity, ID),
			weapon_defense(defense), weapon_speed(speed), weapon_attack(attack), weapon_slot(weapon_slot) {
		}
		int getWeaponDefense() const { return weapon_defense; }
		float getWeaponSpeed() const { return weapon_speed; }
		int getWeaponAttack() const { return weapon_attack; }
		WeaponSlot getWeaponSlot() const { return weapon_slot; }
		std::string seeStats() const override;
		static Weapon fromLine(const std::string& line);
	};
}