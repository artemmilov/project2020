#pragma once
#pragma once
#ifndef Drctr_def
#define Drctr_def


#include "general_terms_for_level_part.h"
#include "map.h"
#include "map_content.h"

class manual_controller
{
private:
	sf::Keyboard keyboard;
public:
	void drive(dirt*);
	void drive(drop*);
};
class patrol_controller
{
private:
public:
	void drive(dirt&);
	void drive(drop&);
};

class intellect
{
private:
	manual_controller player_controller;
public:
	void drive(map_class&, population&);
};


#endif