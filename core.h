#pragma once
#ifndef Rm_def
#define Rm_def

#include "general_terms_for_level_part.h"
#include "map.h"
#include "map_content.h"
#include "interactor.h"
#include "intellect.h"
#include "drawer.h"
#include "location_scheme.h"

class core
{
private:
	std::pair <float, float> last_checkpoint_coord;
	unsigned int last_checkpoint_id;	
	std::string last_checkpoint_room_name;
	level_mode_enum level_mode;


	sf::RenderWindow* p_window;

	map_class room_map;
	population room_population;

	intellect brain;						
	interactor room_interactor;				
	drawer room_drawer;						

	location_scheme current_location_scheme;

	bool start_in_coordinates(float, float);
	bool start_in_coordinates_in_other_room(float, float, std::string);

	bool interact_with_checkpoint();
	bool interact_with_transitions();
	bool transit_by_transition_with_id(unsigned int);

	bool restart();
public:
	bool start(sf::RenderWindow&, std::string, std::string, unsigned int);
	//bool start_from_beginning(sf::RenderWindow&, std::string);
	void play();
	void pause();

	level_mode_enum get_level_mode();

	//bool come_in_to_transition(std::string);
	bool update(float);
};

/*
class new_room
{
private:
	std::string location_name;
	std::string name;

	map_class room_map;
	
	dirt spark;
	std::map <unsigned int, drop> map_waterfears;
public:
	std::string get_location_name();
	std::string get_name();

	map_class& get_room_map();

	dirt& get_spark();
	std::map <unsigned int, drop>& get_map_waterfears();

	bool expand(std::string, std::string);
	bool introduce_player_to_transition(std::string);
	bool enter_room(std::string, std::string, std::string);
	bool update(float);
};
*/


#endif