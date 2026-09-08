#include "Item.h"
#include <sstream>
#include <iomanip>

namespace oop::projekt
{
    Item Item::fromLine(const std::string& line)
    {
        std::stringstream ss(line);

        std::string name;
        Item_Type type = Item_Type::Junk;
        Rarity rarity = Rarity::Common;
        int ID;

        std::string s;
        getline(ss, s, ';');
        ID = std::stoi(s);

        getline(ss, name, ';');

        getline(ss, s, ';');
        rarity = stringToRarity(s);

        Item item(name, type, rarity, ID);
        return item;
    }

    std::string Item::seeStats() const
    {
        std::ostringstream out;
        out << "item name: " << item_name << "\n"
            << "item type: " << toString(item_type) << "\n"
            << "item rarity: " << toString(rarity) << "\n";
        return out.str();
    }

    Armor Armor::fromLine(const std::string& line)
    {
        std::stringstream ss(line);

        std::string name;
        Rarity rarity = Rarity::Common;
        int ID;
        int defense;
        float speed;
        ArmorSlot slot = ArmorSlot::Helmet;

        std::string s;
        getline(ss, s, ';');
        ID = std::stoi(s);

        getline(ss, name, ';');

        getline(ss, s, ';');
        rarity = stringToRarity(s);

        getline(ss, s, ';');
        defense = std::stoi(s);

        getline(ss, s, ';');
        speed = std::stof(s);

        getline(ss, s, ';');
        slot = stringToArmorSlot(s);


        Armor armor(name, ID, rarity, defense, speed, slot);
        return armor;
    }

    std::string Armor::seeStats() const
    {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2);
        out << "item name: " << item_name << "\n"
            << "item type: " << toString(item_type) << "\n"
            << "armor defense: " << armor_defense << "\n"
            << "armor speed: " << armor_speed << "\n"
            << "armor type: " << toString(armor_type) << "\n"
            << "item rarity: " << toString(rarity) << "\n";
        return out.str();
    }

    Weapon Weapon::fromLine(const std::string& line)
    {
        std::stringstream ss(line);

        std::string name;
        Rarity rarity = Rarity::Common;
        int ID;
        int defense, attack;
        float speed;
        WeaponSlot slot = WeaponSlot::Right_hand;

        std::string s;
        getline(ss, s, ';');
        ID = std::stoi(s);

        getline(ss, name, ';');

        getline(ss, s, ';');
        rarity = stringToRarity(s);

        getline(ss, s, ';');
        defense = std::stoi(s);

        getline(ss, s, ';');
        speed = std::stof(s);

        getline(ss, s, ';');
        attack = std::stoi(s);

        getline(ss, s, ';');
        slot = stringToWeaponSlot(s);


        Weapon weapon(name, ID, rarity, defense, speed,attack, slot);
        return weapon;
    }

    std::string Weapon::seeStats() const
    {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2);
        out << "item name: " << item_name << "\n"
            << "item type: " << toString(item_type) << "\n"
            << "weapon attack: " << weapon_attack << "\n"
            << "weapon defense: " << weapon_defense << "\n"
            << "weapon speed: " << weapon_speed << "\n"
            << "hand: " << toString(weapon_slot) << "\n"
            << "item rarity: " << toString(rarity) << "\n";
        return out.str();
    }

}