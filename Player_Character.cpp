#include "Player_Character.h"
#include <iostream>
#include <string>

namespace oop::projekt
{
	Entity::Entity(std::string name, float max_hp, float base_attack,
		float base_defense, float base_speed, int size) : e_inventory(size)
	{
		this->name = name;
		this->max_hp = max_hp;
		hp = max_hp;
		this->base_attack = base_attack;
		this->base_defense = base_defense;
		this->base_speed = base_speed;
	}

	float Entity::take_dmg(float enemy_dmg)
	{
		if (enemy_dmg - base_defense > 0)
		{
			hp = hp - (enemy_dmg - base_defense);
			return enemy_dmg - base_defense;
		}
		return 0;
	}

	Player_Character::Player_Character(std::string name, float max_hp, float base_attack,
		float base_defense, float base_speed,int size) : Entity(name, max_hp, base_attack, base_defense, base_speed,size)
	{
		recalculate_stats();
	}


	// Ponovno racuna ukupne statistike iz osnovnih vrijednosti i bonusa
	// opremljenih predmeta. Mora se pozvati nakon svakog opremanja i skidanja,
	// inace bi lik zadrzao stare vrijednosti.
	void Player_Character::recalculate_stats() 
	{
		total_defense = base_defense + c_armor.total_armor_defense() + c_weapon.total_weapon_defense();
		total_speed = base_speed + c_armor.total_armor_speed() + c_weapon.total_weapon_speed();
		total_attack = base_attack + c_weapon.total_weapon_attack();
	}
	
	float Player_Character::take_dmg(float enemy_dmg)
	{
		if (enemy_dmg - total_defense > 0)
		{
			hp = hp - (enemy_dmg - total_defense);
			return enemy_dmg - total_defense;
		}
		return 0;
	}

	bool Player_Character::unequip_armor(int slot_position)
	{
		if (e_inventory.isFull())
			return false;
		std::unique_ptr<Armor> old = c_armor.removeItem(slot_position);
		if (old)
			e_inventory.addItem(std::move(old)); // natrag u backpack
		recalculate_stats();
		return true;
	}

	bool Player_Character::unequip_weapon(int slot_position)
	{
		if (e_inventory.isFull())
			return false;
		std::unique_ptr<Weapon> old = c_weapon.removeItem(slot_position);
		if (old)
			e_inventory.addItem(std::move(old));
		recalculate_stats();
		return true;
	}

	// Oprema predmet s zadanog mjesta u ruksaku. Predmet koji je vec bio u tom
	// slotu vraca se u ruksak, pa u njemu mora biti slobodnog mjesta.
	// Vraca false ako mjesto u ruksaku nije popunjeno ili predmet nije oprema.
	// Oprema predmet s zadanog mjesta u ruksaku. Predmet koji je vec bio u tom
	// slotu vraca se u ruksak, pa u njemu mora biti slobodnog mjesta.
	// Ako skidanje ne uspije, novi predmet se vraca u ruksak umjesto da se
	// izgubi, a metoda vraca false.
	bool Player_Character::equip(int backpack_position)
	{
		std::unique_ptr<Item> item = e_inventory.removeItem(backpack_position);
		if (!item) 
			return false;

		if (item->getType() == Item_Type::Armor)
		{
			std::unique_ptr<Armor> armor = c_armor.transferToArmor(std::move(item));
			if (!armor)
				return false;

			if (!unequip_armor(static_cast<int>(armor->getArmorSlot())))
			{
				e_inventory.addItem(std::move(armor));
				return false;
			}
			c_armor.addItem(std::move(armor));
			recalculate_stats();
			return true;
		}
		else if (item->getType() == Item_Type::Weapon) 
		{
			std::unique_ptr<Weapon> weapon = c_weapon.transferToWeapon(std::move(item));
			if (!weapon)
				return false;

			if (!unequip_weapon(static_cast<int>(weapon->getWeaponSlot())))
			{
				e_inventory.addItem(std::move(weapon));
				return false;
			}
			c_weapon.addItem(std::move(weapon));
			recalculate_stats();
			return true;
		}

		e_inventory.addItem(std::move(item));
		return false;
	}

	// Stavlja predmet u ruksak. Za razliku od addItem, koji samo vraca false,
	// ovdje se baca iznimka jer pozivatelj (uzimanje plijena) mora razlikovati
	// pun ruksak od neispravnog predmeta.
	//
	// Predmet se prima po referenci, a ne po vrijednosti. Kad bi se primao po
	// vrijednosti, vlasnistvo bi se prenijelo vec pri pozivu, pa bi se predmet
	// unistio zajedno s parametrom kad se baci iznimka. Ovako on ostaje kod
	// pozivatelja, koji ga moze vratiti protivniku.
	void Player_Character::takeItem(std::unique_ptr<Item>& item)
	{
		if (!item)
			return;
		if (e_inventory.isFull())
			throw InventoryFullException(item->getName());
		e_inventory.addItem(std::move(item));
	}

	// Vraca bodove zivota, ali nikad iznad maksimuma.
	void Player_Character::heal(float heal_amount)
	{
		if (heal_amount <= 0)
			return;
		hp = hp + heal_amount;
		if (hp > max_hp)
			hp = max_hp;
	}

	// std::to_string kod float vrijednosti uvijek ispisuje sest decimala, pa se
	// cijeli brojevi ispisuju bez njih, a brzina zaokruzuje na dvije.
	static std::string formatStat(float value)
	{
		if (value == static_cast<int>(value))
			return std::to_string(static_cast<int>(value));

		std::string text = std::to_string(value);
		size_t dot = text.find('.');
		if (dot != std::string::npos && text.size() > dot + 3)
			text = text.substr(0, dot + 3);
		return text;
	}

	std::string Player_Character::showStats() const
	{
		std::string out;
		float armor_def = c_armor.total_armor_defense();
		float armor_spd = c_armor.total_armor_speed();
		float weapon_atk = c_weapon.total_weapon_attack();
		float weapon_def = c_weapon.total_weapon_defense();
		float weapon_spd = c_weapon.total_weapon_speed();

		return "Name: " + name + "\n" +
			"HP: " + formatStat(hp) + " / " + formatStat(max_hp) + "\n" +
			"Attack: " + formatStat(base_attack) + " (+" + formatStat(weapon_atk) + " weapon) = " + formatStat(total_attack) + "\n" +
			"Defense: " + formatStat(base_defense) + " (+" + formatStat(armor_def + weapon_def) + " gear) = " + formatStat(total_defense) + "\n" +
			"Speed: " + formatStat(base_speed) + " (+" + formatStat(armor_spd + weapon_spd) + " gear) = " + formatStat(total_speed) + "\n";
	}
};
