#include "Game.h"
#include <iostream>
#include <string>

namespace oop::projekt
{
	Game::BattleResult Game::run_Battle(Enemy& enemy)
	{
		Battle battle(&player, &enemy);
		UI ui;
		Battle::TurnOwner turnowner;
		while (true)
		{
			turnowner = battle.processTick();
			switch (turnowner)
			{
			case(Battle::TurnOwner::Player):
			{
				switch (ui.showBattleMenu(player, enemy))
				{
				case(0):
				{
					ui.showAttackResult(battle.preformAttack(turnowner), turnowner);
					if (battle.isBattleOver())
						return player.isAlive() ? Game::BattleResult::PlayerWon : Game::BattleResult::PlayerLost;
					break;
				}
				case(1):
				{
					battle.preformDefense();
					ui.showDefendResult();
					break;
				}
				case(2):
				{
					bool escape = battle.preformEscape();
					ui.showRunResult(escape);
					if (escape)
						return Game::BattleResult::PlayerEscaped;
					break;
				}
				}
				break;
			}
			case(Battle::TurnOwner::Enemy):
			{
				ui.showAttackResult(battle.preformAttack(turnowner), turnowner);
				if (battle.isBattleOver())
					return player.isAlive() ? Game::BattleResult::PlayerWon : Game::BattleResult::PlayerLost;
				break;
			}
			}
		}
	}

	void Game::maptravle()
	{
		int choice = ui.showMapMenu(map);
		std::vector<Location*> available = map.getAvailableLocations();
		if (available.size() > choice)
		{
			map.moveTo(available[choice]);
			int chance = 20 + Location::toInt(map.getCurrentLocation()->getlocationdifficulty()) * 15;
			if (rand() % 100 < chance)
			{
				startEncounter();
			}
		}
	}

	void Game::inventory_control()
	{
		InventoryEngine inventoryEngine(&player);
		bool invenotry = true;

		while (invenotry)
		{
			switch (ui.showInventory(player))
			{
			case(0):
			{
				inventoryEngine.openBackpack(ui);
				break;
			}
			case(1):
			{
				inventoryEngine.openArmor(ui);
				break;
			}
			case(2):
			{
				inventoryEngine.openWeapon(ui);
				break;
			}
			case(3):
			{
				invenotry = false;
				break;
			}
			default:
			{
				invenotry = false;
				break;
			}
			}
		}
	}

	void Game::startEncounter()
	{
		Enemy enemy = enemyfac.EnemyFactory(map.getCurrentLocation());
		BattleResult result = run_Battle(enemy);
		switch (result)
		{
		case(BattleResult::PlayerWon):
			ui.showMessage("Pobijedio si!");
			break;
		case(BattleResult::PlayerLost):
			ui.showMessage("Umro si...");
			break;
		case(BattleResult::PlayerEscaped):
			ui.showMessage("Pobjegao si iz borbe.");
			break;
		}
	}

	void Game::gameStart()
	{
		int game = true;
		int menu = true;
		while (game)
		{
			switch (ui.showMainMenu())
			{
			case(0):
			{
				menu = true;
				while (menu)
					switch (ui.gameMenu(map))
					{
					case(0):
					{
						maptravle();
						break;
					}
					case(1):
					{
						inventory_control();
						break;
					}
					case(2):
					{
						ui.showMessage(player.showstats());
						break;
					}
					case(3):
					{
						startEncounter();
						break;
					}
					case(4):
					{
						menu = false;
						break;
					}
					}
				break;
			}
			case(1):
			{
				game = false;
				break;
			}
			default:
				break;
			}
		}
	}
}