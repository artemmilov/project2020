#include "map.h"

using namespace std;

bool layer::init(tage layer_tage)
{
	layer_code.clear();
	name = "";

	if (layer_tage.get_tage_name() != "layer")
		return false;
	if (!is_number(layer_tage.get_characteristic("id")) || !is_number(layer_tage.get_characteristic("width")) || !is_number(layer_tage.get_characteristic("height")))
		return false;
	id = atoi(layer_tage.get_characteristic("id").c_str());
	name = layer_tage.get_characteristic("name");
	width = atoi(layer_tage.get_characteristic("width").c_str());
	height = atoi(layer_tage.get_characteristic("height").c_str());
	if (layer_tage.get_v_subtages().size() != 1)
		return false;
	layer_tage = layer_tage.get_v_subtages()[0];
	if (layer_tage.get_tage_name() != "data")
		return false;
	if (layer_tage.get_characteristic("encoding") != "csv")
		return false;
	string temp_layer_code = layer_tage.get_body();
	if (temp_layer_code.size() == 0)
		return false;
	char a = temp_layer_code[0];
	int t = 0;
	vector <int> add_vec;
	add_vec.clear();
	for (int i(0); i < width; i++)
		add_vec.push_back(-1000);
	for (int i(0); i < height; i++)
		layer_code.push_back(add_vec);

	int count = 0;
	string new_num = "";
	while (t < temp_layer_code.size())
	{
		a = temp_layer_code[t];
		if (is_number(a))
		{
			new_num += a;
		}
		else
		{
			layer_code[count / width][count % width] = atoi(new_num.c_str());
			new_num = "";
			count++;
		}
		t++;
	}
	layer_code[count / width][count % width] = atoi(new_num.c_str());
	count++;
	if (count / height != width && count % width != height)
		return false;

	return true;
}
int layer::get_id()
{
	return id;
}
string layer::get_name()
{
	return name;
}
int layer::get_width()
{
	return width;
}
int layer::get_height()
{
	return height;
}
int layer::get_tile_with_coordinate(int x, int y)
{
	if (y < 0 || y >= height || x < 0 || x >= width)
		return -1000;
	return layer_code[y][x];
}

bool object_rect::init(tage object_tage)
{
	if (object_tage.get_tage_name() != "object")
		return false;
	if (object_tage.get_v_subtages().size() != 0)
		return false;
	if (!is_number(object_tage.get_characteristic("x")) || !is_number(object_tage.get_characteristic("x")) || !is_number(object_tage.get_characteristic("y")) || !is_number(object_tage.get_characteristic("width")) || !is_number(object_tage.get_characteristic("height")) || !is_number(object_tage.get_characteristic("id")))
		return false;
	id = atoi(object_tage.get_characteristic("id").c_str());
	name = object_tage.get_characteristic("name");
	x = atoi(object_tage.get_characteristic("x").c_str());
	y = atoi(object_tage.get_characteristic("y").c_str());
	width = atoi(object_tage.get_characteristic("width").c_str());
	height = atoi(object_tage.get_characteristic("height").c_str());
	return true;
}
unsigned int object_rect::get_id()
{
	return id;
}
string object_rect::get_name()
{
	return name;
}
double object_rect::get_x()
{
	return x;
}
double object_rect::get_y()
{
	return y;
}
double object_rect::get_width()
{
	return width;
}
double object_rect::get_height()
{
	return height;
}

bool object_point::init(tage object_tage)
{
	if (object_tage.get_tage_name() != "object")
		return false;
	if (object_tage.get_v_subtages().size() != 1)
		return false;
	if (object_tage.get_v_subtages()[0].get_tage_name() != "point")
		return false;
	if (!is_number(object_tage.get_characteristic("id")) || !is_number(object_tage.get_characteristic("x")) || !is_number(object_tage.get_characteristic("y")) || !is_number(object_tage.get_characteristic("id")))
		return false;
	id = atoi(object_tage.get_characteristic("id").c_str());
	name = object_tage.get_characteristic("name");
	x = atoi(object_tage.get_characteristic("x").c_str());
	y = atoi(object_tage.get_characteristic("y").c_str());
	return true;
}
unsigned int object_point::get_id()
{
	return id;
}
string object_point::get_name()
{
	return name;
}
double object_point::get_x()
{
	return x;
}
double object_point::get_y()
{
	return y;
}

bool object_group::init(tage object_group_tage)
{
	name = "";
	v_object_rects.clear();
	v_object_points.clear();

	if (object_group_tage.get_tage_name() != "objectgroup")
		return false;
	if (!is_number(object_group_tage.get_characteristic("id")))
		return false;
	id = atoi(object_group_tage.get_characteristic("id").c_str());
	name = object_group_tage.get_characteristic("name");
	object_rect add_object_rect;
	object_point add_object_point;
	for (int i(0); i < object_group_tage.get_v_subtages().size(); i++)
	{
		if (object_group_tage.get_v_subtages()[i].get_v_subtages().size() == 0)
		{
			if(add_object_rect.init(object_group_tage.get_v_subtages()[i]))
				v_object_rects.push_back(add_object_rect);
			else
				return false;
		}
		else
		{
			if (add_object_point.init(object_group_tage.get_v_subtages()[i]))
				v_object_points.push_back(add_object_point);
			else
				return false;
		}
	}
	return true;
}
int object_group::get_id()
{
	return id;
}
string object_group::get_name()
{
	return name;
}
vector <object_rect>& object_group::get_v_object_rects()
{
	return v_object_rects;
}
vector <object_point>& object_group::get_v_object_points()
{
	return v_object_points;
}

bool dirt_damage::init(float X, float Y, float WIDTH, float HEIGHT, int DAMAGE_VALUE)
{
	if (!static_circumstance::set(X, Y, WIDTH, HEIGHT))
		return false;
	if (DAMAGE_VALUE < 0)
		return false;
	damage_value = DAMAGE_VALUE;
	return true;
}
int dirt_damage::get_damage_value()
{
	return damage_value;
}

bool dirt_replenish::init(float X, float Y, float WIDTH, float HEIGHT, int REPLENISH_VALUE)
{
	if (!static_circumstance::set(X, Y, WIDTH, HEIGHT))
		return false;
	if (REPLENISH_VALUE < 0)
		return false;
	replenish_value = REPLENISH_VALUE;
	return true;
}
int dirt_replenish::get_replenish_value()
{
	return replenish_value;
}

bool drop_damage::init(float X, float Y, float WIDTH, float HEIGHT, int DAMAGE_VALUE)
{
	if (!static_circumstance::set(X, Y, WIDTH, HEIGHT))
		return false;
	if (DAMAGE_VALUE < 0)
		return false;
	damage_value = DAMAGE_VALUE;
	return true;
}
int drop_damage::get_damage_value()
{
	return damage_value;
}

bool drop_replenish::init(float X, float Y, float WIDTH, float HEIGHT, int REPLENISH_VALUE)
{
	if (!static_circumstance::set(X, Y, WIDTH, HEIGHT))
		return false;
	if (REPLENISH_VALUE < 0)
		return false;
	replenish_value = REPLENISH_VALUE;
	return true;
}
int drop_replenish::get_replenish_value()
{
	return replenish_value;
}

bool room_transition::init(std::string NAME, float X, float Y, float WIDTH, float HEIGHT)
{
	if (!static_circumstance::set(X, Y, WIDTH, HEIGHT))
		return false;
	name = NAME;
	return true;
}
std::string room_transition::get_name()
{
	return name;
}

bool map_class::init(tmx_file INPUT_FILE, std::string adress_path)
{
	v_tilesets.clear();
	v_layers.clear();
	map_static_bodies.clear();
	dirt_circumstances.clear();
	drop_circumstances.clear();
	map_checkpoints.clear();
	map_room_transitions.clear();
	drive_circumstances.clear();
	map_born.clear();

	bool success_init_define, success_init_main;
	tage main_tage;
	if (INPUT_FILE.get_v_general_tages().size() != 1)
		return false;
	main_tage = INPUT_FILE.get_v_general_tages()[0];
	if (main_tage.get_tage_name() != "map")
		return false;
	vector <tage> temp_v_layers_tage, temp_v_object_groups_tage, temp_v_tilesets_tage;
	layer add_layer;
	object_group add_object_group;
	tileset add_tileset;
	temp_v_layers_tage = main_tage.get_v_subtages_with_name("layer");
	temp_v_object_groups_tage = main_tage.get_v_subtages_with_name("objectgroup");
	temp_v_tilesets_tage = main_tage.get_v_subtages_with_name("tileset");
	for (int i(temp_v_layers_tage.size() - 1); i >= 0; i--)
	{
		if (!add_layer.init(temp_v_layers_tage[i]))
			return false;
		v_layers.push_back(add_layer);
	}
	for (int i(temp_v_tilesets_tage.size() - 1); i >= 0; i--)
	{
		if (!add_tileset.init(temp_v_tilesets_tage[i], adress_path))
			return false;
		v_tilesets.push_back(add_tileset);
	}
	for (int i(temp_v_object_groups_tage.size() - 1); i >= 0; i--)
	{
		if (!add_object_group.init(temp_v_object_groups_tage[i]))
			return false;
		if (add_object_group.get_name() == "static_body")
		{
			if (add_object_group.get_v_object_points().size() != 0)
				return false;
			for (int i(0); i < add_object_group.get_v_object_rects().size(); i++)
			{
				static_body add_static_body;
				if (!add_static_body.init(add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height()))
					return false;
				map_static_bodies[add_object_group.get_v_object_rects()[i].get_id()] = add_static_body;
			}
		}
		if (add_object_group.get_name() == "dirt_circumstances")
		{
			if (add_object_group.get_v_object_points().size() != 0)
				return false;
			for (int i(0); i < add_object_group.get_v_object_rects().size(); i++)
			{
				if (add_object_group.get_v_object_rects()[i].get_name() == "damage")
				{
					dirt_damage add_dirt_damage;
					if (!add_dirt_damage.init(add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height(), 1))
						return false;
					dirt_circumstances.map_dirt_damages[add_object_group.get_v_object_rects()[i].get_id()] = add_dirt_damage;
				}
				if (add_object_group.get_v_object_rects()[i].get_name() == "replenish")
				{
					dirt_replenish add_dirt_replenish;
					if (!add_dirt_replenish.init(add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height(), 1))
						return false;
					dirt_circumstances.map_dirt_replenishes[add_object_group.get_v_object_rects()[i].get_id()] = add_dirt_replenish;
				}
				if (add_object_group.get_v_object_rects()[i].get_name() == "unstick")
				{
					dirt_unstick add_dirt_unstick;
					if (!add_dirt_unstick.set(add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height()))
						return false;
					dirt_circumstances.map_dirt_unsticks[add_object_group.get_v_object_rects()[i].get_id()] = add_dirt_unstick;
				}
			}
		}
		if (add_object_group.get_name() == "drop_circumstances")
		{
			if (add_object_group.get_v_object_points().size() != 0)
				return false;
			for (int i(0); i < add_object_group.get_v_object_rects().size(); i++)
			{
				if (add_object_group.get_v_object_rects()[i].get_name() == "damage")
				{
					drop_damage add_drop_damage;
					if (!add_drop_damage.init(add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height(), 1))
						return false;
					drop_circumstances.map_drop_damages[add_object_group.get_v_object_rects()[i].get_id()] = add_drop_damage;
				}
				if (add_object_group.get_v_object_rects()[i].get_name() == "replenish")
				{
					drop_replenish add_drop_replenish;
					if (!add_drop_replenish.init(add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height(), 1))
						return false;
					drop_circumstances.map_drop_replenishes[add_object_group.get_v_object_rects()[i].get_id()] = add_drop_replenish;
				}
			}
		}
		if (add_object_group.get_name() == "checkpoint")
		{
			if (add_object_group.get_v_object_points().size() != 0)
				return false;
			for (int i(0); i < add_object_group.get_v_object_rects().size(); i++)
			{
				if (!map_checkpoints[add_object_group.get_v_object_rects()[i].get_id()].set(add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height()))
					return false;
				if (add_object_group.get_v_object_rects()[i].get_name() == "start")
					player_start_id = add_object_group.get_v_object_rects()[i].get_id();
				if (add_object_group.get_v_object_rects()[i].get_name() == "finish")
					player_finish_id = add_object_group.get_v_object_rects()[i].get_id();
			}
		}
		if (add_object_group.get_name() == "room_transition")
		{
			if (add_object_group.get_v_object_points().size() != 0)
				return false;
			for (int i(0); i < add_object_group.get_v_object_rects().size(); i++)
			{
				if (!map_room_transitions[add_object_group.get_v_object_rects()[i].get_id()].init(add_object_group.get_v_object_rects()[i].get_name(), add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height()))
					return false;
			}
		}
		if (add_object_group.get_name() == "drive_curcumstances")
		{
			if (add_object_group.get_v_object_points().size() != 0)
				return false;
			for (int i(0); i < add_object_group.get_v_object_rects().size(); i++)
			{
				if (add_object_group.get_v_object_rects()[i].get_name() == "left_down_angle")
				{
					static_circumstance add_left_down_angle;
					if (!add_left_down_angle.set(add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height()))
						return false;
					drive_circumstances.map_left_down_angles[add_object_group.get_v_object_rects()[i].get_id()] = add_left_down_angle;
				}
				if (add_object_group.get_v_object_rects()[i].get_name() == "right_down_angle")
				{
					static_circumstance add_right_down_angle;
					if (!add_right_down_angle.set(add_object_group.get_v_object_rects()[i].get_x(), add_object_group.get_v_object_rects()[i].get_y(), add_object_group.get_v_object_rects()[i].get_width(), add_object_group.get_v_object_rects()[i].get_height()))
						return false;
					drive_circumstances.map_right_down_angles[add_object_group.get_v_object_rects()[i].get_id()] = add_right_down_angle;
				}
			}
		}
		if (add_object_group.get_name() == "born")
		{
			if (add_object_group.get_v_object_rects().size() != 0)
				return false;
			for (int i(0); i < add_object_group.get_v_object_points().size(); i++)
			{
				map_born[add_object_group.get_v_object_points()[i].get_id()] = { add_object_group.get_v_object_points()[i].get_name(), {add_object_group.get_v_object_points()[i].get_x(), add_object_group.get_v_object_points()[i].get_y()} };
			}
		}
	}

	if (main_tage.get_tage_name() != "map")
		return false;
	if (!is_number(main_tage.get_characteristic("width")) || !is_number(main_tage.get_characteristic("height")) || !is_number(main_tage.get_characteristic("tilewidth")) || !is_number(main_tage.get_characteristic("tileheight")) || !is_number(main_tage.get_characteristic("infinite")) || !is_number(main_tage.get_characteristic("nextlayerid")) || !is_number(main_tage.get_characteristic("nextobjectid")))
		return false;
	version = main_tage.get_characteristic("version");
	tiledversion = main_tage.get_characteristic("tiledversion");
	orientation = main_tage.get_characteristic("orientation");
	renderorder = main_tage.get_characteristic("renderorder");
	compressionlevel = atoi(main_tage.get_characteristic("compressionlevel").c_str());
	width = atoi(main_tage.get_characteristic("width").c_str());
	height = atoi(main_tage.get_characteristic("height").c_str());
	tilewidth = atoi(main_tage.get_characteristic("tilewidth").c_str());
	tileheight = atoi(main_tage.get_characteristic("tileheight").c_str());
	infinite = atoi(main_tage.get_characteristic("infinite").c_str());
	nextlayerid = atoi(main_tage.get_characteristic("nextlayerid").c_str());
	nextobjectid = atoi(main_tage.get_characteristic("nextobjectid").c_str());

	for (int i(0); i < v_tilesets.size(); i++)
		if (v_tilesets[i].get_tilewidth() != tilewidth || v_tilesets[i].get_tileheight() != tileheight)
			return false;
	return true;
}
bool map_class::load_from_file(string ADRESS)
{
	tmx_file map_file;
	if (!map_file.load_from_file(ADRESS))
		return false;
	if (!init(map_file, ADRESS))
		return false;
	return true;
}
std::string map_class::get_version()
{
	return version;
}
std::string map_class::get_tiledversion()
{
	return tiledversion;
}
std::string map_class::get_orientation()
{
	return orientation;
}
std::string map_class::get_renderorder()
{
	return renderorder;
}
int map_class::get_compressionlevel()
{
	return compressionlevel;
}
int map_class::get_width()
{
	return width;
}
int map_class::get_height()
{
	return height;
}
int map_class::get_tilewidth()
{
	return tilewidth;
}
int map_class::get_tileheight()
{
	return tileheight;
}
int map_class::get_infinite()
{
	return infinite;
}
int map_class::get_nextlayerid()
{
	return nextlayerid;
}
int map_class::get_nextobjectid()
{
	return nextobjectid;
}
vector <tileset>& map_class::get_v_tilesets()
{
	return v_tilesets;
}
vector <layer>& map_class::get_v_layers()
{
	return v_layers;
}
map <unsigned int, static_body>& map_class::get_map_static_bodies()
{
	return map_static_bodies;
}
dirt_circumstances_struct& map_class::get_dirt_circumstances()
{
	return dirt_circumstances;
}
drop_circumstances_struct& map_class::get_drop_circumstances()
{
	return drop_circumstances;
}
map <unsigned int, static_circumstance>& map_class::get_map_checkpoints()
{
	return map_checkpoints;
}
map <unsigned int, room_transition>& map_class::get_map_room_transitions()
{
	return map_room_transitions;
}
unsigned int map_class::get_player_start_id()
{
	return player_start_id;
}
unsigned int map_class::get_player_finish_id()
{
	return player_finish_id;
}
std::map <unsigned int, std::pair <std::string, std::pair<float, float>>>& map_class::get_map_born()
{
	return map_born;
}
drive_circumstances_struct& map_class::get_drive_circumstances()
{
	return drive_circumstances;
}