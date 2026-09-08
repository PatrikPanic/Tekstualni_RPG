#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "Location.h"

namespace oop::projekt
{
	// Map - svijet igre zadan kao graf. Lokacije su cvorovi, a veze medu njima
	// bridovi. Veze su dvosmjerne i cuvaju se kao popis susjeda za svaku
	// lokaciju, pa se igracu nude samo lokacije dostupne iz trenutne.
	//
	// Vazno: connections cuva pokazivace na elemente spremnika locations.
	// Zato je koristen deque, a ne vector - deque pri dodavanju na kraj ne
	// premjesta postojece elemente, pa pokazivaci ostaju valjani bez obzira
	// na broj lokacija. Kod vectora bi se morala pamtiti tocna rezervacija.
	class Map
	{
	private:
		std::deque<Location> locations;
		Location* current_location;
		Location* start_location;
		Location* final_location;
		std::vector<Location*> midboss_locations;
		std::map<Location*, std::vector<Location*>> connections;

	public:
		Map();
		std::vector<Location*> getAvailableLocations() const { return connections.at(current_location);}
		Location* getCurrentLocation() const { return current_location;}
		Location* getFinalLocation() const { return final_location; }
		std::vector<Location*> getMidbossLocations() const { return midboss_locations; }
		bool isAtFinalLocation() const { return current_location == final_location; }
		bool isMidbossLocation(Location* location) const;
		void resetToStart() { current_location = start_location; }
		void moveTo(Location* destination) { current_location = destination; }
	};
}

