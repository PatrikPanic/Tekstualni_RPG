#include "Player_Character.h"
#include <iostream>
#include <string>

namespace oop::projekt
{
	Entity::Entity(std::string name,int LVL, float max_hp, float base_attack,
		float base_defense, float base_speed, int size) : e_invenotry(size)
	{
		this->name = name;
		this->LVL = LVL;
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

	Player_Character::Player_Character(std::string name,int LVL, float max_hp, float base_attack,
		float base_defense, float base_speed,int size) : Entity(name,LVL, max_hp, base_attack, base_defense, base_speed,size)
	{
		recalcute_stats();
	}


	void Player_Character::recalcute_stats() 
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
		if (e_invenotry.isFull())
			return false;
		std::unique_ptr<Armor> old = c_armor.removeItem(slot_position);
		if (old)
			e_invenotry.addItem(std::unique_ptr<Item>(old.release())); // natrag u backpack
		recalcute_stats();
		return true;
	}

	bool Player_Character::unequip_weapon(int slot_position)
	{
		if (e_invenotry.isFull())
			return false;
		std::unique_ptr<Weapon> old = c_weapon.removeItem(slot_position);
		if (old)
			e_invenotry.addItem(std::unique_ptr<Item>(old.release()));
		recalcute_stats();
		return true;
	}

	bool Player_Character::equip(int backpack_position)
	{
		std::unique_ptr<Item> item = e_invenotry.removeItem(backpack_position);
		if (!item) 
			return false;
		if (item->getType() == Item_Type::Armor)
		{
			std::unique_ptr<Armor> armor = c_armor.transferToArmor(std::move(item));
			unequip_armor(static_cast<int>(armor->getArmorSlot())); 
			c_armor.addItem(std::move(armor));
			recalcute_stats();
			return true;
		}
		else if (item->getType() == Item_Type::Weapon) 
		{
			std::unique_ptr<Weapon> weapon = c_weapon.transferToWeapon(std::move(item));
			unequip_weapon(static_cast<int>(weapon->getWeaponSlot()));
			c_weapon.addItem(std::move(weapon));
			recalcute_stats();
			return true;
		}

		e_invenotry.addItem(move(item));
		return false;
	}

	std::string Player_Character::showstats() const
	{
		std::string out;
		float armor_def = c_armor.total_armor_defense();
		float armor_spd = c_armor.total_armor_speed();
		float weapon_atk = c_weapon.total_weapon_attack();
		float weapon_def = c_weapon.total_weapon_defense();
		float weapon_spd = c_weapon.total_weapon_speed();

		return "Name: " + name + "\n" +
			"Level: " + std::to_string(LVL) + "\n" +
			"HP: " + std::to_string(hp) + " / " + std::to_string(max_hp) + "\n" +
			"Attack: " + std::to_string(base_attack) + " (+" + std::to_string(weapon_atk) + " weapon) = " + std::to_string(total_attack) + "\n" +
			"Defense: " + std::to_string(base_defense) + " (+" + std::to_string(armor_def + weapon_def) + " oprema) = " + std::to_string(total_defense) + "\n" +
			"Speed: " + std::to_string(base_speed) + " (+" + std::to_string(armor_spd + weapon_spd) + " oprema) = " + std::to_string(total_speed) + "\n";
	}
};
