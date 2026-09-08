#include "UI.h"
#include <conio.h>

namespace oop::projekt
{
    int UI::showOptions(std::vector<std::string> options, std::string header1, std::string header2)
    {
        int selected = 0;
        int size = static_cast<int>(options.size());

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
        std::vector<std::string> options = { "Start game", "Highscore", "Exit" };
        return showOptions(options);
    }

    std::string UI::askPlayerName()
    {
        system("cls");
        std::string name;
        std::cout << "Enter your hero's name: ";
        std::getline(std::cin, name);
        if (name.empty())
            name = "Player";
        return name;
    }

    void UI::showIntro(std::string player_name)
    {
        system("cls");
        std::cout << "===== TEXT RPG =====\n" << std::endl;
        std::cout << "  For twenty years they called you " << player_name << " the Ironhand." << std::endl;
        std::cout << "  You fought in wars nobody remembers the names of anymore." << std::endl;
        std::cout << std::endl;
        std::cout << "  Then you stopped. You hung your sword above the door, married" << std::endl;
        std::cout << "  Mira, and swore you would never lift a blade again. For six" << std::endl;
        std::cout << "  quiet years in Ironhold, you kept that promise." << std::endl;
        std::cout << std::endl;
        std::cout << "  Three nights ago, servants of the Voidspire Overlord came for" << std::endl;
        std::cout << "  her. They dragged Mira south, through the dead city of" << std::endl;
        std::cout << "  Ashenvale, down to Voidspire Cave, where their master keeps" << std::endl;
        std::cout << "  what he takes." << std::endl;
        std::cout << std::endl;
        std::cout << "  This morning you took the sword down off the wall." << std::endl;
        std::cout << std::endl;
        std::cout << "  The entrance to Voidspire is sealed by black magic. The seal is" << std::endl;
        std::cout << "  held by two of the Overlord's commanders:" << std::endl;
        std::cout << std::endl;
        std::cout << "     Mirkwood Warden     - waits for you in Mirkwood forest" << std::endl;
        std::cout << "     Thornwood Butcher   - waits for you in Thornwood forest" << std::endl;
        std::cout << std::endl;
        std::cout << "  While both of them live, you cannot enter Voidspire." << std::endl;
        std::cout << std::endl;
        std::cout << "  You are not the man you were at thirty. The road south runs" << std::endl;
        std::cout << "  through forests, roads and caves full of things that were once" << std::endl;
        std::cout << "  people. Take the gear off the ones you put down - you will need" << std::endl;
        std::cout << "  it. Rest in the cities when you must, but every day you spend is" << std::endl;
        std::cout << "  a day they still have her, and it will cost you." << std::endl;
        std::cout << std::endl;
        std::cout << "  Do not walk into Voidspire wounded." << std::endl;
        std::cout << std::endl;
        std::cout << "  Mira is waiting." << std::endl;
        std::cout << "\n\nPress any key to begin..." << std::endl;
        _getch();
    }

    void UI::showHighscore(Highscore& highscore)
    {
        system("cls");
        std::cout << "===== HIGHSCORE =====" << std::endl;

        std::vector<std::string> scores = highscore.getScoreNames();
        if (scores.empty())
            std::cout << "No games played yet." << std::endl;

        for (int i = 0; i < static_cast<int>(scores.size()); i++)
        {
            std::cout << scores[i] << std::endl;
        }

        std::cout << "\nPress any key to go back..." << std::endl;
        _getch();
    }

    void UI::showGameOver(bool won, int raw_score, int days, int final_score)
    {
        system("cls");
        if (won)
        {
            std::cout << "===== VICTORY =====" << std::endl;
            std::cout << std::endl;
            std::cout << "  The Voidspire Overlord falls. The chains come apart in your" << std::endl;
            std::cout << "  hands, and Mira is standing there, alive." << std::endl;
            std::cout << std::endl;
            std::cout << "  You walk back north together. Somewhere on the road you" << std::endl;
            std::cout << "  leave the sword behind, in the dirt, and you do not go back" << std::endl;
            std::cout << "  for it." << std::endl;
            std::cout << std::endl;
            std::cout << "  Whatever comes for the world next, it comes without you." << std::endl;
            std::cout << "  You have done your part. Twice." << std::endl;
        }
        else
        {
            std::cout << "===== GAME OVER =====" << std::endl;
            std::cout << std::endl;
            std::cout << "  Your strength gives out somewhere on the southern road." << std::endl;
            std::cout << "  You were right the first time - you should never have" << std::endl;
            std::cout << "  picked the sword back up." << std::endl;
            std::cout << std::endl;
            std::cout << "  Mira is still waiting." << std::endl;
        }
        std::cout << "\n  Score earned : " << raw_score << std::endl;
        std::cout << "  Days spent   : " << days << std::endl;
        std::cout << "  Day penalty  : -" << (raw_score - final_score) << std::endl;
        std::cout << "  ---------------------------" << std::endl;
        std::cout << "  FINAL SCORE  : " << final_score << std::endl;
        std::cout << "\nPress any key..." << std::endl;
        _getch();
    }

    UI::Game_choice UI::gameMenu(Map& map, Player_Character& player, Score& score, bool can_rest)
    {
        std::string header = "  Day: " + std::to_string(score.getDays()) +
            "     HP: " + std::to_string((int)player.gethp()) + " / " + std::to_string((int)player.getMaxHp()) +
            "     Score: " + std::to_string(score.getScore()) +
            "  (-" + std::to_string(score.getDayPenalty()) + " for days)\n\n";
        header += drawMap(map.getCurrentLocation());

        // Popis stavki i popis pripadnih radnji grade se zajedno, pa odabrani
        // redni broj uvijek pokazuje na ispravnu radnju.
        std::vector<std::string> options = { "Travel", "Inventory", "Stats", "Attack" };
        std::vector<Game_choice> actions = { Game_choice::Travel, Game_choice::Inventory,
            Game_choice::Stats, Game_choice::Attack };

        if (can_rest)
        {
            options.push_back("Rest (heals fully, costs 1 day)");
            actions.push_back(Game_choice::Rest);
        }
        options.push_back("Back");
        actions.push_back(Game_choice::Back);

        int selected = showOptions(options, header);
        if (selected < 0 || selected >= static_cast<int>(actions.size()))
            return Game_choice::Back;
        return actions[selected];
    }

    int UI::showBattleMenu(Player_Character& player, Enemy& enemy)
    {
        std::string header;
        header = std::string(20, ' ') + "[ " + player.getName() +
            " HP: " + std::to_string((int)player.gethp()) +
            " ] -------- [ " + enemy.getName() + " Lv." + std::to_string(enemy.getLVL()) +
            " HP: " + std::to_string((int)enemy.gethp()) + " ]";

        std::vector<std::string> options = { "Attack", "Defend", "Run" };
        return showOptions(options, header);
    }

    // Popis predmeta koje je protivnik ostavio. Uz njih se nudi i pregled
    // vlastitog ruksaka, kako bi igrac mogao osloboditi mjesto bez izlaska
    // iz ekrana plijena.
    int UI::lootMenu(Player_Character& player, Enemy& enemy)
    {
        std::string header1 = "Loot from: " + enemy.getName() + "\n";
        std::string header2 = "\nYour backpack (" +
            std::to_string(player.get_backpack_inventory().inventory_size()) + " / " +
            std::to_string(player.get_backpack_inventory().capacity()) + " items)\n" +
            player.get_backpack_inventory().getItemNames_string() + "\n";

        std::vector<std::string> options = enemy.get_backpack_inventory().getItemNames();
        options.push_back("My backpack (drop items)");
        options.push_back("Back");
        return showOptions(options, header1, header2);
    }

    int UI::lootItemChoice(Item& item)
    {
        std::string header1 = item.seeStats() + "\n";
        std::vector<std::string> options = { "Take", "Back" };
        return showOptions(options, header1);
    }

    // Pregled vlastitog ruksaka tijekom uzimanja plijena. Uz svaki predmet
    // prikazuje se i sto je protivnik ostavio, kako bi igrac mogao usporediti
    // prije nego nesto baci.
    int UI::lootBackpackMenu(Player_Character& player, Enemy& enemy)
    {
        std::string header1 = "Your backpack (" +
            std::to_string(player.get_backpack_inventory().inventory_size()) + " / " +
            std::to_string(player.get_backpack_inventory().capacity()) + " items)\n";
        std::string header2 = "\nStill on the body of " + enemy.getName() + ":\n" +
            enemy.get_backpack_inventory().getItemNames_string() + "\n";

        std::vector<std::string> options = player.get_backpack_inventory().getItemNames();
        options.push_back("Back");
        return showOptions(options, header1, header2);
    }

    int UI::lootDropChoice(Item& item)
    {
        std::string header1 = item.seeStats() + "\n";
        std::vector<std::string> options = { "Drop this item", "Keep it" };
        return showOptions(options, header1);
    }

    void UI::showMessage(std::string message)
    {
        std::cout << message << std::endl;
        _getch();
    }

    void UI::showAttackResult(float dmg, Battle::TurnOwner attacker)
    {
        std::string msg = (attacker == Battle::TurnOwner::Player ? "You hit for " : "The enemy hits you for ");
        msg += std::to_string((int)dmg) + " damage!";
        showMessage(msg);
    }

    void UI::showDefendResult()
    {
        showMessage("You raise your guard.");
    }

    void UI::showRunResult(bool success)
    {
        showMessage(success ? "You got away!" : "You failed to escape, the enemy attacks!");
    }

    // Crta mapu svijeta kao staticku sliku i oznacava trenutnu lokaciju.
    // Crtez se ne generira iz grafa nego je zapisan rucno, jer je mapa
    // nepromjenjiva i tako je ispis pregledniji.
    std::string UI::drawMap(Location* current)
    {
        std::string map = "";
        map += "                [Ashwood]---[Frostpeak Cave]\n";
        map += "                    |\n";
        map += "    [Pinewood]---[IRONHOLD]---[East Road]---[DUSKPORT]\n";
        map += "         |          |                            |\n";
        map += "   [North Road]  [Darkwood]            [Shadowpit Cave]\n";
        map += "         |          |                            |\n";
        map += "         +---[STONEHAVEN]                 [Thornwood]*\n";
        map += "                    |                            |\n";
        map += "              [Mirkwood]*---[South Road]---------+\n";
        map += "                    |            |\n";
        map += "                    +---[ASHENVALE]\n";
        map += "                              |\n";
        map += "                     [Voidspire Cave]**\n";
        map += "\n   *  mid boss        ** final boss\n";
        map += "\n  Current location: " + current->getLocationName();
        map += "\n";
        return map;
    }

    int UI::showMapMenu(Map& map)
    {
        std::vector<std::string> options;
        std::vector<Location*> available = map.getAvailableLocations();
        for (Location* location : available)
        {
            options.push_back(location->getLocationName() + " (Difficulty: " + Location::toString(location->getlocationdifficulty()) + ")");
        }
        options.push_back("Back");
        return showOptions(options, drawMap(map.getCurrentLocation()));
    }

    int UI::backpack_inventory(Player_Character& player)
    {
        std::string header2 = "\nArmor\n" + player.get_armor_inventory().getItemNames_string() + "\n" +
            "Weapon\n" + player.get_weapon_inventory().getItemNames_string() + "\n";
        std::vector<std::string> options = player.get_backpack_inventory().getItemNames();
        options.push_back("Back");
        return showOptions(options, "Backpack", header2);
    }

    int UI::armor_inventory(Player_Character& player)
    {
        std::string header1, header2;
        header1 = "Backpack\n" + player.get_backpack_inventory().getItemNames_string() +
            "\nArmor\n";
        header2 = "\nWeapon\n" + player.get_weapon_inventory().getItemNames_string();
        std::vector<std::string> options = player.get_armor_inventory().getItemNames();
        options.push_back("Back");
        return showOptions(options, header1, header2);
    }

    int UI::weapon_inventory(Player_Character& player)
    {
        std::string header1;
        header1 = "Backpack\n" + player.get_backpack_inventory().getItemNames_string() +
            "\nArmor\n" + player.get_armor_inventory().getItemNames_string() + "\nWeapon\n";
        std::vector<std::string> options = player.get_weapon_inventory().getItemNames();
        options.push_back("Back");
        return showOptions(options, header1);
    }

    int UI::showInventory()
    {
        std::vector<std::string> options{ "Backpack", "Armor", "Weapon", "Back" };
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
            std::vector<std::string> options{ "Drop", "See item stats", "Back" };
            is_equipped ? options.insert(options.begin(), "Unequip") : options.insert(options.begin(), "Equip");
            int choice = showOptions(options);
            if (choice == 0)
                return is_equipped ? Item_choice::Unequip : Item_choice::Equip;
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