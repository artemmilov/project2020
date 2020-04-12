//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//							***map_tmx_reader***																						//
//Данный код преобразует сырой tmx файл, созданный в Tiled, в удобную структуру карты.													//
//Графика здесь никак не задействована. Программа просто разбивает текстовые данные на составляющие карты.								//
//Карта состоит из:																														//
//	1. Тайлсетов - картинок, из которых будем загружать тайлы (данный код ничего не загружает, он лишь помнит адрес картинки).			//
//	2. Слоёв тайлов - косметических слоёв карты, с которыми нет взаимодействия.															//
//	3. Слоёв объектов - слоёв невидимых для пользователя объектов с которыми происходит взаимодействие									//
//																																		//
//ВНИМАНИЕ!!!		ВНИМАНИЕ!!!		ВНИМАНИЕ!!!																							//
//--------->Программа не поддерживает обработку непрямоугольных объектов. Единственные возможные объекты								//
//			- прямоугольники с горизонтальными и вертикальными сторонами.																//
//																																		//
//--------->Работа с кириллицей также не поддерживается!																				//
//ВНИМАНИЕ!!!		ВНИМАНИЕ!!!		ВНИМАНИЕ!!!																							//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef map_tmx_reader
#define map_tmx_reader


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <stdlib.h>

//Все информацию из tmx файла разбиваем на теги. 
//Каждый тег соответствует какой-то состовляющей карты (слай тайлов, слой объектов, тайлсет, картинка из которой получаем тайлы и т.д.). 
//Но у них у всех есть общая структура.
class tage
{
private:
	std::string tage_name;
	std::string body;
	std::vector <tage> v_subtages;
	std::map <std::string, std::string> characteristics;
public:
	bool init(std::string);

	std::string get_tage_name();
	std::string get_characteristic(std::string);
	std::string get_body();
	std::vector <tage> get_v_subtages();
	std::vector <tage> get_v_subtages_with_name(std::string);
};

//Слой тайлов, формируется из соответствующего тега.
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

//Тайлсет, формируется из соответствующего тега. Содержит классы картинок, откуда берёт тайлы.
class image
{
private:
	std::string source;
	int width;
	int height;
public:
	bool init(tage);

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
	std::vector <image> v_images;
public:
	bool init(tage);

	int get_firstgid();
	std::string get_name();
	int get_tilewidth();
	int get_tileheight();
	int get_tilecount();
	int get_columns();
	std::vector <image> get_v_images();
};

//Слой объектов, формируется из соответствующего тега. Содержит классы объектов, которые в нём содержаться.
class object
{
private:
	double x, y;
	double width, height;
	int id;
public:
	bool init(tage);

	double get_x();
	double get_y();
	double get_width();
	double get_height();
};
class object_group
{
private:
	int id;
	std::string name;
	std::vector <object> v_objects;
public:
	bool init(tage);

	int get_id();
	std::string get_name();
	std::vector <object> get_v_objects();
};

//Класс всея карты. Состоит из слоёв тайлов, слоёв объектов и тайлсетов. Формируется из tmx файла.
class game_map
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
	std::vector <object_group> v_object_groups;
public:
	bool init(std::string);

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
	std::vector <tileset> get_v_tilesets();
	std::vector <layer> get_v_layers();
	std::vector <object_group> get_v_object_groups();
};


#endif
