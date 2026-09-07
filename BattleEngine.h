#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include "Player_Character.h"

namespace oop::projekt
{
	// Battle - vodi jednu borbu izmedu igraca i protivnika. Redoslijed poteza
	// odreduje se sustavom brojaca ("turn meter"): svakom otkucaju oba
	// sudionika dodaje se njihova brzina, a potez dobiva onaj ciji brojac prvi
	// prijede prag. Brzi lik zato moze odigrati vise poteza zaredom.
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
		float performAttack(TurnOwner vlasnik);
		void performDefense();
		bool performEscape();
		bool isBattleOver();
	};
}