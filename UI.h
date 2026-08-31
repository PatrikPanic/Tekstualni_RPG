#pragma once

#include <iostream>
#include <string>
#include "BattleEngine.h"
#include "Map.h"
#include "Player_Character.h"
#include "Location.h"
#include "Enemyfactory.h"

namespace oop::projekt
{
	class UI
	{
	private:
		int showOptions(std::vector<std::string> options, std::string header = "", std::string header2 = "");
	public:

		enum class Item_choice
		{
			Drop,
			Equipe,
			Unequipe,
			See_stats,
			Back
		};
		void showMessage(std::string message);
		int showMainMenu();
		int gameMenu(Map& map);
		template <typename T> int showInventory(T& invenotry)
		{
			return showOptions(invenotry.getItemNames());
		}
		int showBattleMenu(Player_Character& player, Enemy& enemy);
		void showAttackResult(float dmg, Battle::TurnOwner attacker);
		void showDefendResult();
		void showRunResult(bool success);
		std::string drawMap(Location* current);
		int showMapMenu(Map &mapa);
		int showInventory(Player_Character& player);
		int backpack_inventory(Player_Character& player);
		int armor_inventory(Player_Character& player);
		int weapon_inventory(Player_Character& player);
		Item_choice item_choice(Item& item, bool is_equipped);
	};
}