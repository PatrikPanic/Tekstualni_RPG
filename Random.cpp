#include "Random.h"

namespace oop::projekt
{
	// Generator je lokalna staticka varijabla, pa se stvara pri prvoj uporabi
	// i traje do kraja programa. Time se izbjegava ovisnost o redoslijedu
	// inicijalizacije globalnih objekata.
	std::mt19937& Random::engine()
	{
		static std::mt19937 generator(std::random_device{}());
		return generator;
	}

	void Random::seed(unsigned int value)
	{
		engine().seed(value);
	}

	int Random::next(int min, int max)
	{
		if (min > max)
			return min;
		std::uniform_int_distribution<int> raspon(min, max);
		return raspon(engine());
	}
}
