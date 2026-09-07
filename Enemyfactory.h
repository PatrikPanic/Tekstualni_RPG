#pragma once
#include <iostream>
#include "Player_Character.h"
#include "Location.h"
#include <random>
#include <fstream>
#include <map>
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
		std::map<int, std::string> city;
		std::map<int, std::string> forest;
		std::map<int, std::string> road;
		std::map<int, std::string> cave;

		std::string pickRandomName(const std::map<int, std::string>& names) const;
	public:
		Enemy createEnemy(Location* location);
		Enemy createBoss();
		Enemy createMidBoss(Location* location);
		void fillEnemyBackpack(Enemy& enemy);
		void loadFromFile();
		EnemyFactory() { loadFromFile(); }
	};
}
