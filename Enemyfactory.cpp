#include "Enemyfactory.h"

namespace oop::projekt
{
	// Vraca nasumicno ime iz zadanog popisa. Ako je popis prazan (npr. datoteka
	// s imenima nije pronadena), vraca zamjensko ime umjesto dijeljenja s nulom.
	std::string EnemyFactory::pickRandomName(const std::vector<std::string>& names) const
	{
		if (names.empty())
			return "Unknown Enemy";
		return names[Random::next(0, static_cast<int>(names.size()) - 1)];
	}

	// Stvara obicnog protivnika. Razina raste s tezinom lokacije, a iz nje se
	// izvode ostale statistike, pa su protivnici na teskim lokacijama opasniji
	// ali i vrjedniji.
	Enemy EnemyFactory::createEnemy(Location* location)
	{
		int lvl = 1 + Location::toInt(location->getlocationdifficulty()) * 4 + Random::next(0, 3);
		int max_hp = 50 + lvl * 14;
		float base_attack = 14 + lvl * 4.0;
		float base_defense = 3 + lvl * 1.4;
		float base_speed = 0.75 + lvl * 0.02;
		std::string name;
		switch (location->getLocationType())
		{
		case(Location::LocationType::City):
		{
			name = pickRandomName(city);
			break;
		}
		case(Location::LocationType::Forest):
		{
			name = pickRandomName(forest);
			break;
		}
		case(Location::LocationType::Road):
		{
			name = pickRandomName(road);
			break;
		}
		case(Location::LocationType::Cave):
		{
			name = pickRandomName(cave);
			break;
		}
		default:
		{
			name = pickRandomName(city);
			break;
		}
		}
		Enemy enemy(name, lvl, max_hp, base_attack, base_defense, base_speed, 5);
		fillEnemyBackpack(enemy);
		return enemy;
	}

	// Zavrsni protivnik u Ashenvaleu. Statistike su fiksne jer je to jedini
	// protivnik koji zavrsava igru, pa ne smije ovisiti o nasumicnosti.
	Enemy EnemyFactory::createBoss()
	{
		int lvl = 18;
		int max_hp = 400;
		float base_attack = 76;
		float base_defense = 22;
		float base_speed = 1.0;
		Enemy boss("Voidspire Overlord", lvl, max_hp, base_attack, base_defense, base_speed, 5);
		fillEnemyBackpack(boss);
		return boss;
	}

	// Srednji boss cuva prolaz prema Voidspireu. Jaci je od obicnih protivnika
	// na istoj lokaciji, ali slabiji od zavrsnog bossa.
	Enemy EnemyFactory::createMidBoss(Location* location)
	{
		int lvl = 12;
		int max_hp = 200;
		float base_attack = 52;
		float base_defense = 14;
		float base_speed = 0.9;
		std::string name = "Midboss";
		if (location->getLocationName() == "Mirkwood")
			name = "Mirkwood Warden";
		else if (location->getLocationName() == "Thornwood")
			name = "Thornwood Butcher";

		Enemy midboss(name, lvl, max_hp, base_attack, base_defense, base_speed, 5);
		fillEnemyBackpack(midboss);
		return midboss;
	}

	// Puni ruksak protivnika opremom koju igrac moze uzeti nakon pobjede.
	// Rijetkost predmeta ovisi o razini protivnika.
	void EnemyFactory::fillEnemyBackpack(Enemy& enemy)
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
		int roll_rarity,roll_store;
		for (int i = 0; i < num_items; i++)
		{
			roll_rarity = Random::next(0, static_cast<int>(total) - 1);
			if (roll_rarity < common_weight)
			{
				roll_store = Random::next(0, 2);
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
				roll_rarity -= common_weight;
				if (roll_rarity < rear_weight)
				{
					roll_store = Random::next(0, 1);
					std::unique_ptr <Item> item;
					switch (roll_store)
					{
					case(0):
					{
						item = pickRandomItem<Armor>(Rarity::Rare);
						if (item)
						{
							enemy.get_backpack_inventory().addItem(std::move(item));
						}
						break;
					}
					case(1):
					{
						item = pickRandomItem<Weapon>(Rarity::Rare);
						if (item)
						{
							enemy.get_backpack_inventory().addItem(std::move(item));
						}
						break;
					}
					}
				}
				else {
					roll_rarity -= rear_weight;
					if (roll_rarity < epic_weight)
					{
						roll_store = Random::next(0, 1);
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
						roll_store = Random::next(0, 1);
						std::unique_ptr <Item> item;
						switch (roll_store)
						{
						case(0):
						{
							item = pickRandomItem<Armor>(Rarity::Legendary);
							if (item)
							{
								enemy.get_backpack_inventory().addItem(std::move(item));
							}
							break;
						}
						case(1):
						{
							item = pickRandomItem<Weapon>(Rarity::Legendary);
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

	void EnemyFactory::loadFromFile()
	{
		std::fstream file("Enemy", std::ios::in);
		if (!file.is_open())
			throw FileNotFoundException("Enemy");
		{
			std::string line;
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
					city.push_back(line);
					break;
				}
				case(Location::LocationType::Forest):
				{
					getline(ss, line);
					forest.push_back(line);
					break;
				}
				case(Location::LocationType::Road):
				{
					getline(ss, line);
					road.push_back(line);
					break;
				}
				case(Location::LocationType::Cave):
				{
					getline(ss, line);
					cave.push_back(line);
					break;
				}
				default:
					break;
				}
			}
		}
	}
}