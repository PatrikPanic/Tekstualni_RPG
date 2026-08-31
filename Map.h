#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Location.h"

namespace oop::projekt
{
	class Map
	{
	private:
		std::vector<Location> locations;
		Location* current_location;
		std::map<Location*, std::vector<Location*>> connections;

	public:
		Map();
		std::vector<Location*> getAvailableLocations() const { return connections.at(current_location);}
		Location* getCurrentLocation() const { return current_location;}
		void moveTo(Location* destination) { current_location = destination; }
	};
}

