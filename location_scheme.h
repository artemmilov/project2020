#pragma once
#ifndef Loc_def
#define Loc_def

#include "general_terms_for_level_part.h"

class location_scheme
{
private:
	std::string name;

	std::vector <std::string> v_room_names;
	std::string current_room_name;

	std::map <std::string, std::pair <std::string, std::string>> map_interior_transitions;
	std::map <std::string, std::pair <std::string, std::string>> map_external_transitions;
public:
	std::string get_name();
	std::vector <std::string>& get_v_room_names();
	std::string get_current_room_name();
	std::map <std::string, std::pair <std::string, std::string>>& get_map_interior_transitions();
	std::map <std::string, std::pair <std::string, std::string>>& get_map_external_transitions();

	bool start_in_room(std::string);
	bool transit_by_interior_transition(std::string);

	bool init(std::string, std::string);
};


#endif