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

	class Enemyfactory
	{
	private:
		std::map<int, std::string> city;
		std::map<int, std::string> forest;
		std::map<int, std::string> road;
		std::map<int, std::string> cave;
	public:
		Enemy EnemyFactory(Location* location);
		void fillenemybackpack(Enemy& enemy);
		void loadFromFile();
		Enemyfactory() { loadFromFile(); }
	};
}
