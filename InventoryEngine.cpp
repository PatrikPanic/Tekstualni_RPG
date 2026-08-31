#include "InventoryEngine.h"

namespace oop::projekt
{
	InventoryEngine::InventoryEngine(Player_Character* player) : player(player) {}

	void InventoryEngine::openBackpack(UI& ui)
	{
		int choice;
		bool ininvenotry = true;
		bool itemcc;

		while (ininvenotry)
		{
			choice = ui.backpack_inventory(*player);
			if (player->get_backpack_inventory().inventory_size() > choice)
			{
				Item* chosen_item = player->get_backpack_inventory().item_from_invenotry(choice);
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
						case(UI::Item_choice::Equipe):
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
				ininvenotry = false;
			}
		}
	}

	void InventoryEngine::openArmor(UI& ui)
	{
		int choice;
		bool ininvenotry = true;
		bool itemcc;

		while (ininvenotry)
		{
			choice = ui.armor_inventory(*player);
			if (player->get_armor_inventory().inventory_size() > choice)
			{
				Item* chosen_item = player->get_armor_inventory().item_from_invenotry(choice);
				if (chosen_item)
				{
					itemcc = true;
					while (itemcc)
					{
						switch (ui.item_choice(*chosen_item, true))
						{
						case(UI::Item_choice::Unequipe):
						{
							if (player->get_backpack_inventory().isFull())
							{
								ui.showMessage("Backpack invenotry is full");
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
							player->recalcute_stats();
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
				ininvenotry = false;
			}
		}
	}

	void InventoryEngine::openWeapon(UI& ui)
	{
		int choice;
		bool ininvenotry = true;
		bool itemcc;

		while (ininvenotry)
		{
			choice = ui.weapon_inventory(*player);
			if (player->get_weapon_inventory().inventory_size() > choice)
			{
				Item* chosen_item = player->get_weapon_inventory().item_from_invenotry(choice);
				if (chosen_item)
				{
					itemcc = true;
					while (itemcc)
					{
						switch (ui.item_choice(*chosen_item, true))
						{
						case(UI::Item_choice::Unequipe):
						{
							if (player->get_backpack_inventory().isFull())
							{
								ui.showMessage("Backpack invenotry is full");
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
							player->recalcute_stats();
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
				ininvenotry = false;
			}
		}
	}
}