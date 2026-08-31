#include "Enemyfactory.h"

namespace oop::projekt
{
	Enemy Enemyfactory::EnemyFactory(Location* location)
	{
		int lvl = 1 + Location::toInt(location->getlocationdifficulty()) * 3 + (rand() % 3);
		int max_hp = 40 + lvl * 8;
		float base_attack = 6 + lvl * 1.5;
		float base_defense = 2 + lvl * 0.8;
		float base_speed = 0.7 + lvl * 0.03;
		std::string name;
		int random;
		switch (location->getLocationType())
		{
		case(Location::LocationType::City):
		{
			random = rand() % static_cast<int>(city.size());
			name = city[random];
			break;
		}
		case(Location::LocationType::Forest):
		{
			random = rand() % static_cast<int>(forest.size());
			name = forest[random];
			break;
		}
		case(Location::LocationType::Road):
		{
			random = rand() % static_cast<int>(road.size());
			name = road[random];
			break;
		}
		case(Location::LocationType::Cave):
		{
			random = rand() % static_cast<int>(cave.size());
			name = cave[random];
			break;
		}
		default:
		{
			random = rand() % static_cast<int>(city.size());
			name = city[random];
			break;
		}
		}
		Enemy enemy(name, lvl, max_hp, base_attack, base_defense, base_speed, 5);
		fillenemybackpack(enemy);
		return enemy;
	}

	void Enemyfactory::fillenemybackpack(Enemy& enemy)
	{
		int num_items = 1 + (enemy.getLVL() / 4);
		if (num_items > 5)
		{
			num_items = 5;
		}
		float common_weight = std::max(100 - enemy.getLVL() * 7, 5);
		float rear_weight = 40;
		float epic_weight = enemy.getLVL() * 4;
		float legendery_weight = std::max(enemy.getLVL() * 2 - 10, 0);
		float total = common_weight + rear_weight + epic_weight + legendery_weight;
		int roll_rearity,roll_store;
		for (int i = 0; i < num_items; i++)
		{
			roll_rearity = rand() % static_cast<int>(total);
			if (roll_rearity < common_weight)
			{
				roll_store = rand() % 3;
				std::unique_ptr <Item> item;
				switch (roll_store)
				{
				case(0):
				{
					item = pickRandomItem<Item>(Rarity::Common);
					if (item)
					{
						enemy.get_backpack_inventory().addItem(std::move(item));
					}
					break;
				}
				case(1):
				{
					item = pickRandomItem<Armor>(Rarity::Common);
					if (item)
					{
						enemy.get_backpack_inventory().addItem(std::move(item));
					}
					break;
				}
				case(2):
				{
					item = pickRandomItem<Weapon>(Rarity::Common);
					if (item)
					{
						enemy.get_backpack_inventory().addItem(std::move(item));
					}
					break;
				}
				}
			}
			else {
				roll_rearity -= common_weight;
				if (roll_rearity < rear_weight)
				{
					roll_store = rand() % 2;
					std::unique_ptr <Item> item;
					switch (roll_store)
					{
					case(0):
					{
						item = pickRandomItem<Armor>(Rarity::Rear);
						if (item)
						{
							enemy.get_backpack_inventory().addItem(std::move(item));
						}
						break;
					}
					case(1):
					{
						item = pickRandomItem<Weapon>(Rarity::Rear);
						if (item)
						{
							enemy.get_backpack_inventory().addItem(std::move(item));
						}
						break;
					}
					}
				}
				else {
					roll_rearity -= rear_weight;
					if (roll_rearity < epic_weight)
					{
						roll_store = rand() % 2;
						std::unique_ptr <Item> item;
						switch (roll_store)
						{
						case(0):
						{
							item = pickRandomItem<Armor>(Rarity::Epic);
							if (item)
							{
								enemy.get_backpack_inventory().addItem(std::move(item));
							}
							break;
						}
						case(1):
						{
							item = pickRandomItem<Weapon>(Rarity::Epic);
							if (item)
							{
								enemy.get_backpack_inventory().addItem(std::move(item));
							}
							break;
						}
						}
					}
					else
					{
						roll_store = rand() % 2;
						std::unique_ptr <Item> item;
						switch (roll_store)
						{
						case(0):
						{
							item = pickRandomItem<Armor>(Rarity::Legendery);
							if (item)
							{
								enemy.get_backpack_inventory().addItem(std::move(item));
							}
							break;
						}
						case(1):
						{
							item = pickRandomItem<Weapon>(Rarity::Legendery);
							if (item)
							{
								enemy.get_backpack_inventory().addItem(std::move(item));
							}
							break;
						}
						}
					}
				}
			}
		}
		
	}

	void Enemyfactory::loadFromFile()
	{
		std::fstream file("Enemy", std::ios::in);
		if (file.is_open())
		{
			std::string line;
			int cityid = 0,forestid = 0,caveid = 0,roadid = 0;
			Location::LocationType typeoflocation;
			getline(file, line);
			while (getline(file, line))
			{
				std::stringstream ss(line);
				getline(ss, line, ';');
				typeoflocation = Location::stringToLocation(line);
				switch (typeoflocation)
				{
				case(Location::LocationType::City):
				{
					getline(ss, line);
					city.emplace(cityid, line);
					cityid += 1;
					break;
				}
				case(Location::LocationType::Forest):
				{
					getline(ss, line);
					forest.emplace(forestid, line);
					forestid += 1;
					break;
				}
				case(Location::LocationType::Road):
				{
					getline(ss, line);
					road.emplace(roadid, line);
					roadid += 1;
					break;
				}
				case(Location::LocationType::Cave):
				{
					getline(ss, line);
					cave.emplace(caveid, line);
					caveid += 1;
					break;
				}
				default:
					break;
				}
			}
		}
	}
}