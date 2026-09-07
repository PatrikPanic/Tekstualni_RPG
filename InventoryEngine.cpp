#include "InventoryEngine.h"

namespace oop::projekt
{
	InventoryEngine::InventoryEngine(Player_Character* player) : player(player) {}

	void InventoryEngine::openBackpack(UI& ui)
	{
		int choice;
		bool in_inventory = true;
		bool itemcc;

		while (in_inventory)
		{
			choice = ui.backpack_inventory(*player);
			if (player->get_backpack_inventory().inventory_size() > choice)
			{
				Item* chosen_item = player->get_backpack_inventory().item_from_inventory(choice);
				if (chosen_item)
				{
					itemcc = true;
					while (itemcc)
					{
						switch (ui.item_choice(*chosen_item, false))
						{
						case(UI::Item_choice::Drop):
						{
							player->get_backpack_inventory().removeItem(choice);
							itemcc = false;
							break;
						}
						case(UI::Item_choice::Equip):
						{
							player->equip(choice);
							itemcc = false;
							break;
						}
						case(UI::Item_choice::See_stats):
						{
							ui.showMessage(chosen_item->seeStats());
							break;
						}
						case(UI::Item_choice::Back):
						{
							itemcc = false;
							break;
						}
						default:
							break;
						}
					}
				}
			}
			else
			{
				in_inventory = false;
			}
		}
	}

	void InventoryEngine::openArmor(UI& ui)
	{
		int choice;
		bool in_inventory = true;
		bool itemcc;

		while (in_inventory)
		{
			choice = ui.armor_inventory(*player);
			if (player->get_armor_inventory().inventory_size() > choice)
			{
				Item* chosen_item = player->get_armor_inventory().item_from_inventory(choice);
				if (chosen_item)
				{
					itemcc = true;
					while (itemcc)
					{
						switch (ui.item_choice(*chosen_item, true))
						{
						case(UI::Item_choice::Unequip):
						{
							if (player->get_backpack_inventory().isFull())
							{
								ui.showMessage("Your backpack is full.");
								break;
							}
							else
							{
								player->unequip_armor(choice);
								itemcc = false;
								break;
							}
						}
						case(UI::Item_choice::Drop):
						{
							player->get_armor_inventory().removeItem(choice);
							player->recalculate_stats();
							itemcc = false;
							break;
						}
						case(UI::Item_choice::See_stats):
						{
							ui.showMessage(chosen_item->seeStats());
							break;
						}
						case(UI::Item_choice::Back):
						{
							itemcc = false;
							break;
						}
						default:
							break;
						}
					}
				}
			}
			else
			{
				in_inventory = false;
			}
		}
	}

	void InventoryEngine::openWeapon(UI& ui)
	{
		int choice;
		bool in_inventory = true;
		bool itemcc;

		while (in_inventory)
		{
			choice = ui.weapon_inventory(*player);
			if (player->get_weapon_inventory().inventory_size() > choice)
			{
				Item* chosen_item = player->get_weapon_inventory().item_from_inventory(choice);
				if (chosen_item)
				{
					itemcc = true;
					while (itemcc)
					{
						switch (ui.item_choice(*chosen_item, true))
						{
						case(UI::Item_choice::Unequip):
						{
							if (player->get_backpack_inventory().isFull())
							{
								ui.showMessage("Your backpack is full.");
								break;
							}
							else
							{
								player->unequip_weapon(choice);
								itemcc = false;
								break;
							}
						}
						case(UI::Item_choice::Drop):
						{
							player->get_weapon_inventory().removeItem(choice);
							player->recalculate_stats();
							itemcc = false;
							break;
						}
						case(UI::Item_choice::See_stats):
						{
							ui.showMessage(chosen_item->seeStats());
							break;
						}
						case(UI::Item_choice::Back):
						{
							itemcc = false;
							break;
						}
						default:
							break;
						}
					}
				}
			}
			else
			{
				in_inventory = false;
			}
		}
	}
}