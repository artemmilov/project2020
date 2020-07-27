#include "general_terms_for_level_part.h"

using namespace std;

bool is_number(string str)
{
	if (str == "")
		return false;
	bool ok = true;
	bool was_point = false;
	if (str[0] != '-')
	{
		for (int i(0); i < str.size(); i++)
			if (str[i] < '0' || str[i] > '9')
				if (str[i] == '.' && !was_point)
					was_point = true;
				else
					ok = false;
	}
	else
	{
		if (str.size() == 1)
			ok = false;
		else
			for (int i(1); i < str.size(); i++)
				if (str[i] < '0' || str[i] > '9')
					if (str[i] == '.' && !was_point)
						was_point = true;
					else
						ok = false;
	}
	if (str[str.size() - 1] == '.')
		return false;
	return ok;
}
bool is_number(char a)
{
	return a >= '0' && a <= '9';
}

float min(float a, float b)
{
	if (a <= b)
		return a;
	else
		return b;
}
float max(float a, float b)
{
	if (a >= b)
		return a;
	else
		return b;
}

bool does_rectangles_intersects(std::pair<std::pair<float, float>, std::pair<float, float>> rect1, std::pair<std::pair<float, float>, std::pair<float, float>> rect2)
{
	float eps = 0.0001;
	pair <float, float> p1, p2;
	p1 = rect1.first;
	p2 = rect1.second;
	pair <float, float> q1, q2;
	q1 = rect2.first;
	q2 = rect2.second;
	float left = max(p1.first, q1.first);
	float right = min(p2.first, q2.first);
	float top = max(p1.second, q1.second);
	float bottom = min(p2.second, q2.second);
	if ((left < right || (abs(left - right) < eps)) && (top < bottom || abs(top - bottom) < eps))
		return true;
	return false;
}
//float find_distance_from_point_to_rectangle(std::pair<float, float> p, std::pair<std::pair<float, float>, std::pair<float, float>> rect)
//{
//	
//}

bool image::init(tage image_tage, string adress_path)
{
	source = "";

	if (image_tage.get_tage_name() != "image")
		return false;
	if (!is_number(image_tage.get_characteristic("width")) || !is_number(image_tage.get_characteristic("height")))
		return false;
	source = adress_path + "/../" + image_tage.get_characteristic("source");
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

bool tileset::init(tage tileset_tage, string adress_path)
{
	name = "";

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
	if (tileset_tage.get_v_subtages().size() != 1)
		return false;
	if (!tileset_image.init(tileset_tage.get_v_subtages()[0], adress_path))
		return false;
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
image& tileset::get_tileset_image()
{
	return tileset_image;
}

bool static_body::init(float X, float Y, float WIDTH, float HEIGHT)
{

	x = X;
	y = Y;
	if (WIDTH < 0 || HEIGHT < 0)
		return false;
	width = WIDTH;
	height = HEIGHT;

	return true;
}
float static_body::get_x()
{
	return x;
}
float static_body::get_y()
{
	return y;
}
float static_body::get_width()
{
	return width;
}
float static_body::get_height()
{
	return height;
}

bool body::init(float X, float Y, float WIDTH, float HEIGHT, float VX, float VY, float STICKINESS)
{
	x = X;
	y = Y;
	if (WIDTH < 0 || HEIGHT < 0)
		return false;
	width = WIDTH;
	height = HEIGHT;
	v_x = VX;
	v_y = VY;
	dist_left = dist_right = dist_up = dist_down = INFINITY;
	position = flying;
	if (STICKINESS < 0 || STICKINESS > 1)
		return false;
	stickiness = STICKINESS;
	standart_stickiness = STICKINESS;
	stick_clock.restart();

	return true;
}
float body::get_x()
{
	return x;
}
float body::get_y()
{
	return y;
}
float body::get_width()
{
	return width;
}
float body::get_height()
{
	return height;
}
float body::get_v_x()
{
	return v_x;
}
float body::get_v_y()
{
	return v_y;
}
float body::get_dist_left()
{
	return dist_left;
}
float body::get_dist_right()
{
	return dist_right;
}
float body::get_dist_up()
{
	return dist_up;
}
float body::get_dist_down()
{
	return dist_down;
}
void body::set_dist_left(float DL)
{
	dist_left = DL;
}
void body::set_dist_right(float DR)
{
	dist_right = DR;
}
void body::set_dist_up(float DU)
{
	dist_up = DU;
}
void body::set_dist_down(float DD)
{
	dist_down = DD;
}
position_enum body::get_position()
{
	return position;
}
float body::get_stickiness()
{
	return stickiness;
}
void body::set_speed(float VX, float VY)
{
	v_x = VX;
	v_y = VY;
}
void body::set_position(position_enum pos)
{
	position = pos;
}
void body::unstick()
{
	stickiness = 0;
	stick_clock.restart();
}
void body::update(float cur_time)
{
	float body_max_speed = 2500;
	if (sqrt(v_x *v_x + v_y * v_y) > body_max_speed)
	{
		float sin_angle = v_y / sqrt(v_x *v_x + v_y * v_y);
		float cos_angle = v_x / sqrt(v_x *v_x + v_y * v_y);
		v_y = sin_angle * body_max_speed;
		v_x = cos_angle * body_max_speed;
	}

	x += v_x * cur_time;
	y += v_y * cur_time;
	if (stick_clock.getElapsedTime().asMilliseconds() >= 10)
		stickiness = standart_stickiness;
	if (position == flying || position == on_ground)
		v_y += gravitation * cur_time;
	//if (position == stick_left)
	//	v_x -= INFINITY;
	//if (position == stick_right)
	//	v_x += INFINITY;
	//if (position == stick_up)
	//	v_y -= INFINITY;
	//if (position == in_left_up_angle)
	//{
	//	v_x -= dist_left;
	//	v_y -= dist_up;
	//}
	//if (position == in_left_down_angle)
	//{
	//	v_x -= dist_left;
	//	v_y += dist_down;
	//}
	//if (position == in_right_up_angle)
	//{
	//	v_x += dist_right;
	//	v_y -= dist_up;
	//}
	//if (position == in_right_down_angle)
	//{
	//	v_x += dist_right;
	//	v_y += dist_down;
	//}
	
}

bool static_circumstance::set(float X, float Y, float WIDTH, float HEIGHT)
{
	x = X;
	y = Y;
	if (WIDTH < 0 || HEIGHT < 0)
		return false;
	width = WIDTH;
	height = HEIGHT;

	return true;
}
float static_circumstance::get_x()
{
	return x;
}
float static_circumstance::get_y()
{
	return y;
}
float static_circumstance::get_width()
{
	return width;
}
float static_circumstance::get_height()
{
	return height;
}

bool circumstance::set(float X, float Y, float WIDTH, float HEIGHT, float VX, float VY, float VWIDTH, float VHEIGHT)
{
	x = X;
	y = Y;
	if (WIDTH < 0 || HEIGHT < 0)
		return false;
	width = WIDTH;
	height = HEIGHT;
	v_x = VX;
	v_y = VY;
	v_width = VWIDTH;
	v_height = VHEIGHT;

	return true;
}
float circumstance::get_x()
{
	return x;
}
float circumstance::get_y()
{
	return y;
}
float circumstance::get_width()
{
	return width;
}
float circumstance::get_height()
{
	return height;
}
float circumstance::get_v_x()
{
	return v_x;
}
float circumstance::get_v_y()
{
	return v_y;
}
float circumstance::get_v_width()
{
	return v_width;
}
float circumstance::get_v_height()
{
	return v_height;
}
void circumstance::update(float cur_time)
{
	x += v_x * cur_time;
	y += v_y * cur_time;
	width += v_width * cur_time;
	height += v_height * cur_time;
	if (width < 0 && height >= 0)
	{
		width = -width;
		x = x - width;
		v_width = -v_width;
	}
	if (width >= 0 && height < 0)
	{
		height = -height;
		y = y - height;
		v_height = -v_height;
	}
	if (width < 0 && height < 0)
	{
		width = -width;
		x = x - width;
		v_width = -v_width;
		height = -height;
		y = y - height;
		v_height = -v_height;
	}
}

template <class T>
map <unsigned int, T*>  unite_maps(std::vector <map <unsigned int, T*>> v_maps)
{
	map <unsigned int, T*> result_map;
	result_map.clear();

	for (int i(0); i < v_maps.size(); i++)
		for (auto T_it = v_maps[i].begin(); T_it != v_maps[i].end(); T_it++)
		{
			result_map[T_it->first] = T_it->second;
		}
	return result_map;
}