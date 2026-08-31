#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include "BattleEngine.h"
#include "UI.h"
#include "InventoryEngine.h"
#include "Enemyfactory.h"

namespace oop::projekt
{
	class Game
	{
	protected:
		Player_Character player;
		Map map;
		UI ui;
		Enemyfactory enemyfac;

	public:
		enum class BattleResult
		{
			PlayerWon,
			PlayerLost,
			PlayerEscaped
		};
		Game() : player("Patrik", 1, 100, 20, 10, 1.0, 10)
		{
			ItemTemplateStore<Item>::loadFromFile("Item.txt");
			ItemTemplateStore<Armor>::loadFromFile("Armor.txt");
			ItemTemplateStore<Weapon>::loadFromFile("Weapon.txt");
			gameStart();
		}
		BattleResult run_Battle(Enemy& enemy);
		void maptravle();
		void inventory_control();
		void startEncounter();
		void gameStart();
	};
}