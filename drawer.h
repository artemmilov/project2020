#pragma once
#ifndef ANMTR_def
#define ANMTR_def

#include "general_terms_for_level_part.h"
#include "map.h"
#include "map_content.h"

class fone_drawer
{
private:
	sf::RenderWindow* p_window;

	std::vector<sf::Image> v_map_images;
	std::vector<sf::Texture> v_map_textures;
	std::vector<sf::Sprite> v_map_sprites;

	map_class* p_map;
	float tilewidth, tileheight;
	std::map <unsigned int, std::pair <unsigned int, sf::IntRect>> map_code;

	bool print_tile(unsigned int, float, float);
public:
	bool init(sf::RenderWindow*, map_class*);
	bool show(float, float);
};

struct entity_tile_data
{
	std::vector <sf::IntRect> v_tile_IntRect;
	std::pair <float, float> image_indent;
};
class dirt_drawer
{
private:
	std::string adress;

	sf::RenderWindow* p_window;

	sf::Image dirt_image;
	sf::Texture dirt_texture;
	sf::Sprite dirt_sprite;
	
	sf::Image mud_image;
	sf::Texture mud_texture;
	sf::Sprite mud_sprite;

	float image_indent_x, image_indent_y;
	int width, height;

	std::map <dirt_status_enum, entity_tile_data> map_statuses;
public:
	bool init(sf::RenderWindow*, std::string);
	bool show(dirt*);
};
class drop_drawer
{
private:
	std::string adress;

	sf::RenderWindow* p_window;

	sf::Image drop_image;
	sf::Texture drop_texture;
	sf::Sprite drop_sprite;

	float image_indent_x, image_indent_y;
	int width, height;

	std::map <drop_status_enum, entity_tile_data> map_statuses;
public:
	bool init(sf::RenderWindow*, std::string);
	bool show(drop*);
};

struct info_tile_data
{
	sf::IntRect rect;
	int indent_x, indent_y;
};
class game_info_drawer
{
private:
	sf::RenderWindow* p_window;

	sf::Image info_image;
	sf::Texture info_texture;
	sf::Sprite info_sprite;
	
	info_tile_data health_tile;

	int indent_x, indent_y;
public:
	bool init(sf::RenderWindow*);
	bool show(dirt*, float, float);
	bool show(drop*, float, float);
};

struct population_drawer
{
	dirt_drawer spark_drawer;
	drop_drawer waterfear_drawer;
};
class drawer
{
private:
	sf::RenderWindow* p_window;
	sf::View view;
	sf::Shader shader;

	fone_drawer fone_drawer_part;
	population_drawer population_drawer_part;
	game_info_drawer game_info_drawer_part;

	void set_view(map_class*, entity*);
public:
	bool init(sf::RenderWindow&, map_class&);
	bool show(map_class&, population&, level_mode_enum);
};


#endif