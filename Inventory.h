#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "item.h"


namespace oop::projekt
{
	template <typename T>

	class Inventory
	{
	protected:
		std::vector<std::unique_ptr <T>> slots;
	public:
		Inventory(int size) { slots.resize(size); }

		//funkcija za dodavanje itema u invenotry na prvo slobodno mjesto
		virtual bool addItem(std::unique_ptr <T> addingitem)
		{
			for (int i = 0; i < slots.size(); i++)
			{
				if (!slots[i])
				{
					slots[i] = std::move(addingitem);
					return true;
				}
			}
			std::cout << "invenotry je pun" << std::endl;
			return false;
		}
		//funkcija za micanje itema sa pozicije
		virtual std::unique_ptr <T> removeItem(int position)
		{
			if (position >= slots.size())
			{
				return nullptr;
			}
			else
			{
				std::unique_ptr <T> returningitem = std::move(slots[position]);
				slots[position] = nullptr;
				return returningitem;
			}
		}

		virtual std::vector<std::string> getItemNames() const
		{
			std::vector<std::string> inventoryitemnames;
			for (int i = 0; i < slots.size(); i++)
			{
				if (slots[i])
					inventoryitemnames.push_back(slots[i]->getName());
			}
			return inventoryitemnames;
		}

		std::string getItemNames_string() const
		{
			std::string inventoryitemnames;
			for (int i = 0; i < slots.size(); i++)
			{
				if (slots[i])
				{
					inventoryitemnames += slots[i]->getName();
					inventoryitemnames += ",";
				}
			}
			return inventoryitemnames;
		}

		bool isFull()
		{
			for (auto i = 0; i < slots.size(); i++)
			{
				if (!slots[i])
					return false;
			}
			return true;
		}

		Item* item_from_invenotry(int position) const { return slots[position].get(); }

		virtual int inventory_size() const { return static_cast<int>(slots.size()); }


	};

	class Backpack_Inventory : public Inventory<Item>
	{
	public:
		Backpack_Inventory(int size) : Inventory<Item>(size) {}
		void compact();
		std::unique_ptr <Item> removeItem(int position) override;
		int inventory_size() const override;
	};

	class Armor_Inventory :public Inventory<Armor>
	{
	public:
		Armor_Inventory() : Inventory<Armor>(5) {}
		bool addItem(std::unique_ptr <Armor> addingitem) override;
		std::unique_ptr <Armor> transferToArmor(std::unique_ptr <Item> item);
		float total_armor_defense() const;
		float total_armor_speed() const;
		std::vector<std::string> getItemNames() const override;
	};

	class Weapon_Inventory : public Inventory<Weapon>
	{
	public:
		Weapon_Inventory() : Inventory<Weapon>(2){}
		bool addItem(std::unique_ptr <Weapon> addingitem) override;
		std::unique_ptr <Weapon> transferToWeapon(std::unique_ptr <Item> item);
		float total_weapon_defense() const;
		float total_weapon_speed() const;
		float total_weapon_attack() const;
		std::vector<std::string> getItemNames() const override;
	};
}