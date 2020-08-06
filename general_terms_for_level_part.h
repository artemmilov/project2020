#pragma once
#ifndef GTFL_def
#define GTFL_def


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <filesystem>

#include "tmx_reader.h"
#include "SFML/Graphics.hpp"

static const float from_pixels_to_meters = 1 / 3779.5751757502;
static const float from_meters_to_pixels = 3779.5751757502;
static const float gravitation = 9.8 * from_meters_to_pixels / 10;
static const float jumping_acceleration = 15 * from_meters_to_pixels / 10;
static const float rapprochement = 5000;
static const float resistance_factor = 0.1;
static const float slipping_speed = 300;

static enum position_enum {none_position, on_ground, stick_left, stick_right, in_left_down_angle, in_right_down_angle, in_left_up_border, in_right_up_border, flying};
static enum tilt_enum {up, down, left, right};
static enum direct_enum {dir_left, dir_right, dir_zero};
static enum level_mode_enum {play, pause, cut_scene};
//static enum sticking_degree_enum {unsticky, slip_side, stick_side, stick_everything};
	
bool is_number(std::string);
bool is_number(char);

float min(float, float);
float max(float, float);

bool does_rectangles_intersects(std::pair<std::pair<float, float>, std::pair<float, float>>, std::pair<std::pair<float, float>, std::pair<float, float>>);
//float find_distance_from_point_to_rectangle(std::pair<float, float>, std::pair<std::pair<float, float>, std::pair<float, float>>);

class image
{
private:
	std::string source;
	int width;
	int height;
public:
	bool init(tage, std::string);

	std::string get_source();
	int get_width();
	int get_height();
};
class tileset
{
private:
	int firstgid;
	std::string name;
	int tilewidth;
	int tileheight;
	int tilecount;
	int columns;
	image tileset_image;
public:
	bool init(tage, std::string);

	int get_firstgid();
	std::string get_name();
	int get_tilewidth();
	int get_tileheight();
	int get_tilecount();
	int get_columns();
	image& get_tileset_image();
};

class static_body
{
private:
	float x, y;
	float width, height;
public:
	bool init(float, float, float, float);
	
	float get_x();
	float get_y();
	float get_width();
	float get_height();
};
class body
{
private:
	float x, y;
	float width, height;
	float v_x, v_y;
	float dist_left, dist_right, dist_up, dist_down;
	position_enum position;
	bool sticky;
	sf::Clock stick_clock;

public:
	bool init(float, float, float, float, float, float, bool);
	
	float get_x();
	float get_y();
	float get_width();
	float get_height();
	float get_v_x();
	float get_v_y();
	float get_dist_left();
	float get_dist_right();
	float get_dist_up();
	float get_dist_down();
	void set_dist_left(float);
	void set_dist_right(float);
	void set_dist_up(float);
	void set_dist_down(float);
	position_enum get_position();
	bool is_sticky();

	void set_speed(float, float);
	void set_position(position_enum);
	void unstick();

	void update(float);
};

class static_circumstance
{
private:
	float x, y;
	float width, height;
public:
	bool set(float, float, float, float);

	float get_x();
	float get_y();
	float get_width();
	float get_height();
};
class circumstance
{
private:
	float x, y;
	float width, height;

	float v_x, v_y;
	float v_width, v_height;
public:
	bool set(float, float, float, float, float, float, float, float);

	float get_x();
	float get_y();
	float get_width();
	float get_height();
	float get_v_x();
	float get_v_y();
	float get_v_width();
	float get_v_height();

	void update(float);
};

template <typename T>
std::map <unsigned int, T*>  unite_maps(std::vector <std::map <unsigned int,T*>>);


#endif
