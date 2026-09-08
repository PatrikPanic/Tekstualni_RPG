#pragma once
#include <random>

namespace oop::projekt
{
	// Random - jedno mjesto na kojem se generiraju svi slucajni brojevi u igri.
	// Umjesto rand() koristi se mt19937 iz <random>, koji daje ravnomjerniju
	// raspodjelu i ne ovisi o globalnom stanju koje bilo tko moze promijeniti.
	//
	// Generator je zajednicki za cijelu igru i sjeme se postavlja jednom, pa se
	// zadavanjem sjemena moze dobiti ponovljiv tijek, sto koriste testovi.
	class Random
	{
	public:
		// Vraca nasumican cijeli broj iz raspona [min, max], ukljucivo.
		static int next(int min, int max);

		// Postavlja sjeme generatora. Bez poziva se generator sam nasumicno
		// inicijalizira pri prvoj uporabi.
		static void seed(unsigned int value);

	private:
		static std::mt19937& engine();
	};
}
