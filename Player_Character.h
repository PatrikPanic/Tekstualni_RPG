#pragma once
#include <iostream>
#include <string>
#include "Inventory.h"
#include "Exception.h"

namespace oop::projekt
{
	// Entity - apstraktna osnova za sve likove u igri. Cuva zajednicke
	// statistike i ruksak, a izracun napada i primljene stete prepusta
	// izvedenim klasama, jer se kod igraca uzima u obzir i oprema.
	class Entity
	{
	protected:
		std::string name;
		float hp;
		float max_hp;
		float base_attack;
		float base_defense;
		float base_speed;
		Backpack_Inventory e_inventory;
	public:
		Entity(std::string name, float max_hp, float base_attack,
			float base_defense, float base_speed, int size);
		bool isAlive() { return hp > 0; }
		virtual float deal_dmg() const = 0;
		virtual float take_dmg(float enemy_dmg);
		virtual float getSpeed()const { return base_speed; }
		float gethp() const { return hp; }
		std::string getName() const { return name; }
		virtual float getDefense() const { return base_defense; }
		Backpack_Inventory& get_backpack_inventory() { return e_inventory; }


	};
	// Player_Character - lik kojim igrac upravlja. Uz osnovne statistike
	// ima spremnike za opremljeni oklop i oruzje, a ukupne vrijednosti se
	// nakon svake promjene opreme ponovno racunaju u recalculate_stats.
	class Player_Character : public Entity
	{
	public:
		float total_attack;
		float total_defense;
		float total_speed;
		Armor_Inventory c_armor;
		Weapon_Inventory c_weapon;

	public:
		Player_Character(std::string name, float max_hp, float base_attack,
			float base_defense, float base_speed,int size);
		void recalculate_stats();
		float deal_dmg() const override{ return total_attack; }
		float take_dmg(float enemy_dmg) override;
		float getSpeed()const override{ return total_speed; }
		float getDefense() const override{ return total_defense; }
		Armor_Inventory& get_armor_inventory() { return c_armor; }
		Weapon_Inventory& get_weapon_inventory() { return c_weapon; }
		bool equip(int backpack_position);
		bool unequip_armor(int slot_position);
		bool unequip_weapon(int slot_position);
		void heal(float heal_amount);
		void takeItem(std::unique_ptr<Item>& item);
		float getMaxHp() const { return max_hp; }
		std::string showStats() const ;
	};

	// Enemy - protivnik. Nema opremu koju bi mogao mijenjati, pa su mu
	// statistike odredene pri stvaranju, a ruksak sluzi samo kao plijen.
	class Enemy : public Entity
	{
	protected:
		// Razina postoji samo kod protivnika. Iz nje se izvode njegove
		// statistike, kolicina i rijetkost plijena te bodovi za pobjedu.
		// Igrac razinu nema jer u igri ne napreduje - jaci postaje iskljucivo
		// preko opreme koju pokupi.
		int LVL;
	public:
		Enemy(std::string name, int LVL, float max_hp, float base_attack,
			float base_defense, float base_speed, int size) 
			: Entity(name, max_hp, base_attack, base_defense, base_speed, size) {
			this->LVL = LVL;
		}
		int getLVL() const { return LVL; }
		float deal_dmg() const { return base_attack; }
	};


	
};

