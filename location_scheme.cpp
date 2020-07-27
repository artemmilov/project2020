#include "location_scheme.h"

using namespace std;
using namespace std::experimental::filesystem;

std::string location_scheme::get_name()
{
	return name;
}
std::vector <std::string>& location_scheme::get_v_room_names()
{
	return v_room_names;
}
string location_scheme::get_current_room_name()
{
	return current_room_name;
}
std::map <std::string, std::pair <std::string, std::string>>& location_scheme::get_map_interior_transitions()
{
	return map_interior_transitions;
}
std::map <std::string, std::pair <std::string, std::string>>& location_scheme::get_map_external_transitions()
{
	return map_external_transitions;
}
bool location_scheme::start_in_room(std::string start_room_name)
{
	bool ok = false;
	for (int i(0); i < v_room_names.size(); i++)
		if (v_room_names[i] == start_room_name)
			if (!ok)
				ok = true;
			else
				return false;
	if (!ok)
		return false;
	current_room_name = start_room_name;
	return true;
}
bool location_scheme::transit_by_interior_transition(std::string transition_name)
{
	if (map_interior_transitions[transition_name].first != current_room_name && map_interior_transitions[transition_name].second != current_room_name)
		return false;
	if (map_interior_transitions[transition_name].first == current_room_name && map_interior_transitions[transition_name].second == current_room_name)
		return false;
	if (map_interior_transitions[transition_name].first == current_room_name && map_interior_transitions[transition_name].second != current_room_name)
	{
		current_room_name = map_interior_transitions[transition_name].second;
		return true;
	}
	if (map_interior_transitions[transition_name].first != current_room_name && map_interior_transitions[transition_name].second == current_room_name)
	{
		current_room_name = map_interior_transitions[transition_name].first;
		return true;
	}
	return false;
}
bool location_scheme::init(string NAME, string CURRENT_ROOM_NAME)
{
	v_room_names.clear();
	map_interior_transitions.clear();

	name = NAME;
	tmx_file transitions_tmx;
	if (!transitions_tmx.load_from_file("locations/" + name + "/initialization.tsv"))
		return false;
	if (transitions_tmx.get_v_general_tages_with_tage_name("rooms").size() != 1)
		return false;
	if (transitions_tmx.get_v_general_tages_with_tage_name("transitions_map").size() != 1)
		return false;
	for (int i(0); i < transitions_tmx.get_v_general_tages_with_tage_name("rooms")[0].get_v_subtages().size(); i++)
		if (!exists("locations/" + name + "/" + transitions_tmx.get_v_general_tages_with_tage_name("rooms")[0].get_v_subtages()[i].get_characteristic("name") + ".tmx"))
			return false;
		else
		{
			if (transitions_tmx.get_v_general_tages_with_tage_name("rooms")[0].get_v_subtages()[i].get_tage_name() != "room")
				return false;
			v_room_names.push_back(transitions_tmx.get_v_general_tages_with_tage_name("rooms")[0].get_v_subtages()[i].get_characteristic("name"));
		}

	tage transitions_map_tage = transitions_tmx.get_v_general_tages_with_tage_name("transitions_map")[0];
	if (transitions_map_tage.get_tage_name() != "transitions_map")
		return false;
	tage transition_tage;
	for (int i(0); i < transitions_map_tage.get_v_subtages().size(); i++)
	{
		transition_tage = transitions_map_tage.get_v_subtages()[i];
		if (transition_tage.get_tage_name() != "transition")
			return false;
		if (map_interior_transitions.find(transition_tage.get_characteristic("name")) != map_interior_transitions.end())
			return false;
		map_interior_transitions[transition_tage.get_characteristic("name")] = { transition_tage.get_characteristic("first"), transition_tage.get_characteristic("second") };
		//if (map_interior_transitions.find(map_interior_transitions[transition_tage.get_characteristic("name")].first) == map_interior_transitions.end())
		//	return false;
		bool ok = false;
		for (int j(0); j < v_room_names.size(); j++)
			if (v_room_names[j] == map_interior_transitions[transition_tage.get_characteristic("name")].first)
				ok = true;
		if (!ok)
			return false;
		ok = false;
		for (int j(0); j < v_room_names.size(); j++)
			if (v_room_names[j] == map_interior_transitions[transition_tage.get_characteristic("name")].second)
				ok = true;
		if (!ok)
			return false;
	}
	if (!start_in_room(CURRENT_ROOM_NAME))
		return false;
	return true;
}

/*bool location::init(string NAME)
{
	map_rooms.clear();
	map_transitions.clear();

	name = NAME;
	tmx_file transitions_tmx;
	if (!transitions_tmx.load_from_file("locations/" + name + "/initialization.tsv"))
		return false;
	if (transitions_tmx.get_v_general_tages_with_tage_name("rooms").size() != 1)
		return false;
	if (transitions_tmx.get_v_general_tages_with_tage_name("transitions_map").size() != 1)
		return false;
	for (int i(0); i < transitions_tmx.get_v_general_tages_with_tage_name("rooms")[0].get_v_subtages().size(); i++)
		if (!exists("locations/" + transitions_tmx.get_v_general_tages_with_tage_name("rooms")[0].get_v_subtages()[i].get_characteristic("name") + ".tsv"))
			return false;
		else
			map_rooms[transitions_tmx.get_v_general_tages_with_tage_name("rooms")[0].get_v_subtages()[i].get_characteristic("name")] = 
	path location_path = "locations/" + name;
	int t = 0;
	for (auto it(location_path.begin()); it != location_path.end(); it++)
		t++;
	//if (t-1 != map_room)

	tage transitions_map_tage = transitions_tmx.get_v_general_tages_with_tage_name("transitions_map")[0];
	if (transitions_map_tage.get_tage_name() != "transitions_map")
		return false;
	
	tage transition_tage;
	for (int i(0); i < transition_tage.get_v_subtages().size(); i++)
	{
		transition_tage = transition_tage.get_v_subtages()[i];
		if (transition_tage.get_tage_name() != "transition")
			return false;
		if (map_transitions.find(transition_tage.get_characteristic("name")) != map_transitions.end())
			return false;
		map_transitions[transition_tage.get_characteristic("name")] = { transition_tage.get_characteristic("first"), transition_tage.get_characteristic("second") };
		if (map_rooms.find(map_transitions[transition_tage.get_characteristic("name")].first) == map_rooms.end())
			return false;
		if (map_rooms.find(map_transitions[transition_tage.get_characteristic("name")].second) == map_rooms.end())
			return false;
	}

}*/