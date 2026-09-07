#pragma once

#include <iostream>
#include <string>
#include "BattleEngine.h"

namespace oop::projekt
{
	// Pomice brojace dok jedan od njih ne prijede prag i vraca vlasnika poteza.
	// Brojac se pritom umanjuje za prag, a ne postavlja na nulu, kako se visak
	// ne bi gubio i kako bi razlika u brzini ostala vidljiva kroz vise poteza.
	Battle::TurnOwner Battle::processTick()
	{
		while (true)
		{
			player_turnmetar += player->getSpeed();
			enemy_turnmetar += enemy->getSpeed();
			if (player_turnmetar >= 1 || enemy_turnmetar >= 1)
			{
				if (player_turnmetar < enemy_turnmetar)
				{
					enemy_turnmetar -= 1;
					return TurnOwner::Enemy;
				}
				else
				{
					player_turnmetar -= 1;
					return TurnOwner::Player;
				}
			}
		}
	}

	float Battle::performAttack(TurnOwner vlasnik)
	{
		float dmg;
		float result;
		switch (vlasnik)
		{
		case(TurnOwner::Player):
		{
			dmg = player->deal_dmg();
			result = enemy->take_dmg(dmg);
			return result;
		}
		case(TurnOwner::Enemy):
		{
			if (player_defending)
			{
				dmg = enemy->deal_dmg() - player->deal_dmg() / 2;
				result = player->take_dmg(dmg);
				player_defending = false;
				return result;
			}
			dmg = enemy->deal_dmg();
			result = player->take_dmg(dmg);
			player_defending = false;
			return result;
		}
		default:
			return 0;
		}
	}

	void Battle::performDefense()
	{
		player_defending = true;
	}

	bool Battle::performEscape()
	{
		return std::rand() % 2 == 0;
	}

	bool Battle::isBattleOver()
	{
		return (!player->isAlive() || !enemy->isAlive());
	}

	
}