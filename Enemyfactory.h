#pragma once
#include <iostream>
#include "Player_Character.h"
#include "Location.h"
#include <random>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <memory>

namespace oop::projekt
{
	template <typename T>
	std::unique_ptr<Item> pickRandomItem(Rarity rarity)
	{
		int id = ItemTemplateStore<T>::getRandomId(rarity);
		if (id == -1) return nullptr;
		return ItemTemplateStore<T>::createItem(id);
	}

	// EnemyFactory - stvara protivnike prilagodene lokaciji. Imena se ucitavaju
	// iz datoteke i razvrstana su po tipu lokacije, dok se statistike racunaju
	// iz njezine tezine. Uz obicne protivnike stvara i dva srednja bossa te
	// zavrsnog bossa, cije su statistike fiksne.
	class EnemyFactory
	{
	private:
		// Imena su razvrstana po tipu lokacije. Vektor je ovdje prirodniji od
		// mape, jer se imenima pristupa iskljucivo po nasumicnom rednom broju.
		std::vector<std::string> city;
		std::vector<std::string> forest;
		std::vector<std::string> road;
		std::vector<std::string> cave;

		std::string pickRandomName(const std::vector<std::string>& names) const;
	public:
		Enemy createEnemy(Location* location);
		Enemy createBoss();
		Enemy createMidBoss(Location* location);
		void fillEnemyBackpack(Enemy& enemy);
		void loadFromFile();
		EnemyFactory() { loadFromFile(); }
	};
}
