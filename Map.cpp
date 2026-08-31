#include "Map.h"

namespace oop::projekt
{
	Map::Map()
	{
        locations.reserve(13);
        // kreiranje lokacija
        locations.reserve(14);
        locations.push_back(Location("Ironhold", Location::LocationType::City, Location::Difficulty::Easy));
        locations.push_back(Location("Stonehaven", Location::LocationType::City, Location::Difficulty::Medium));
        locations.push_back(Location("Duskport", Location::LocationType::City, Location::Difficulty::Easy));
        locations.push_back(Location("Ashenvale", Location::LocationType::City, Location::Difficulty::VeryHard));
        locations.push_back(Location("Frostpeak Cave", Location::LocationType::Cave, Location::Difficulty::Medium));
        locations.push_back(Location("Shadowpit Cave", Location::LocationType::Cave, Location::Difficulty::Medium));
        locations.push_back(Location("North Road", Location::LocationType::Road, Location::Difficulty::Easy));
        locations.push_back(Location("East Road", Location::LocationType::Road, Location::Difficulty::Easy));
        locations.push_back(Location("South Road", Location::LocationType::Road, Location::Difficulty::VeryHard));
        locations.push_back(Location("Pinewood", Location::LocationType::Forest, Location::Difficulty::Easy));
        locations.push_back(Location("Ashwood", Location::LocationType::Forest, Location::Difficulty::Easy));
        locations.push_back(Location("Darkwood", Location::LocationType::Forest, Location::Difficulty::Easy));
        locations.push_back(Location("Mirkwood", Location::LocationType::Forest, Location::Difficulty::Hard));
        locations.push_back(Location("Thornwood", Location::LocationType::Forest, Location::Difficulty::Hard));

        // pointeri za lakse citanje
        Location* ironhold = &locations[0];
        Location* stonehaven = &locations[1];
        Location* duskport = &locations[2];
        Location* ashenvale = &locations[3];
        Location* frostpeak = &locations[4];
        Location* shadowpit = &locations[5];
        Location* northroad = &locations[6];
        Location* eastroad = &locations[7];
        Location* southroad = &locations[8];
        Location* pinewood = &locations[9];
        Location* ashwood = &locations[10];
        Location* darkwood = &locations[11];
        Location* mirkwood = &locations[12];
        Location* thornwood = &locations[13];

        current_location = ironhold;

        // veze
        connections[ironhold] = { pinewood, ashwood, darkwood,eastroad};
        connections[stonehaven] = { northroad, mirkwood, darkwood };
        connections[duskport] = { eastroad, shadowpit };
        connections[ashenvale] = { mirkwood,southroad };
        connections[frostpeak] = { ashwood };
        connections[shadowpit] = { duskport, thornwood };
        connections[northroad] = { pinewood, stonehaven };
        connections[eastroad] = { ironhold, duskport };
        connections[southroad] = { mirkwood, ashenvale, thornwood,darkwood };
        connections[pinewood] = { ironhold, northroad };
        connections[ashwood] = { ironhold, frostpeak };
        connections[darkwood] = { ironhold, stonehaven, southroad };
        connections[mirkwood] = { stonehaven,southroad,ashenvale };
        connections[thornwood] = { shadowpit, southroad };
    }
}