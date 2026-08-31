#pragma once
#include <iostream>
#include <string>


namespace oop::projekt
{

	class Location
	{
	public:
		enum class LocationType
		{
			City,
			Forest,
			Cave,
			Road
		};

		inline std::string toString(LocationType type)
		{
			switch (type) {
			case LocationType::City:
				return "City";
			case LocationType::Forest:
				return "Forest";
			case LocationType::Cave:
				return "Cave";
			case LocationType::Road:
				return "Road";
			default:
				return "None";
			}
		}

		static LocationType stringToLocation(const std::string& s) 
		{
			if (s == "City") return LocationType::City;
			if (s == "Road") return LocationType::Road;
			if (s == "Forest") return LocationType::Forest;
			if (s == "Cave") return LocationType::Cave;
			return LocationType::City;
		}

		enum class Difficulty
		{
			Easy,
			Medium,
			Hard,
			VeryHard
		};

		std::string toString(Difficulty diff)
		{
			switch (diff) {
			case Difficulty::Easy:
				return "Easy";
			case Difficulty::Medium:
				return "Medium";
			case Difficulty::Hard:
				return "Hard";
			case Difficulty::VeryHard:
				return "Very Hard";
			default:
				return "None";
			}
		}

		static int toInt(Difficulty diff)
		{
			switch (diff) {
			case Difficulty::Easy:
				return 0;
			case Difficulty::Medium:
				return 1;
			case Difficulty::Hard:
				return 2;
			case Difficulty::VeryHard:
				return 3;
			default:
				return 0;
			}
		}

	private:
		std::string location_Name;
		LocationType location_Type;
		Difficulty locationdifficulty;

	public:
		Location(std::string location_Name, LocationType location_Type, Difficulty locationadiff) :
			location_Name(location_Name), location_Type(location_Type), locationdifficulty(locationadiff) {}
		std::string getLocationName() { return location_Name; }
		LocationType getLocationType() { return location_Type; }
		Difficulty getlocationdifficulty() { return locationdifficulty; }
	};
}

