#include "UI.h"
#include <conio.h>

namespace oop::projekt
{
    int UI::showOptions(std::vector<std::string> options, std::string header1, std::string header2)
    {
        int selected = 0;
        int size = options.size();

        while (true)
        {
            system("cls");

            std::cout << header1 << std::endl;

            for (int i = 0; i < size; i++)
            {
                if (i == selected)
                    std::cout << "> ";

                else
                    std::cout << "  ";

                std::cout << options[i] << '\n';
            }

            std::cout << header2 << std::endl;

            int key = _getch();

            // arrow keys
            if (key == 224)
            {
                key = _getch();

                if (key == 72) // gore
                    selected--;

                else if (key == 80) // dolje
                    selected++;
            }

            if (selected < 0)
                selected = size - 1;

            if (selected > size - 1)
                selected = 0;

            if (key == 13) // ENTER
            {
                return selected;
            }
        }
    }

    int UI::showMainMenu()
    {
        std::vector<std::string> options = { "Start game", "Exit" };
        return showOptions(options);
    }

    int UI::gameMenu(Map& map)
    {
        std::vector<std::string> options = { "Travel", "Inventory","Stats","Attack","Back"};
        return showOptions(options, drawMap(map.getCurrentLocation()));
    }

    int UI::showBattleMenu(Player_Character& player, Enemy& enemy)
    {
        std::string header;
        header = std::string(20, ' ') + "[ " + player.getName() + " Lv." + std::to_string(player.getLVL()) +
            " HP: " + std::to_string((int)player.gethp()) +
            " ] -------- [ " + enemy.getName() + " Lv." + std::to_string(enemy.getLVL()) +
            " HP: " + std::to_string((int)enemy.gethp()) + " ]";

        std::vector<std::string> options = { "Attack", "Defend", "Run" };
        return showOptions(options, header);
    }

    void UI::showMessage(std::string message)
    {
        std::cout << message << std::endl;
        _getch();
    }

    void UI::showAttackResult(float dmg, Battle::TurnOwner attacker)
    {
        std::string msg = (attacker == Battle::TurnOwner::Player ? "Player" : "Enemy");
        msg += " napao za " + std::to_string(dmg) + " dmg!";
        showMessage(msg);
    }

    void UI::showDefendResult()
    {
        showMessage("Player se brani!");
    }

    void UI::showRunResult(bool success)
    {
        showMessage(success ? "Pobjegao si!" : "Nije uspio pobjeći, enemy napada!");
    }

    std::string UI::drawMap(Location* current)
    {
        std::string map = "";
        map += "              [Ashwood]---[Frostpeak Cave]\n";
        map += "                    |         \n";
        map += "    [Pinewood]------[IRONHOLD]---[East Road]-------[DUSKPORT]\n";
        map += "        |                |                               |\n";
        map += "    [North Road]         +------[Darkwood]       [Shadowpit Cave]\n";
        map += "        |                          /   \                 |\n";
        map += "        +------[STONEHAVEN]-------+     \           [Thornwood]\n";
        map += "                    |                    \               |\n";
        map += "              [Mirkwood]          [South Road]-----------+\n";
        map += "                    |                 |\n";
        map += "                    +-----------------+\n";
        map += "                                      |\n";
        map += "                                 [ASHENVALE]\n";
        map += "\n  Trenutna lokacija: " + current->getLocationName();
        map += "\n";
        return map;
    }

    int UI::showMapMenu(Map& map)
    {
        std::vector<std::string> options;
        std::vector<Location*> available = map.getAvailableLocations();
        for (Location* location : available)
        {
            options.push_back(location->getLocationName() + " (Difficulty: " + location->toString(location->getlocationdifficulty()) + ")");
        }
        options.push_back("back");
        return showOptions(options, drawMap(map.getCurrentLocation()));
    }

    int UI::backpack_inventory(Player_Character& player)
    {
        std::string header2 = "\nArmor inventory\n" + player.get_armor_inventory().getItemNames_string() + "\n" + 
            "Weapon inventory\n" + player.get_weapon_inventory().getItemNames_string() + "\n";
        std::vector<std::string> options = player.get_backpack_inventory().getItemNames();
        options.push_back("back");
        return showOptions(options, "Backpack inventory", header2);
    }

    int UI::armor_inventory(Player_Character& player)
    {
        std::string header1, header2;
        header1 = "Backpack inventory\n" + player.get_backpack_inventory().getItemNames_string() +
            "\n Armor inventory\n";
        header2 = "Weapon inventory\n" + player.get_weapon_inventory().getItemNames_string();
        std::vector<std::string> options = player.get_armor_inventory().getItemNames();
        options.push_back("back");
        return showOptions(options, header1, header2);
    }

    int UI::weapon_inventory(Player_Character& player)
    {
        std::string header1;
        header1 = "Backpack inventory\n" + player.get_backpack_inventory().getItemNames_string() +
            "\n Armor inventory\n" + player.get_armor_inventory().getItemNames_string() + "\n Weapon inventory \n";
        std::vector<std::string> options = player.get_weapon_inventory().getItemNames();
        options.push_back("back");
        return showOptions(options, header1);
    }

    int UI::showInventory(Player_Character& player)
    {
        std::vector<std::string> options{ "Backpack Inventory" , "Armor inventory", "Weapon invenotry", "back" };
        return(showOptions(options));
    }

    UI::Item_choice UI::item_choice(Item& item, bool is_equipped)
    {
        switch (item.getType())
        {
        case(Item_Type::Junk):
        {
            std::vector<std::string> options{ "Drop", "Back" };
            int option = showOptions(options);
            return option ? Item_choice::Back : Item_choice::Drop;
            break;
        }
        case (Item_Type::Weapon):
        case(Item_Type::Armor):
        {
            std::vector<std::string> options{ "Drop","see stats of item", "Back" };
            is_equipped ? options.insert(options.begin(), "Unequip") : options.insert(options.begin(), "Equip");
            int choice = showOptions(options);
            if (choice == 0)
                return is_equipped ? Item_choice::Unequipe : Item_choice::Equipe;
            else if (choice == 1)
                return Item_choice::Drop;
            else if (choice == 2)
                return Item_choice::See_stats;
            else
                return Item_choice::Back;
        }
        default:
            return Item_choice::Back;
        }

    }
}