#include "Inventory.h"

namespace oop::projekt
{
	void Backpack_Inventory::compact()
	{
		if (slots.empty())
			return;

		// Predmete pomicemo prema pocetku i cuvamo njihov redoslijed,
		// tako da se popis u izborniku ne premijesa nakon vadenja predmeta.
		int insert_position = 0;
		for (int i = 0; i < static_cast<int>(slots.size()); i++)
		{
			if (slots[i])
			{
				if (i != insert_position)
				{
					slots[insert_position] = std::move(slots[i]);
					slots[i] = nullptr;
				}
				insert_position += 1;
			}
		}
	}

	// Vadi predmet sa zadanog mjesta i vraca vlasnistvo nad njim pozivatelju.
	// Vraca nullptr ako je mjesto prazno ili izvan granica.
	std::unique_ptr <Item> Backpack_Inventory::removeItem(int position)
	{
		if (position < 0 || position >= static_cast<int>(slots.size()))
		{
			return nullptr;
		}
		else
		{
			std::unique_ptr <Item> returningitem = std::move(slots[position]);
			slots[position] = nullptr;
			compact();
			return returningitem;
		}

	}

	int Backpack_Inventory::inventory_size() const
	{
		return static_cast<int>(std::count_if(slots.begin(), slots.end(),
			[](const std::unique_ptr<Item>& predmet) { return predmet != nullptr; }));
	}

	// Funkcije spremnika oklopa
	// Oklop se uvijek sprema na mjesto koje odgovara njegovom slotu. Ako je to
	// mjesto vec zauzeto, stari se predmet gubi, pa ga pozivatelj mora prije
	// skinuti preko unequip_armor.
	// Oklop se sprema na mjesto koje odgovara njegovom slotu. Vrijednosti
	// ArmorSlot odgovaraju indeksima u spremniku, pa pretvorba u broj daje
	// trazeno mjesto. Zauzeto mjesto se ne prepisuje, jer bi se stari predmet
	// tiho izgubio; pozivatelj ga mora prvo skinuti preko unequip_armor.
	bool Armor_Inventory::addItem(std::unique_ptr <Armor> addingitem)
	{
		if (!addingitem)
			return false;

		int slot = static_cast<int>(addingitem->getArmorSlot());
		if (slot < 0 || slot >= static_cast<int>(slots.size()))
			return false;
		if (slots[slot])
			return false;

		slots[slot] = std::move(addingitem);
		return true;
	}

	// Provjera vrste ide preko dynamic_cast, jer se tek u trenutku pretvorbe
	// zna je li opci predmet doista oklop.
	// Pretvara opci predmet u oklop. Vraca nullptr ako predmet nije oklop,
	// pri cemu se predmet gubi, pa se vrsta mora provjeriti prije poziva.
	std::unique_ptr <Armor> Armor_Inventory::transferToArmor(std::unique_ptr <Item> item)
	{
		Armor* rawptr = dynamic_cast<Armor*>(item.get());
		if (rawptr)
		{
			item.release();
			std::unique_ptr<Armor> armorPtr(rawptr);
			return armorPtr;
		}
		return nullptr;
	}

	float Armor_Inventory::total_armor_defense() const
	{
		return std::accumulate(slots.begin(), slots.end(), 0.0f,
			[](float zbroj, const std::unique_ptr<Armor>& komad)
			{
				return komad ? zbroj + komad->getArmorDefense() : zbroj;
			});
	}
	// Bonus brzine je prosjek opremljenih dijelova, a ne zbroj, kako pet
	// komada oklopa ne bi neuobicajeno ubrzalo igraca.
	// Bonus brzine je prosjek opremljenih dijelova, a ne zbroj, kako pet
	// komada oklopa ne bi neuobicajeno ubrzalo igraca.
	float Armor_Inventory::total_armor_speed() const
	{
		int opremljenih = static_cast<int>(std::count_if(slots.begin(), slots.end(),
			[](const std::unique_ptr<Armor>& komad) { return komad != nullptr; }));
		if (opremljenih == 0)
			return 0.0f;

		float zbroj = std::accumulate(slots.begin(), slots.end(), 0.0f,
			[](float suma, const std::unique_ptr<Armor>& komad)
			{
				return komad ? suma + komad->getArmorSpeed() : suma;
			});
		return zbroj / opremljenih;
	}

	std::vector<std::string> Armor_Inventory::getItemNames() const
	{
		std::vector<std::string> inventoryitemnames;
		for (int i = 0; i < static_cast<int>(slots.size()); i++)
		{
			if (slots[i])
				inventoryitemnames.push_back(slots[i]->getName());
			else
			{
				inventoryitemnames.push_back("empty slot " + toString(static_cast<ArmorSlot>(i)));
			}
		}
		return inventoryitemnames;
	}

	std::string Armor_Inventory::getItemNames_string() const
	{
		std::string inventoryitemnames;
		for (int i = 0; i < static_cast<int>(slots.size()); i++)
		{
			std::string slot_name = toString(static_cast<ArmorSlot>(i));
			std::string item_name = "-";
			if (slots[i])
				item_name = slots[i]->getName();
			inventoryitemnames += "    " + padRight(slot_name, 10) + " : " + item_name + "\n";
		}
		return inventoryitemnames;
	}

	// Funkcije spremnika oruzja
	// Vrijedi isto sto i za oklop: mjesto je odredeno rukom u kojoj se oruzje
	// drzi, a zauzeto mjesto se ne prepisuje.
	bool Weapon_Inventory::addItem(std::unique_ptr <Weapon> addingitem)
	{
		if (!addingitem)
			return false;

		int slot = static_cast<int>(addingitem->getWeaponSlot());
		if (slot < 0 || slot >= static_cast<int>(slots.size()))
			return false;
		if (slots[slot])
			return false;

		slots[slot] = std::move(addingitem);
		return true;
	}

	std::unique_ptr <Weapon> Weapon_Inventory::transferToWeapon(std::unique_ptr <Item> item)
	{
		Weapon* rawptr = dynamic_cast<Weapon*>(item.get());
		if (rawptr)
		{
			item.release();
			std::unique_ptr<Weapon> WeaponPtr(rawptr);
			return WeaponPtr;
		}
		return nullptr;
	}

	float Weapon_Inventory::total_weapon_defense() const
	{
		return std::accumulate(slots.begin(), slots.end(), 0.0f,
			[](float zbroj, const std::unique_ptr<Weapon>& oruzje)
			{
				return oruzje ? zbroj + oruzje->getWeaponDefense() : zbroj;
			});
	}

	// Kao i kod oklopa, brzina je prosjek opremljenih komada.
	float Weapon_Inventory::total_weapon_speed() const
	{
		int opremljenih = static_cast<int>(std::count_if(slots.begin(), slots.end(),
			[](const std::unique_ptr<Weapon>& oruzje) { return oruzje != nullptr; }));
		if (opremljenih == 0)
			return 0.0f;

		float zbroj = std::accumulate(slots.begin(), slots.end(), 0.0f,
			[](float suma, const std::unique_ptr<Weapon>& oruzje)
			{
				return oruzje ? suma + oruzje->getWeaponSpeed() : suma;
			});
		return zbroj / opremljenih;
	}


	float Weapon_Inventory::total_weapon_attack() const
	{
		return std::accumulate(slots.begin(), slots.end(), 0.0f,
			[](float zbroj, const std::unique_ptr<Weapon>& oruzje)
			{
				return oruzje ? zbroj + oruzje->getWeaponAttack() : zbroj;
			});
	}

	std::vector<std::string> Weapon_Inventory::getItemNames() const
	{
		std::vector<std::string> inventoryitemnames;
		for (int i = 0; i < static_cast<int>(slots.size()); i++)
		{
			if (slots[i])
				inventoryitemnames.push_back(slots[i]->getName());
			else
			{
				inventoryitemnames.push_back("empty slot " + toString(static_cast<WeaponSlot>(i)));
			}
		}
		return inventoryitemnames;
	}

	std::string Weapon_Inventory::getItemNames_string() const
	{
		std::string inventoryitemnames;
		for (int i = 0; i < static_cast<int>(slots.size()); i++)
		{
			std::string slot_name = toString(static_cast<WeaponSlot>(i));
			std::string item_name = "-";
			if (slots[i])
				item_name = slots[i]->getName();
			inventoryitemnames += "    " + padRight(slot_name, 10) + " : " + item_name + "\n";
		}
		return inventoryitemnames;
	}

}
