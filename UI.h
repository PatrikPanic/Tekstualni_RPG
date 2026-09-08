#pragma once

#include <iostream>
#include <string>
#include "BattleEngine.h"
#include "Map.h"
#include "Player_Character.h"
#include "Location.h"
#include "Enemyfactory.h"
#include "Score.h"

namespace oop::projekt
{
	// UI - sve sto se ispisuje na ekran i sav unos s tipkovnice. Odvojen je od
	// ostatka igre, pa logika borbe, inventara i mape ne ovisi o konzoli i moze
	// se testirati bez nje.
	class UI
	{
	private:
		// Prikazuje izbornik i vraca redni broj odabrane stavke. Kretanje je
		// strelicama, potvrda tipkom Enter, a popis je kruzan.
		int showOptions(std::vector<std::string> options, std::string header = "", std::string header2 = "");
	public:

		enum class Item_choice
		{
			Drop,
			Equip,
			Unequip,
			See_stats,
			Back
		};
		void showMessage(std::string message);
		int showMainMenu();
		std::string askPlayerName();
		void showIntro(std::string player_name);
		void showHighscore(Highscore& highscore);
		void showGameOver(bool won, int raw_score, int days, int final_score);
		// Stavke izbornika igre. Vraca se oznaka odabrane radnje, a ne redni
		// broj, jer se "Rest" nudi samo u gradovima pa se redni brojevi
		// mijenjaju ovisno o lokaciji.
		enum class Game_choice
		{
			Travel,
			Inventory,
			Stats,
			Attack,
			Rest,
			Back
		};
		Game_choice gameMenu(Map& map, Player_Character& player, Score& score, bool can_rest);
		template <typename T> int showInventory(T& inventory)
		{
			return showOptions(inventory.getItemNames());
		}
		int showBattleMenu(Player_Character& player, Enemy& enemy);
		int lootMenu(Player_Character& player, Enemy& enemy);
		int lootItemChoice(Item& item);
		int lootBackpackMenu(Player_Character& player, Enemy& enemy);
		int lootDropChoice(Item& item);
		void showAttackResult(float dmg, Battle::TurnOwner attacker);
		void showDefendResult();
		void showRunResult(bool success);
		std::string drawMap(Location* current);
		int showMapMenu(Map &mapa);
		int showInventory();
		int backpack_inventory(Player_Character& player);
		int armor_inventory(Player_Character& player);
		int weapon_inventory(Player_Character& player);
		Item_choice item_choice(Item& item, bool is_equipped);
	};
}