#include "core.h"

using namespace std;

std::map <unsigned int, dirt*> population::put_all_dirts_map()
{
	map <unsigned int, dirt*> res;
	res.clear();
	res[0] = &spark;
	return res;
}
std::map <unsigned int, drop*> population::put_all_drops_map()
{
	map <unsigned int, drop*> res;
	res.clear();

	for (auto it_drop = map_waterfears.begin(); it_drop != map_waterfears.end(); it_drop++)
		res[it_drop->first] = &(it_drop->second);
	return res;
}


bool core::start_in_coordinates(float X, float Y)
{
	room_population.map_waterfears.clear();

	if (!room_map.load_from_file("locations/" + current_location_scheme.get_name() + "/" + current_location_scheme.get_current_room_name() + ".tmx"))
		return false;
	for (auto it_born = room_map.get_map_born().begin(); it_born != room_map.get_map_born().end(); it_born++)
	{
		bool ok = false;
		if (it_born->second.first == "waterfear")
		{
			ok = true;
			if (!room_population.map_waterfears[it_born->first].init("entities/waterfear.tsv", it_born->second.second.first, it_born->second.second.second, it_born->first))
				return false;
		}
		if (!ok)
			return false;
	}
	if (!room_population.spark.init("entities/spark.tsv", X, Y, 0))
		return false;
	if (!room_drawer.init(*p_window, room_map))
		return false;
	level_mode = level_mode_enum::play;
	return true;
}
bool core::start_in_coordinates_in_other_room(float X, float Y, string new_room_name)
{
	if (!current_location_scheme.start_in_room(new_room_name))
			return false;
	if (!start_in_coordinates(X, Y))
		return false;
	return true;
}
bool core::interact_with_checkpoint()
{
	if (last_checkpoint_room_name == current_location_scheme.get_current_room_name())
	{
		if (room_map.get_map_checkpoints()[last_checkpoint_id].get_x() + room_map.get_map_checkpoints()[last_checkpoint_id].get_width() / 2.0 != last_checkpoint_coord.first)
			return false;
		if (room_map.get_map_checkpoints()[last_checkpoint_id].get_y() + room_map.get_map_checkpoints()[last_checkpoint_id].get_height() / 2.0 != last_checkpoint_coord.second)
			return false;
	}
	std::pair <std::pair<float, float>, std::pair<float, float>> rect1;
	std::pair <std::pair<float, float>, std::pair<float, float>> rect2;
	for (auto checkpt_it = room_map.get_map_checkpoints().begin(); checkpt_it != room_map.get_map_checkpoints().end(); checkpt_it++)
	{
		rect1 = { {room_population.spark.get_x(),room_population.spark.get_y()}, {room_population.spark.get_x() + room_population.spark.get_width(), room_population.spark.get_y() + room_population.spark.get_height()} };
		rect2 = { {checkpt_it->second.get_x(),checkpt_it->second.get_y()}, {checkpt_it->second.get_x() + checkpt_it->second.get_width(), checkpt_it->second.get_y() + checkpt_it->second.get_height()} };
		if (does_rectangles_intersects(rect1, rect2))
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
			{
				last_checkpoint_id = checkpt_it->first;
				last_checkpoint_coord.first = checkpt_it->second.get_x() + checkpt_it->second.get_width() / 2.0;
				last_checkpoint_coord.second = checkpt_it->second.get_y() + checkpt_it->second.get_height() / 2.0;
				last_checkpoint_room_name = current_location_scheme.get_current_room_name();
			}
	}
	return true;
}
bool core::interact_with_transitions()
{
	static bool open_to_transit = false;
	std::pair <std::pair<float, float>, std::pair<float, float>> rect1;
	std::pair <std::pair<float, float>, std::pair<float, float>> rect2;
	bool went = false;
	unsigned int transit_want_id;
	for (auto transit_it = room_map.get_map_room_transitions().begin(); (transit_it != room_map.get_map_room_transitions().end()) && !went; transit_it++)
	{
			rect1 = { {room_population.spark.get_x(),room_population.spark.get_y()}, {room_population.spark.get_x() + room_population.spark.get_width(), room_population.spark.get_y() + room_population.spark.get_height()} };
			rect2 = { {transit_it->second.get_x(),transit_it->second.get_y()}, {transit_it->second.get_x() + transit_it->second.get_width(), transit_it->second.get_y() + transit_it->second.get_height()} };
			if (does_rectangles_intersects(rect1, rect2))
			{
				transit_want_id = transit_it->first;
				went = true;
			}
	}
	if (went)
	{
		if (open_to_transit)
		{
			if (!transit_by_transition_with_id(transit_want_id))
				return false;
			else
				open_to_transit = false;
		}
	}
	else
		open_to_transit = true;
	return true;
}
bool core::transit_by_transition_with_id(unsigned int trans_id)
{
	string transition_name = room_map.get_map_room_transitions()[trans_id].get_name();
	if (room_map.get_map_room_transitions().find(trans_id) == room_map.get_map_room_transitions().end())
		return false;
	if (!current_location_scheme.transit_by_interior_transition(room_map.get_map_room_transitions()[trans_id].get_name()))
		return false;
	if (!room_map.load_from_file("locations/" + current_location_scheme.get_name() + "/" + current_location_scheme.get_current_room_name() + ".tmx"))
		return false;
	auto want_transit_it = room_map.get_map_room_transitions().begin();
	bool find = false;
	for (auto it(room_map.get_map_room_transitions().begin()); it != room_map.get_map_room_transitions().end(); it++)
	{
		if (it->second.get_name() == transition_name)
		{
			if (!find)
			{
				want_transit_it = it;
				find = true;
			}
			else
				return false;
		}
	}
	if (!start_in_coordinates(want_transit_it->second.get_x() + want_transit_it->second.get_width() / 2.0, want_transit_it->second.get_y() + want_transit_it->second.get_height() / 2.0))
		return false;
	return true;
}
bool core::restart()
{
	if (last_checkpoint_room_name == current_location_scheme.get_current_room_name())
	{
		if (room_map.get_map_checkpoints().find(last_checkpoint_id) == room_map.get_map_checkpoints().end())
			return false;
		if (room_map.get_map_checkpoints()[last_checkpoint_id].get_x() + room_map.get_map_checkpoints()[last_checkpoint_id].get_width() / 2.0 != last_checkpoint_coord.first)
			return false;
		if (room_map.get_map_checkpoints()[last_checkpoint_id].get_y() + room_map.get_map_checkpoints()[last_checkpoint_id].get_height() / 2.0 != last_checkpoint_coord.second)
			return false;
		if (!start_in_coordinates(last_checkpoint_coord.first, last_checkpoint_coord.second))
			return false;
	}
	else
	{
		if (!start_in_coordinates_in_other_room(last_checkpoint_coord.first, last_checkpoint_coord.second, last_checkpoint_room_name))
			return false;
	}
	return true;
}
bool core::start(sf::RenderWindow& window, string LOCATION_NAME, string ROOM_NAME, unsigned int ID_CHCKPT)
{
	room_population.map_waterfears.clear();

	p_window = &window;
	if (!current_location_scheme.init(LOCATION_NAME, ROOM_NAME))
		return false;
	if (!room_map.load_from_file("locations/" + current_location_scheme.get_name() + "/" + current_location_scheme.get_current_room_name() + ".tmx"))
		return false;
	last_checkpoint_room_name = current_location_scheme.get_current_room_name();
	last_checkpoint_id = ID_CHCKPT;
	if (room_map.get_map_checkpoints().find(last_checkpoint_id) == room_map.get_map_checkpoints().end())
		return false;
	last_checkpoint_coord.first = room_map.get_map_checkpoints()[last_checkpoint_id].get_x() + room_map.get_map_checkpoints()[last_checkpoint_id].get_width() / 2.0;
	last_checkpoint_coord.second = room_map.get_map_checkpoints()[last_checkpoint_id].get_y() + room_map.get_map_checkpoints()[last_checkpoint_id].get_height() / 2.0;
	if (!start_in_coordinates(last_checkpoint_coord.first, last_checkpoint_coord.second))
		return false;
	return true;
}
/*bool core::start_from_beginning(sf::RenderWindow& window, string NAME)
{
	name = NAME;
	if (!room_map.load_from_file("levels/" + name + ".tmx"))
		return false;
	if (!room_population.spark.init("entities/spark.tsv", room_map.get_map_checkpoints()[room_map.get_player_start_id()].get_x() + room_map.get_map_checkpoints()[room_map.get_player_start_id()].get_width() / 2.0, room_map.get_map_checkpoints()[room_map.get_player_start_id()].get_y() + room_map.get_map_checkpoints()[room_map.get_player_start_id()].get_height() / 2.0, 0))
		return false;
	for (auto it_born = room_map.get_map_born().begin(); it_born != room_map.get_map_born().end(); it_born++)
	{
		bool ok = false;
		if (it_born->second.first == "waterfear")
		{
			ok = true;
			if (!room_population.map_waterfears[it_born->first].init("entities/waterfear.tsv", it_born->second.second.first, it_born->second.second.second, it_born->first))
				return false;
		}
		if (!ok)
			return false;
	}
	if (!room_drawer.init(window, room_map))
		return false;
	current_checkpoint_id = room_map.get_player_start_id();
	level_mode = level_mode_enum::play;
	return true;
}*/
void core::play()
{
	level_mode = level_mode_enum::play;
}
void core::pause()
{
	level_mode = level_mode_enum::pause;
}
level_mode_enum core::get_level_mode()
{
	return level_mode;
}
bool core::update(float time)
{
	if (level_mode == level_mode_enum::play)
	{
		if (room_population.spark.is_alive())
		{
			if (!interact_with_transitions())
				return false;
			if (!interact_with_checkpoint())
				return false;
			if (!room_interactor.interact(room_map, room_population.put_all_dirts_map(), room_population.put_all_drops_map(), time))
				return false;
			brain.drive(room_map, room_population);
			if (!room_interactor.interact(room_map, room_population.put_all_dirts_map(), room_population.put_all_drops_map(), time))
				return false;
			if (!room_population.spark.update(time))
				return false;
			for (auto it_waterfear = room_population.map_waterfears.begin(); it_waterfear != room_population.map_waterfears.end(); it_waterfear++)
				if (!it_waterfear->second.update(time))
					return false;
			if (!room_drawer.show(room_map, room_population, level_mode_enum::play))
				return false;
		}
		else
		{
			restart();
		}
	}
	else
	{
		if (!room_drawer.show(room_map, room_population, level_mode_enum::pause))
			return false;
	}
	return true;
}

/*
std::string new_room::get_location_name()
{
	return location_name;
}
std::string new_room::get_name()
{
	return name;
}
map_class& new_room::get_room_map()
{
	return room_map;
}
dirt& new_room::get_spark()
{
	return spark;
}
std::map <unsigned int, drop>& new_room::get_map_waterfears()
{
	return map_waterfears;
}
bool new_room::expand(std::string LOCATION_NAME, std::string NAME)
{
	map_waterfears.clear();

	location_name = LOCATION_NAME;
	name = NAME;
	if (!room_map.load_from_file("locations/" + location_name + "/" + name + ".tsv"))
		return false;
	for (auto it_born = room_map.get_map_born().begin(); it_born != room_map.get_map_born().end(); it_born++)
	{
		bool ok = false;
		if (it_born->second.first == "waterfear")
		{
			ok = true;
			if (!map_waterfears[it_born->first].init("entities/waterfear.tsv", it_born->second.second.first, it_born->second.second.second, it_born->first))
				return false;
		}
		if (!ok)
			return false;
	}
	return true;
}
bool new_room::introduce_player_to_transition(std::string transition_name)
{
	auto want_transit_it = room_map.get_map_room_transitions().begin();
	bool find = false;
	for (auto it(room_map.get_map_room_transitions().begin()); it != room_map.get_map_room_transitions().end(); it++)
	{
		if (it->second.get_name() == transition_name)
			if (!find)
			{
				want_transit_it = it;
				find = true;
			}
			else
				return false;
	}
	if (!spark.init("entities/spark.tsv", want_transit_it->second.get_x() + want_transit_it->second.get_width() / 2.0, want_transit_it->second.get_y() + want_transit_it->second.get_height() / 2.0, 0))
		return false;
}
bool new_room::enter_room(std::string LOCATION_NAME, std::string NAME, std::string transition_name)
{
	location_name = LOCATION_NAME;
	name = NAME;
	if (!room_map.load_from_file("locations/" + location_name + "/" + name + ".tsv"))
		return false;
	int wanted_transition_id;
	auto want_transit_it = room_map.get_map_room_transitions().begin();
	bool find = false;
	for (auto it(room_map.get_map_room_transitions().begin()); it != room_map.get_map_room_transitions().end(); it++)
	{
		if (it->second.get_name() == transition_name)
			if (!find)
			{
				want_transit_it = it;
				find = true;
			}
			else
				return false;
	}
	if (!spark.init("entities/spark.tsv", want_transit_it->second.get_x() + want_transit_it->second.get_width() / 2.0, want_transit_it->second.get_y() + want_transit_it->second.get_height() / 2.0, 0))
		return false;
	for (auto it_born = room_map.get_map_born().begin(); it_born != room_map.get_map_born().end(); it_born++)
	{
		bool ok = false;
		if (it_born->second.first == "waterfear")
		{
			ok = true;
			if (!map_waterfears[it_born->first].init("entities/waterfear.tsv", it_born->second.second.first, it_born->second.second.second, it_born->first))
				return false;
		}
		if (!ok)
			return false;
	}
	return true;
}
bool new_room::update(float time)
{
	if (!spark.update(time))
		return false;
	for (auto it_waterfear = map_waterfears.begin(); it_waterfear != map_waterfears.end(); it_waterfear++)
		if (!it_waterfear->second.update(time))
			return false;
	return true;
}
*/