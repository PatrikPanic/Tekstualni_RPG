#include "Inventory.h"

namespace oop::projekt
{
	void Backpack_Inventory::compact()
	{
		if (slots.empty()) 
			return;

		for (int i = 0; i < static_cast<int>(slots.size()) - 1; i++)
		{
			if (!slots[i])
			{
				for (int j = static_cast<int>(slots.size()) - 1; j != i; j--)
				{
					if (slots[j])
					{
						slots[i] = std::move(slots[j]);
						slots[j] = nullptr;
						break;
					}
				}
			}
		}
	}

	std::unique_ptr <Item> Backpack_Inventory::removeItem(int position)
	{
		if (position >= slots.size())
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
		int size = 0;
		for (int i = 0; i < slots.size(); i++)
		{
			if (slots[i])
				size++;
		}
		return size;
	}

	//funkcije of Armor invenotrija
	bool Armor_Inventory::addItem(std::unique_ptr <Armor> addingitem)
	{
		switch(addingitem->getArmorSlot())
		{
		case ArmorSlot::Helmet:
			slots[0] = std::move(addingitem);
			return true;
		case ArmorSlot::Chest:
			slots[1] = std::move(addingitem);
			return true;
		case ArmorSlot::Gloves:
			slots[2] = std::move(addingitem);
			return true;
		case  ArmorSlot::Pants:
			slots[3] = std::move(addingitem);
			return true;
		case ArmorSlot::Boots:
			slots[4] = std::move(addingitem);
			return true;
		default:
			return false;
		}
	}

	//dinamic cast iz itema u amror unique_pointer
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
		float total_armor = 0;
		for (auto& po_armor : slots)
		{
			if (po_armor)
			{
				total_armor = total_armor + po_armor->getArmorDefense();
			}
		}
		return total_armor;
	}
	float Armor_Inventory::total_armor_speed() const
	{
		float total_speed = 0;
		int i = 0;
		for (auto& po_armor : slots)
		{
			if (po_armor)
			{
				total_speed = total_speed + po_armor->getArmorSpeed();
				i++;
			}
		}
		if (i != 0)
			return total_speed/i;
		return 0;
	}

	std::vector<std::string> Armor_Inventory::getItemNames() const
	{
		std::vector<std::string> inventoryitemnames;
		for (int i = 0; i < slots.size(); i++)
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

	//funckije od Weapon invenotrija
	bool Weapon_Inventory::addItem(std::unique_ptr <Weapon> addingitem)
	{
		switch (addingitem->getWeaponSlot())
		{
		case WeaponSlot::Left_hand:
			slots[0] = std::move(addingitem);
			return true;
		case WeaponSlot::Right_hand:
			slots[1] = std::move(addingitem);
			return true;
		default:
			return false;
		}
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
		float total_armor = 0;
		for (auto& po_weapon : slots)
		{
			if (po_weapon)
			{
				total_armor = total_armor + po_weapon->getWeaponDefense();
			}
		}
		return total_armor;
	}

	float Weapon_Inventory::total_weapon_speed() const
	{
		float total_speed = 0;
		int i = 0;
		for (auto& po_weapon : slots)
		{
			if (po_weapon)
			{
				total_speed = total_speed + po_weapon->getWeaponSpeed();
				i++;
			}
		}
		if (i != 0)
			return total_speed / i;
		return 0;
	}


	float Weapon_Inventory::total_weapon_attack() const
	{
		float total_attack = 0;
		for (auto& po_weapon : slots)
		{
			if (po_weapon)
			{
				total_attack = total_attack + po_weapon->getWeaponAttack();
			}
		}
		return total_attack;
	}

	std::vector<std::string> Weapon_Inventory::getItemNames() const
	{
		std::vector<std::string> inventoryitemnames;
		for (int i = 0; i < slots.size(); i++)
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

}