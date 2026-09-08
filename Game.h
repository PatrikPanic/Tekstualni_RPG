#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "BattleEngine.h"
#include "UI.h"
#include "InventoryEngine.h"
#include "Enemyfactory.h"
#include "Score.h"

namespace oop::projekt
{
	// Game - povezuje sve dijelove igre i vodi glavnu petlju. Cuva stanje jedne
	// partije: lika, mapu, bodove i podatak o tome koji su bossovi vec
	// porazeni. Sam ne ispisuje nista, nego se za to obraca klasi UI.
	class Game
	{
	protected:
		Player_Character player;
		Map map;
		UI ui;
		EnemyFactory enemyfac;
		Score score;
		Highscore highscore;
		bool player_dead;
		bool boss_defeated;
		std::vector<std::string> defeated_midbosses;

	public:
		enum class BattleResult
		{
			PlayerWon,
			PlayerLost,
			PlayerEscaped
		};
		Game() : player("Player", 100, 20, 10, 1.0, 10)
		{
			player_dead = false;
			boss_defeated = false;
			ItemTemplateStore<Item>::loadFromFile("junk.txt");
			ItemTemplateStore<Armor>::loadFromFile("armor.txt");
			ItemTemplateStore<Weapon>::loadFromFile("weapon.txt");
			gameStart();
		}
		BattleResult run_Battle(Enemy& enemy);
		void mapTravel();
		void inventory_control();
		void startEncounter();
		void lootEnemy(Enemy& enemy);
		void manageBackpack(Enemy& enemy);
		void startBossFight();
		void startMidbossFight();
		bool isMidbossDefeated(std::string location_name);
		bool canEnterFinalLocation();
		void restInCity();
		void endGame(bool won);
		void newGame();
		void gameStart();
	};
}
