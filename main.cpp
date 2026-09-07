#include "Game.h"
#include "Exception.h"

using namespace oop::projekt;
int main()
{
    srand(time(0));

    try
    {
        Game game;
    }
    catch (const GameException& greska)
    {
        std::cout << "Game error: " << greska.what() << std::endl;
        std::cout << "Make sure armor.txt, weapon.txt, junk.txt and Enemy are in the"
            << " same folder as the program." << std::endl;
        return 1;
    }
    catch (const std::exception& greska)
    {
        std::cout << "Unexpected error: " << greska.what() << std::endl;
        return 1;
    }

    return 0;
}
