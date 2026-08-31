#pragma once
#include <iostream>
#include <string>
#include "inventory.h"

namespace oop::projekt
{
	class Entity
	{
	protected:
		std::string name;
		int LVL;
		float hp;
		float max_hp;
		float base_attack;
		float base_defense;
		float base_speed;
		Backpack_Inventory e_invenotry;
	public:
		Entity(std::string name, int LVL, float max_hp, float base_attack,
			float base_defense, float base_speed, int size);
		bool isAlive() { return hp > 0; }
		virtual float deal_dmg() const = 0;
		virtual float take_dmg(float enemy_dmg);
		virtual float getSpeed()const { return base_speed; }
		float gethp() const { return hp; }
		std::string getName() const { return name; }
		int getLVL() const { return LVL; }
		virtual float getDefense() const { return base_defense; }
		Backpack_Inventory& get_backpack_inventory() { return e_invenotry; }


	};
	class Player_Character : public Entity
	{
	public:
		float total_attack;
		float total_defense;
		float total_speed;
		Armor_Inventory c_armor;
		Weapon_Inventory c_weapon;

	public:
		Player_Character(std::string name,int LVL, float max_hp, float base_attack,
			float base_defense, float base_speed,int size);
		void recalcute_stats();
		float deal_dmg() const override{ return total_attack; }
		float take_dmg(float enemy_dmg) override;
		float getSpeed()const override{ return total_speed; }
		float getDefense() const override{ return total_defense; }
		std::vector<std::string> show_armor_inventory() const {};
		std::vector<std::string> show_weapon_inventory() const {};
		Armor_Inventory& get_armor_inventory() { return c_armor; }
		Weapon_Inventory& get_weapon_inventory() { return c_weapon; }
		bool equip(int backpack_position);
		bool unequip_armor(int slot_position);
		bool unequip_weapon(int slot_position);
		std::string showstats() const ;
	};

	class Enemy : public Entity
	{
	public:
		Enemy(std::string name, int LVL, float max_hp, float base_attack,
			float base_defense, float base_speed, int size) 
			: Entity(name, LVL, max_hp, base_attack, base_defense, base_speed, size) {
		}
		float deal_dmg() const { return base_attack; }
	};


	
};

