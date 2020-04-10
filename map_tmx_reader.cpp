#include "map_tmx_reader.h"

using namespace std;

bool is_number(string str)
{
	bool ok = true;
	for (int i(0); i < str.size(); i++)
		if (str[i] < '0' || str[i] > '9')
			ok = false;
	if (str == "")
		ok = false;
	return ok;
}
bool is_number(char a)
{
	return a >= '0' && a <= '9';
}

bool tage::init(string body_tage)
{
	body = "";
	tage_name = "";
	v_subtages.clear();
	characteristics.clear();

	if (body_tage.size() == 0)
		return false;

	char a;
	int count_frame = 0;
	for (int i(0); i < body_tage.size(); i++)
	{
		a = body_tage[i];
		if (a == '>' || a == '<')
			count_frame++;
	}
	if (count_frame < 2)
		return false;
	if (count_frame == 2)
	{
		string head = "";

		for (int i(1); i < body_tage.size() - 1; i++)
		{
			a = body_tage[i];
			head += a;
		}

		tage_name = "";
		bool ok = false;
		int t = 0;
		while (!ok && t < head.size())
		{
			a = head[t];
			if (a != ' ')
				tage_name += a;
			else
				ok = true;
			t++;
		}
		if (tage_name[tage_name.size() - 1] == '/' && tage_name.size() == head.size())
		{
			ok = true;
			tage_name = tage_name.substr(0, tage_name.size() - 1);
		}
		if (!ok)
		{
			return false;
		}

		string current_characteristic = "", current_value = "";
		bool before_eq = true, read_value = false;
		ok = false;
		while (t < head.size())
		{
			while (!ok && t < head.size())
			{
				a = head[t];
				if (a == '=')
				{
					if (a == '=' && before_eq)
					{
						before_eq = false;
						if (t == head.size())
							return false;
						a = head[t + 1];
						if (a != '\"')
							return false;
						t++;
					}
					else
						return false;
				}
				else
				{
					if (a == '\"' && !before_eq)
					{
						ok = true;
						t++;
					}
					else
					{
						if (before_eq)
							current_characteristic += a;
						else
							current_value += a;
					}
				}
				t++;
			}

			if (!before_eq)
			{
				characteristics[current_characteristic] = current_value;
				current_characteristic = current_value = "";
			}
			before_eq = true;
			ok = false;
		}
		return true;
	}
	else
	{
		bool ok;
		bool find_name;
		string name_first = "", name_last = "";
		int body_first = 0, body_last = body_tage.size() - 1;


		a = body_tage[0];
		ok = false;
		find_name = true;
		if (a != '<')
			return false;
		body_first++;
		for (int i(1); !ok && i < body_tage.size(); i++)
		{
			a = body_tage[i];
			if (a == '>')
				ok = true;
			else
			{
				if (a == ' ')
					find_name = false;
				if (find_name)
					name_first += a;
			}
			body_first++;
		}
		if (!ok)
			return false;
		a = body_tage[body_tage.size() - 1];
		ok = false;
		if (a != '>')
			return false;
		body_last--;
		for (int i(body_tage.size() - 2); !ok && i >= 0; i--)
		{
			a = body_tage[i];
			if (a == '<')
				ok = true;
			else
				if (a != '/')
					name_last = a + name_last;
			body_last--;
		}
		if (!ok)
			return false;

		if (name_first != name_last)
			return false;
		body = body_tage.substr(body_first, body_last - body_first + 1);

		string head = "";
		bool in_head = true;
		for (int i(1); in_head && i < body_tage.size(); i++)
		{
			a = body_tage[i];
			if (a == '>')
				in_head = false;

			if (in_head)
				head += a;
		}


		tage_name = "";
		ok = false;
		int t = 0;
		while (!ok && t < head.size())
		{
			a = head[t];
			if (a != ' ')
				tage_name += a;
			else
				ok = true;
			t++;
		}
		if (!ok)
			return false;
		string current_characteristic = "", current_value = "";
		bool before_eq = true, read_value = false;
		ok = false;
		while (t < head.size())
		{
			while (!ok && t < head.size())
			{
				a = head[t];
				if (a == '=')
				{
					if (a == '=' && before_eq)
					{
						before_eq = false;
						if (t == head.size())
							return false;
						a = head[t + 1];
						if (a != '\"')
							return false;
						t++;
					}
					else
						return false;
				}
				else
				{
					if (a == '\"' && !before_eq)
					{
						ok = true;
						t++;
					}
					else
					{
						if (before_eq)
							current_characteristic += a;
						else
							current_value += a;
					}
				}
				t++;
			}

			if (!before_eq)
			{
				characteristics[current_characteristic] = current_value;
				current_characteristic = current_value = "";
			}
			before_eq = true;
			ok = false;
		}


		string find_tage_name = "";
		string current_tage_name = "";
		string body_subtage = "";
		string current_string;
		vector <string> v_add_tags_texts;
		bool under = false;
		t = body.size() - 1;
		int k = body.size() - 1;
		while (t >= 0)
		{
			a = body[t];
			if (!under)
			{
				if (a == '>')
				{
					current_string = "";
					k = t - 1;
					t--;
					while (a != '<' && t >= 0)
					{
						a = body[t];
						if (a != '<')
							current_string = a + current_string;
						t--;
					}
					find_tage_name = "";
					body_subtage = "";
					a = current_string[0];
					if (a != '/')
					{
						k = 0;
						while (a != ' ' && k < current_string.size())
						{
							a = current_string[k];
							if (a != ' ')
								find_tage_name += a;
							k++;
						}
						v_add_tags_texts.push_back('<' + current_string + '>');
					}
					else
					{
						k = 1;
						while (a != ' ' && k < current_string.size())
						{
							a = current_string[k];
							if (a != ' ')
								find_tage_name += a;
							k++;
						}
						under = true;
						body_subtage = "<" + current_string + '>';
					}
				}
				else
				{
					t--;
				}
			}
			else
			{
				if (a == '>')
				{
					current_string = "";
					k = t - 1;
					t--;
					while (a != '<' && t >= 0)
					{
						a = body[t];
						if (a != '<')
							current_string = a + current_string;
						t--;
					}
					current_tage_name = "";
					a = current_string[0];
					k = 0;
					while (a != ' ' && k < current_string.size())
					{
						a = current_string[k];
						if (a != ' ')
							current_tage_name += a;
						k++;
					}
					if (current_tage_name == find_tage_name)
					{
						body_subtage = '<' + current_string + '>' + body_subtage;
						v_add_tags_texts.push_back(body_subtage);
						body_subtage = "";
						under = false;
					}
					else
					{
						body_subtage = '<' + current_string + '>' + body_subtage;
					}
				}
				else
				{
					body_subtage = a + body_subtage;
					t--;
				}
			}
		}


		tage add_tage;
		for (int i(0); i < v_add_tags_texts.size(); i++)
		{
			if (!add_tage.init(v_add_tags_texts[i]))
				return false;
			v_subtages.push_back(add_tage);
		}
		return true;
	}
}
string tage::get_tage_name()
{
	return tage_name;
}
string tage::get_characteristic(string name_characteristic)
{
	return characteristics[name_characteristic];
}
string tage::get_body()
{
	return body;
}
vector <tage> tage::get_v_subtages()
{
	return v_subtages;
}
vector <tage> tage::get_v_subtages_with_name(string find_tage_name)
{
	vector <tage> result;
	result.clear();
	for (int i(0); i < v_subtages.size(); i++)
		if (v_subtages[i].get_tage_name() == find_tage_name)
			result.push_back(v_subtages[i]);
	return result;
}

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
	while (t < temp_layer_code.size())
	{
		a = temp_layer_code[t];
		if (is_number(a))
		{
			layer_code[count / height][count % width] = a - 48;
			count++;
		}
		t++;
	}
	if (count / height != height && count % width != width)
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

bool image::init(tage image_tage)
{
	source = "";

	if (image_tage.get_tage_name() != "image")
		return false;
	if (!is_number(image_tage.get_characteristic("width")) || !is_number(image_tage.get_characteristic("height")))
		return false;
	source = image_tage.get_characteristic("source");
	width = atoi(image_tage.get_characteristic("width").c_str());
	height = atoi(image_tage.get_characteristic("height").c_str());
	return true;
}
string image::get_source()
{
	return source;
}
int image::get_width()
{
	return width;
}
int image::get_height()
{
	return height;
}

bool tileset::init(tage tileset_tage)
{
	name = "";
	v_images.clear();

	if (tileset_tage.get_tage_name() != "tileset")
		return false;
	if (!is_number(tileset_tage.get_characteristic("firstgid")) || !is_number(tileset_tage.get_characteristic("tilewidth")) || !is_number(tileset_tage.get_characteristic("tileheight")) || !is_number(tileset_tage.get_characteristic("tilecount")) || !is_number(tileset_tage.get_characteristic("columns")))
		return false;
	firstgid = atoi(tileset_tage.get_characteristic("firstgid").c_str());
	name = tileset_tage.get_characteristic("name");
	tilewidth = atoi(tileset_tage.get_characteristic("tilewidth").c_str());
	tileheight = atoi(tileset_tage.get_characteristic("tileheight").c_str());
	tilecount = atoi(tileset_tage.get_characteristic("tilecount").c_str());
	columns = atoi(tileset_tage.get_characteristic("columns").c_str());
	image add_image;
	for (int i(0); i < tileset_tage.get_v_subtages().size(); i++)
	{
		add_image.init(tileset_tage.get_v_subtages()[i]);
		v_images.push_back(add_image);
	}
	return true;
}
int tileset::get_firstgid()
{
	return firstgid;
}
string tileset::get_name()
{
	return name;
}
int tileset::get_tilewidth()
{
	return tilewidth;
}
int tileset::get_tileheight()
{
	return tileheight;
}
int tileset::get_tilecount()
{
	return tilecount;
}
int tileset::get_columns()
{
	return columns;
}
vector <image> tileset::get_v_images()
{
	return v_images;
}

bool object::init(tage object_tage)
{
	if (object_tage.get_tage_name() != "object")
		return false;
	if (!is_number(object_tage.get_characteristic("x")) || !is_number(object_tage.get_characteristic("y")) || !is_number(object_tage.get_characteristic("width")) || !is_number(object_tage.get_characteristic("height")) || !is_number(object_tage.get_characteristic("id")))
		return false;
	x = atoi(object_tage.get_characteristic("x").c_str());
	y = atoi(object_tage.get_characteristic("y").c_str());
	width = atoi(object_tage.get_characteristic("width").c_str());
	height = atoi(object_tage.get_characteristic("height").c_str());
	id = atoi(object_tage.get_characteristic("id").c_str());
	return true;
}
double object::get_x()
{
	return x;
}
double object::get_y()
{
	return y;
}
double object::get_width()
{
	return width;
}
double object::get_height()
{
	return height;
}

bool object_group::init(tage object_group_tage)
{
	name = "";
	v_objects.clear();

	if (object_group_tage.get_tage_name() != "objectgroup")
		return false;
	if (!is_number(object_group_tage.get_characteristic("id")))
		return false;
	id = atoi(object_group_tage.get_characteristic("id").c_str());
	name = object_group_tage.get_characteristic("name");
	object add_object;
	for (int i(0); i < object_group_tage.get_v_subtages().size(); i++)
	{
		add_object.init(object_group_tage.get_v_subtages()[i]);
		v_objects.push_back(add_object);
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
vector <object> object_group::get_v_objects()
{
	return v_objects;
}

bool game_map::init(string ADRESS)
{
	v_tilesets.clear();
	v_layers.clear();
	v_object_groups.clear();

	ifstream fin(ADRESS);
	string text = "", add_str;
	string define_tage_str = "", main_tage_str = "";
	
	while (!fin.eof())
	{
		getline(fin, add_str);
		text += add_str;
	}
	
	if (text.size() == 0)
		return false;
	char a = text[0];
	int t = 0;
	while (a != '>' && t < text.size())
	{
		a = text[t];
		define_tage_str += a;
		t++;
	}
	while (t < text.size())
	{
		a = text[t];
		main_tage_str += a;
		t++;
	}

	bool success_init_define, success_init_main;
	tage define_tage, main_tage;
	success_init_define = define_tage.init(define_tage_str);
	success_init_main = main_tage.init(main_tage_str);
	if (!success_init_define || !success_init_main)
		return false;
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
	for (int i(temp_v_object_groups_tage.size() - 1); i >= 0; i--)
	{
		if (!add_object_group.init(temp_v_object_groups_tage[i]))
			return false;
		v_object_groups.push_back(add_object_group);
	}
	for (int i(temp_v_tilesets_tage.size() - 1); i >= 0; i--)
	{
		if (!add_tileset.init(temp_v_tilesets_tage[i]))
			return false;
		v_tilesets.push_back(add_tileset);
	}

	if (main_tage.get_tage_name() != "map")
		return false;
	if (!is_number(main_tage.get_characteristic("compressionlevel")) || !is_number(main_tage.get_characteristic("width")) || !is_number(main_tage.get_characteristic("height")) || !is_number(main_tage.get_characteristic("tilewidth")) || !is_number(main_tage.get_characteristic("tileheight")) || !is_number(main_tage.get_characteristic("infinite")) || !is_number(main_tage.get_characteristic("nextlayerid")) || !is_number(main_tage.get_characteristic("nextobjectid")))
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

	return true;
}
std::string game_map::get_version()
{
	return version;
}
std::string game_map::get_tiledversion()
{
	return tiledversion;
}
std::string game_map::get_orientation()
{
	return orientation;
}
std::string game_map::get_renderorder()
{
	return renderorder;
}
int game_map::get_compressionlevel()
{
	return compressionlevel;
}
int game_map::get_width()
{
	return width;
}
int game_map::get_height()
{
	return height;
}
int game_map::get_tilewidth()
{
	return tilewidth;
}
int game_map::get_tileheight()
{
	return tileheight;
}
int game_map::get_infinite()
{
	return infinite;
}
int game_map::get_nextlayerid()
{
	return nextlayerid;
}
int game_map::get_nextobjectid()
{
	return nextobjectid;
}
vector <tileset> game_map::get_v_tilesets()
{
	return v_tilesets;
}
vector <layer> game_map::get_v_layers()
{
	return v_layers;
}
vector <object_group> game_map::get_v_object_groups()
{
	return v_object_groups;
}
