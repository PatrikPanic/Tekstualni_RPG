#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include "Item.h"


namespace oop::projekt
{
	template <typename T>

	// Inventory - genericki spremnik predmeta s unaprijed zadanim brojem mjesta.
	// Predmeti se cuvaju kao unique_ptr, pa spremnik preuzima vlasnistvo nad
	// njima i sam ih oslobada. Zbog toga se spremnik ne moze kopirati, samo
	// premjestati.
	class Inventory
	{
	protected:
		std::vector<std::unique_ptr <T>> slots;
	public:
		Inventory(int size) { slots.resize(size); }

		// Dodaje predmet na prvo slobodno mjesto. Vraca false ako je spremnik pun.
		virtual bool addItem(std::unique_ptr <T> addingitem)
		{
			for (int i = 0; i < static_cast<int>(slots.size()); i++)
			{
				if (!slots[i])
				{
					slots[i] = std::move(addingitem);
					return true;
				}
			}
			// spremnik je pun; ispis je posao sucelja, pa se ovdje samo javlja neuspjeh
			return false;
		}
		// Vadi predmet sa zadane pozicije i prepusta vlasnistvo pozivatelju.
		virtual std::unique_ptr <T> removeItem(int position)
		{
			if (position < 0 || position >= static_cast<int>(slots.size()))
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

		// Vraca nazive predmeta za ispis u izborniku. Izvedene klase ovo
		// nadjacavaju kako bi prazna mjesta prikazale kao slobodne slotove.
		virtual std::vector<std::string> getItemNames() const
		{
			std::vector<std::string> inventoryitemnames;
			for (int i = 0; i < static_cast<int>(slots.size()); i++)
			{
				if (slots[i])
					inventoryitemnames.push_back(slots[i]->getName());
			}
			return inventoryitemnames;
		}

		// Poravnava tekst na zadanu sirinu kako bi se ispis slozio u stupce.
		static std::string padRight(std::string text, int width)
		{
			while (static_cast<int>(text.size()) < width)
				text += " ";
			return text;
		}

		virtual std::string getItemNames_string() const
		{
			std::string inventoryitemnames;
			int number = 1;
			for (int i = 0; i < static_cast<int>(slots.size()); i++)
			{
				if (slots[i])
				{
					inventoryitemnames += "    " + padRight(std::to_string(number) + ".", 5);
					inventoryitemnames += slots[i]->getName();
					inventoryitemnames += "\n";
					number += 1;
				}
			}
			if (inventoryitemnames.empty())
				inventoryitemnames = "    (empty)\n";
			return inventoryitemnames;
		}

		bool isFull() const
		{
			return std::none_of(slots.begin(), slots.end(),
				[](const std::unique_ptr<T>& mjesto) { return mjesto == nullptr; });
		}

		Item* item_from_inventory(int position) const
		{
			if (position < 0 || position >= static_cast<int>(slots.size()))
				return nullptr;
			return slots[position].get();
		}

		virtual int inventory_size() const { return static_cast<int>(slots.size()); }
		// Ukupan broj mjesta, bez obzira na to koliko ih je popunjeno.
		int capacity() const { return static_cast<int>(slots.size()); }


	};

	// Backpack_Inventory - ruksak igraca i protivnika. Za razliku od spremnika
	// oklopa i oruzja, mjesta nisu vezana uz odredenu vrstu predmeta, pa se
	// nakon vadenja predmeta praznine sabijaju kako bi popis u izborniku
	// odgovarao stvarnim mjestima.
	class Backpack_Inventory : public Inventory<Item>
	{
	public:
		Backpack_Inventory(int size) : Inventory<Item>(size) {}
		void compact();
		std::unique_ptr <Item> removeItem(int position) override;
		int inventory_size() const override;
	};

	// Armor_Inventory - opremljeni oklop. Ima tocno pet mjesta, po jedno za
	// svaki ArmorSlot, pa je mjesto predmeta odredeno njegovim slotom, a ne
	// redoslijedom dodavanja.
	class Armor_Inventory :public Inventory<Armor>
	{
	public:
		Armor_Inventory() : Inventory<Armor>(5) {}
		bool addItem(std::unique_ptr <Armor> addingitem) override;
		std::unique_ptr <Armor> transferToArmor(std::unique_ptr <Item> item);
		float total_armor_defense() const;
		float total_armor_speed() const;
		std::vector<std::string> getItemNames() const override;
		std::string getItemNames_string() const override;
	};

	// Weapon_Inventory - opremljeno oruzje. Dva mjesta, lijeva i desna ruka.
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
		std::string getItemNames_string() const override;
	};
}