#include "user.h"

using namespace std;
using namespace sf;

void user::interact_with_user()
{
	v_user_keyboard_commands.clear();
	if (keyboard.isKeyPressed(sf::Keyboard::D))
		v_user_keyboard_commands.push_back('D');
	if (keyboard.isKeyPressed(sf::Keyboard::A))
		v_user_keyboard_commands.push_back('A');
	if (keyboard.isKeyPressed(sf::Keyboard::W))
		v_user_keyboard_commands.push_back('W');
	if (keyboard.isKeyPressed(sf::Keyboard::S))
		v_user_keyboard_commands.push_back('S');
	if (keyboard.isKeyPressed(sf::Keyboard::Space))
		v_user_keyboard_commands.push_back(' ');
	is_left_mouse_button_pressed = mouse.isButtonPressed(mouse.Left);
	is_right_mouse_button_pressed = mouse.isButtonPressed(mouse.Right);
	mouse_x = mouse.getPosition().x;
	mouse_y = mouse.getPosition().y;
}

std::vector <char> user::get_v_user_keyboard_commands()
{
	return v_user_keyboard_commands;
}
bool user::get_is_left_mouse_button_pressed()
{
	return is_left_mouse_button_pressed;
}
bool user::get_is_right_mouse_button_pressed()
{
	return is_right_mouse_button_pressed;
}
float user::get_mouse_x()
{
	return mouse_x;
}
float user::get_mouse_y()
{
	return mouse_y;
}