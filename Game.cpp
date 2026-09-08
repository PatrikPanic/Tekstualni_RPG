#include "Game.h"
#include <algorithm>
#include <iostream>
#include <string>

namespace oop::projekt
{
	// Vodi jednu borbu do kraja i vraca njezin ishod. Bodovanje i plijen nisu
	// ovdje, nego kod pozivatelja, jer se razlikuju za obicne protivnike,
	// srednje bossove i zavrsnog bossa.
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
					ui.showAttackResult(battle.performAttack(turnowner), turnowner);
					if (battle.isBattleOver())
						return player.isAlive() ? Game::BattleResult::PlayerWon : Game::BattleResult::PlayerLost;
					break;
				}
				case(1):
				{
					battle.performDefense();
					ui.showDefendResult();
					break;
				}
				case(2):
				{
					bool escape = battle.performEscape();
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
				ui.showAttackResult(battle.performAttack(turnowner), turnowner);
				if (battle.isBattleOver())
					return player.isAlive() ? Game::BattleResult::PlayerWon : Game::BattleResult::PlayerLost;
				break;
			}
			}
		}
	}

	// Putovanje na susjednu lokaciju. Redoslijed provjera je bitan: zavrsna
	// lokacija je zakljucana dok oba srednja bossa nisu porazena, boss borba
	// ima prednost pred slucajnim susretom, a srednji boss se pojavljuje samo
	// prvi put na svojoj lokaciji.
	void Game::mapTravel()
	{
		int choice = ui.showMapMenu(map);
		std::vector<Location*> available = map.getAvailableLocations();
		if (choice >= 0 && choice < static_cast<int>(available.size()))
		{
			Location* destination = available[choice];

			// zavrsna lokacija je zakljucana dok oba srednja bossa nisu porazena
			if (destination == map.getFinalLocation() && !canEnterFinalLocation())
			{
				ui.showMessage("The entrance to Voidspire Cave is sealed by black magic.\n"
					"The seal is held by the Mirkwood Warden and the Thornwood Butcher.\n"
					"Defeat them both before you come back here.");
				return;
			}

			map.moveTo(destination);

			if (map.isAtFinalLocation())
			{
				startBossFight();
				return;
			}

			// srednji boss ceka samo prvi put na svojoj lokaciji
			if (map.isMidbossLocation(map.getCurrentLocation()) &&
				!isMidbossDefeated(map.getCurrentLocation()->getLocationName()))
			{
				startMidbossFight();
				return;
			}

			int chance = 20 + Location::toInt(map.getCurrentLocation()->getlocationdifficulty()) * 15;
			if (Random::next(0, 99) < chance)
			{
				startEncounter();
			}
		}
	}

	void Game::inventory_control()
	{
		InventoryEngine inventoryEngine(&player);
		bool inventory = true;

		while (inventory)
		{
			switch (ui.showInventory())
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
				inventory = false;
				break;
			}
			default:
			{
				inventory = false;
				break;
			}
			}
		}
	}

	// Pregled vlastitog ruksaka tijekom uzimanja plijena. Sluzi da igrac moze
	// osloboditi mjesto kad je ruksak pun, bez napustanja ekrana plijena.
	void Game::manageBackpack(Enemy& enemy)
	{
		bool managing = true;
		while (managing)
		{
			std::vector<std::string> nazivi = player.get_backpack_inventory().getItemNames();
			if (nazivi.empty())
			{
				ui.showMessage("Your backpack is empty.");
				return;
			}

			int choice = ui.lootBackpackMenu(player, enemy);
			if (choice < 0 || choice >= static_cast<int>(nazivi.size()))
			{
				managing = false;
				break;
			}

			Item* selected = player.get_backpack_inventory().item_from_inventory(choice);
			if (!selected)
				continue;

			if (ui.lootDropChoice(*selected) != 0)
				continue;

			std::unique_ptr<Item> dropped = player.get_backpack_inventory().removeItem(choice);
			if (dropped)
				ui.showMessage("You dropped: " + dropped->getName());
		}
	}

	// Nakon pobjede igrac pregledava ruksak protivnika i uzima sto zeli.
	void Game::lootEnemy(Enemy& enemy)
	{
		bool looting = true;
		while (looting)
		{
			std::vector<std::string> loot = enemy.get_backpack_inventory().getItemNames();
			if (loot.empty())
			{
				ui.showMessage("There is nothing left to take.");
				return;
			}

			int choice = ui.lootMenu(player, enemy);
			int backpack_option = static_cast<int>(loot.size());

			// pretposljednja stavka otvara vlastiti ruksak, posljednja izlazi
			if (choice == backpack_option)
			{
				manageBackpack(enemy);
				continue;
			}
			if (choice < 0 || choice > backpack_option)
			{
				looting = false;
				break;
			}

			Item* selected = enemy.get_backpack_inventory().item_from_inventory(choice);
			if (!selected)
				continue;

			if (ui.lootItemChoice(*selected) != 0)
				continue;

			std::unique_ptr<Item> taken = enemy.get_backpack_inventory().removeItem(choice);
			if (!taken)
				continue;

			// takeItem baca iznimku ako je ruksak pun. Predmet se u tom slucaju
			// vraca protivniku, kako ne bi nestao iz igre.
			std::string taken_name = taken->getName();
			try
			{
				player.takeItem(taken);
				ui.showMessage("You took: " + taken_name);
			}
			catch (const InventoryFullException& greska)
			{
				enemy.get_backpack_inventory().addItem(std::move(taken));
				ui.showMessage(std::string(greska.what()) +
					"\n\nOpen \"My backpack\" from this screen to drop something first.");
			}
		}
	}

	void Game::startEncounter()
	{
		Enemy enemy = enemyfac.createEnemy(map.getCurrentLocation());
		BattleResult result = run_Battle(enemy);
		switch (result)
		{
		case(BattleResult::PlayerWon):
		{
			score.addEnemyScore(enemy, map.getCurrentLocation());
			ui.showMessage("You won the fight! Score: " + std::to_string(score.getScore()));
			lootEnemy(enemy);
			break;
		}
		case(BattleResult::PlayerLost):
		{
			endGame(false);
			break;
		}
		case(BattleResult::PlayerEscaped):
		{
			ui.showMessage("You escaped the fight. No score awarded.");
			break;
		}
		}
	}

	bool Game::isMidbossDefeated(std::string location_name)
	{
		return std::find(defeated_midbosses.begin(), defeated_midbosses.end(),
			location_name) != defeated_midbosses.end();
	}

	// U Voidspire se moze uci tek kad su oba srednja bossa porazena.
	bool Game::canEnterFinalLocation()
	{
		std::vector<Location*> midbosses = map.getMidbossLocations();
		return std::all_of(midbosses.begin(), midbosses.end(),
			[this](Location* lokacija)
			{
				return isMidbossDefeated(lokacija->getLocationName());
			});
	}

	// Srednji boss cuva prolaz. Pojavljuje se samo prvi put na toj lokaciji.
	void Game::startMidbossFight()
	{
		Location* location = map.getCurrentLocation();
		Enemy midboss = enemyfac.createMidBoss(location);
		ui.showMessage("Your path is blocked by the " + midboss.getName() + ".\nThis is no ordinary enemy.");

		BattleResult result = run_Battle(midboss);
		switch (result)
		{
		case(BattleResult::PlayerWon):
		{
			defeated_midbosses.push_back(location->getLocationName());
			score.addEnemyScore(midboss, location);
			score.addMidbossScore();
			ui.showMessage("You defeated the " + midboss.getName() + "!\nThe seal on Voidspire weakens.\nScore: " +
				std::to_string(score.getScore()));
			lootEnemy(midboss);
			break;
		}
		case(BattleResult::PlayerLost):
		{
			endGame(false);
			break;
		}
		case(BattleResult::PlayerEscaped):
		{
			ui.showMessage("You fled. The " + midboss.getName() + " still guards the way.");
			break;
		}
		}
	}

	// Zavrsna borba u Voidspire Caveu. Pobjeda zavrsava igru.
	void Game::startBossFight()
	{
		ui.showMessage("Voidspire Cave. At the end of the hall stands the Voidspire Overlord,\nand behind him, in chains, Mira.");
		Enemy boss = enemyfac.createBoss();
		BattleResult result = run_Battle(boss);
		switch (result)
		{
		case(BattleResult::PlayerWon):
		{
			score.addEnemyScore(boss, map.getCurrentLocation());
			lootEnemy(boss);
			endGame(true);
			break;
		}
		case(BattleResult::PlayerLost):
		{
			endGame(false);
			break;
		}
		case(BattleResult::PlayerEscaped):
		{
			ui.showMessage("You fled. Mira is still in chains.");
			break;
		}
		}
	}

	// Odmor u gradu vraca sve bodove zivota, ali trosi jedan dan.
	// Svaki dan umanjuje konacni rezultat, pa se odmor ne isplati bez potrebe.
	void Game::restInCity()
	{
		if (map.getCurrentLocation()->getLocationType() != Location::LocationType::City)
		{
			ui.showMessage("You can only rest in a city.");
			return;
		}

		player.heal(player.getMaxHp());
		score.nextDay();
		ui.showMessage("You rest at the inn and recover completely.\n\nHP: " +
			std::to_string((int)player.gethp()) + " / " + std::to_string((int)player.getMaxHp()) +
			"\nDay: " + std::to_string(score.getDays()) +
			"\nScore penalty so far: -" + std::to_string(score.getDayPenalty()));
	}

	// Zavrsava partiju, upisuje konacni rezultat u listu najboljih i prikazuje
	// zavrsni ekran. Poziva se i kod pobjede i kod poraza.
	void Game::endGame(bool won)
	{
		if (won)
		{
			boss_defeated = true;
			score.addBossScore();
			score.addRemainingHpScore(player.gethp());
		}
		else
		{
			player_dead = true;
		}
		highscore.addScore(player.getName(), score.getFinalScore());
		ui.showGameOver(won, score.getScore(), score.getDays(), score.getFinalScore());
		ui.showHighscore(highscore);
	}

	// Priprema novu partiju. Lik se stvara iznova, jer se nakon zavrsene igre
	// ne smiju zadrzati stara oprema, bodovi ni porazeni bossovi.
	void Game::newGame()
	{
		std::string name = ui.askPlayerName();
		ui.showIntro(name);
		player = Player_Character(name, 100, 20, 10, 1.0, 10);
		map.resetToStart();
		score.reset();
		player_dead = false;
		boss_defeated = false;
		defeated_midbosses.clear();
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
				newGame();
				menu = true;
				while (menu)
				{
					// "Rest" se nudi samo u gradovima. Izbornik vraca oznaku
					// radnje, pa redni brojevi stavki nisu bitni.
					bool can_rest = (map.getCurrentLocation()->getLocationType() == Location::LocationType::City);

					switch (ui.gameMenu(map, player, score, can_rest))
					{
					case(UI::Game_choice::Travel):
					{
						mapTravel();
						break;
					}
					case(UI::Game_choice::Inventory):
					{
						inventory_control();
						break;
					}
					case(UI::Game_choice::Stats):
					{
						ui.showMessage(player.showStats());
						break;
					}
					case(UI::Game_choice::Attack):
					{
						startEncounter();
						break;
					}
					case(UI::Game_choice::Rest):
					{
						restInCity();
						break;
					}
					case(UI::Game_choice::Back):
					{
						menu = false;
						break;
					}
					}
					// igra zavrsava smrcu igraca ili porazom zavrsnog bossa
					if (player_dead || boss_defeated)
						menu = false;
				}
				break;
			}
			case(1):
			{
				ui.showHighscore(highscore);
				break;
			}
			case(2):
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