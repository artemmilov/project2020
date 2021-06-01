#pragma once
#ifndef USR_def
#define USR_def

#include <SFML/Graphics.hpp>
#include "level.h"

class user
{
private:
	sf::Mouse mouse;
	sf::Keyboard keyboard;

	std::vector <char> v_user_keyboard_commands;
	bool is_left_mouse_button_pressed;
	bool is_right_mouse_button_pressed;
	float mouse_x, mouse_y;
public:
	void interact_with_user();

	std::vector <char> get_v_user_keyboard_commands();
	bool get_is_left_mouse_button_pressed();
	bool get_is_right_mouse_button_pressed();
	float get_mouse_x();
	float get_mouse_y();
};


#endif