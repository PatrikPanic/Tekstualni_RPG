#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include "Player_Character.h"

namespace oop::projekt
{
	class Battle
	{
	private:
		float player_turnmetar;
		float enemy_turnmetar;
		Player_Character* player;
		Enemy* enemy;
		bool player_defending = false;

	public:
		enum class TurnOwner
		{
			Player,
			Enemy
		};

		Battle(Player_Character* player, Enemy* enemy) { player_turnmetar = 0, enemy_turnmetar = 0,
			this->player = player, this->enemy = enemy; }
		TurnOwner processTick();
		float preformAttack(TurnOwner vlasnik);
		void preformDefense();
		bool preformEscape();
		bool isBattleOver();
	};
}