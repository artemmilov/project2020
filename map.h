#pragma once
#ifndef Mp_def
#define Mp_def

#include "general_terms_for_level_part.h"
#include "tmx_reader.h"

class layer
{
private:
	std::vector <std::vector <int>> layer_code;
	int id;
	std::string name;
	int width, height;

public:
	bool init(tage);

	int get_id();
	std::string get_name();
	int get_width();
	int get_height();
	int get_tile_with_coordinate(int, int);
};

class object_rect
{
private:
	unsigned int id;
	std::string name;
	double x, y;
	double width, height;
public:
	bool init(tage);

	unsigned int get_id();
	std::string get_name();
	double get_x();
	double get_y();
	double get_width();
	double get_height();
};
class object_point
{
private:
	unsigned int id;
	std::string name;
	double x, y;
public:
	bool init(tage);

	unsigned int get_id();
	std::string get_name();
	double get_x();
	double get_y();
};
class object_group
{
private:
	int id;
	std::string name;
	std::vector <object_rect> v_object_rects;
	std::vector <object_point> v_object_points;
public:
	bool init(tage);

	int get_id();
	std::string get_name();
	std::vector <object_rect>& get_v_object_rects();
	std::vector <object_point>& get_v_object_points();
};

class dirt_damage : public static_circumstance
{
private:
	int damage_value;
public:
	bool init(float, float, float, float, int);

	int get_damage_value();
};
class dirt_replenish : public static_circumstance
{
private:
	int replenish_value;
public:
	bool init(float, float, float, float, int);

	int get_replenish_value();
};
class dirt_unstick : public static_circumstance
{};
struct dirt_circumstances_struct
{
	std::map <unsigned int, dirt_damage> map_dirt_damages;
	std::map <unsigned int, dirt_replenish> map_dirt_replenishes;
	std::map <unsigned int, dirt_unstick> map_dirt_unsticks;

	void clear()
	{
		map_dirt_damages.clear();
		map_dirt_replenishes.clear();
		map_dirt_unsticks.clear();
	}
};

class drop_damage : public static_circumstance
{
private:
	int damage_value;
public:
	bool init(float, float, float, float, int);

	int get_damage_value();
};
class drop_replenish : public static_circumstance
{
private:
	int replenish_value;
public:
	bool init(float, float, float, float, int);

	int get_replenish_value();
};
struct drop_circumstances_struct
{
	std::map <unsigned int, drop_damage> map_drop_damages;
	std::map <unsigned int, drop_replenish> map_drop_replenishes;

	void clear()
	{
		map_drop_damages.clear();
		map_drop_replenishes.clear();
	}
};

struct drive_circumstances_struct
{
	std::map <unsigned int, static_circumstance> map_left_down_angles;
	std::map <unsigned int, static_circumstance> map_right_down_angles;

	void clear()
	{
		map_left_down_angles.clear();
		map_right_down_angles.clear();
	}
};

class room_transition : public static_circumstance
{
private:
	std::string name;
public:
	bool init(std::string, float, float, float, float);
	std::string get_name();
};

class map_class
{
private:
	std::string version;
	std::string tiledversion;
	std::string orientation;
	std::string renderorder;
	int compressionlevel;
	int width;
	int height;
	int tilewidth;
	int tileheight;
	int infinite;
	int nextlayerid;
	int nextobjectid;

	std::vector <tileset> v_tilesets;
	std::vector <layer> v_layers;
	std::map <unsigned int, static_body> map_static_bodies;
	dirt_circumstances_struct dirt_circumstances;
	drop_circumstances_struct drop_circumstances;
	std::map <unsigned int, static_circumstance> map_checkpoints;
	std::map <unsigned int, room_transition> map_room_transitions;
	drive_circumstances_struct drive_circumstances;
	unsigned int player_start_id;
	unsigned int player_finish_id;
	std::map <unsigned int, std::pair <std::string, std::pair<float, float>>> map_born;

	friend class mechanical_corrector;
	friend class dirt_interactor;
public:
	bool init(tmx_file, std::string);
	bool load_from_file(std::string);

	std::string get_version();
	std::string get_tiledversion();
	std::string get_orientation();
	std::string get_renderorder();
	int get_compressionlevel();
	int get_width();
	int get_height();
	int get_tilewidth();
	int get_tileheight();
	int get_infinite();
	int get_nextlayerid();
	int get_nextobjectid();
	std::vector <tileset>& get_v_tilesets();
	std::vector <layer>& get_v_layers();
	std::map <unsigned int, static_body>& get_map_static_bodies();
	dirt_circumstances_struct& get_dirt_circumstances();
	drop_circumstances_struct& get_drop_circumstances();
	std::map <unsigned int, static_circumstance>& get_map_checkpoints();
	std::map <unsigned int, room_transition>& get_map_room_transitions();
	drive_circumstances_struct& get_drive_circumstances();
	std::map <unsigned int, std::pair <std::string, std::pair<float, float>>>& get_map_born();
	unsigned int get_player_start_id();
	unsigned int get_player_finish_id();
};

#endif