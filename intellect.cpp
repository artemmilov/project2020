#include "intellect.h"

using namespace std;
using namespace sf;

void manual_controller::drive(dirt* p_dirt)
{
	p_dirt->stay();

	static bool wasZ, go_left, go_right, already_pushed_off;
	if (keyboard.isKeyPressed(Keyboard::Z))
	{
		if (p_dirt->get_position() == flying)
		{
			p_dirt->jump();
			already_pushed_off = true;
		}
		else
		{
			if (!already_pushed_off)
			{
				if (p_dirt->get_position() == stick_left || p_dirt->get_position() == in_right_up_border || p_dirt->get_position() == in_left_down_angle)
					go_right = true;
				if (p_dirt->get_position() == stick_right || p_dirt->get_position() == in_left_up_border || p_dirt->get_position() == in_right_down_angle)
					go_left = true;
				p_dirt->jump();
			}
		}
	}
	else
	{
		already_pushed_off = false;
		go_left = go_right = false;
	}
	if (p_dirt->get_position() != stick_right && p_dirt->get_position() != in_left_up_border && p_dirt->get_position() != in_right_down_angle && p_dirt->get_position() != flying)
		go_left = false;
	if (p_dirt->get_position() != stick_left && p_dirt->get_position() != in_right_up_border && p_dirt->get_position() != in_left_down_angle && p_dirt->get_position() != flying)
		go_right = false;
	if (go_left)
		p_dirt->run_left();
	if (go_right)
		p_dirt->run_right();

	if (keyboard.isKeyPressed(Keyboard::Right))
	{
		p_dirt->run_right();
		go_left = go_right = false;
	}
	if (keyboard.isKeyPressed(Keyboard::Left))
	{
		p_dirt->run_left();
		go_left = go_right = false;
	}
}
void manual_controller::drive(drop* p_drop)
{
	p_drop->stay();

	static bool wasZ, go_left, go_right;
	if (keyboard.isKeyPressed(Keyboard::Z))
	{
		if (!wasZ)
		{
			if (p_drop->get_position() == stick_left || p_drop->get_position() == in_right_up_border || p_drop->get_position() == in_left_down_angle)
			{
				go_right = true;
			}
			if (p_drop->get_position() == stick_right|| p_drop->get_position() == in_left_up_border || p_drop->get_position() == in_right_down_angle)
			{
				go_left = true;
			}
			p_drop->jump();
		}
		wasZ = true;
	}
	else
	{
		wasZ = false;
	}
	if (p_drop->get_position() != stick_right && p_drop->get_position() != in_left_up_border && p_drop->get_position() != flying)
		go_left = false;
	if (p_drop->get_position() != stick_left && p_drop->get_position() != in_right_up_border && p_drop->get_position() != flying)
		go_right = false;
	if (go_left)
		p_drop->run_left();
	if (go_right)
		p_drop->run_right();

	if (keyboard.isKeyPressed(Keyboard::Right))
	{
		p_drop->run_right();
		go_left = go_right = false;
	}
	if (keyboard.isKeyPressed(Keyboard::Left))
	{
		p_drop->run_left();
		go_left = go_right = false;
	}
	if (keyboard.isKeyPressed(Keyboard::Up))
	{
		p_drop->run_up();
	}
	if (keyboard.isKeyPressed(Keyboard::Down))
	{
		p_drop->run_down();
	}
}

void patrol_controller::drive(dirt& p_dirt)
{
}
void patrol_controller::drive(drop& p_drop)
{
	
}

void intellect::drive(map_class& map, population& population_control)
{
	player_controller.drive(&population_control.spark);
}



/*
bool director::update(map_class& map_level, vector<entity>& v_entities_in_level, vector <char> user_decision)
{
	char a;
	bool already_consider_player = false;
	bool was_D, was_A, was_W, was_S, was_SPACE;
	was_D = was_A = was_W = was_S = was_SPACE = false;
	for (int i(0); i < v_entities_in_level.size(); i++)
	{
		if (v_entities_in_level[i].get_type() == "player")
		{
			if (!already_consider_player)
			{
				for (int j(0); j < user_decision.size(); j++)
				{
					a = user_decision[j];
					if (a == 'D')
					{
						if (!was_D)
						{
							v_entities_in_level[i].run_right();
							was_D = true;
						}
						else
							return false;
					}
					if (a == 'A')
					{
						if (!was_A)
						{
							v_entities_in_level[i].run_left();
							was_A = true;
						}
						else
							return false;
					}
					if (a == 'W')
					{
						if (!was_W)
						{
							v_entities_in_level[i].
							was_W = true;
						}
						else
							return false;
					}
					if (a == 'S')
					{
						if (!was_S)
						{
							v_new_commands.push_back(run_down);
							was_S = true;
						}
						else
							return false;
					}
					if (a == ' ')
					{
						if (!was_SPACE)
						{
							v_new_commands.push_back(jump);
							was_SPACE = true;
						}
						else
							return false;
					}
					if (a != 'D' && a != 'A' && a != 'W' && a != 'S' && a != ' ' && a != '"')
						return false;
				}
				v_entities_in_level[i].set_v_commands(v_new_commands);
				already_consider_player = true;
			}
			else
				return false;
		}
	}
	return true;
}*/