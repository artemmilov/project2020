#include "map_content.h"

using namespace std;

bool entity::init(tmx_file tmx_entity, float X, float Y, unsigned int ID)
{
	frame_clock.restart();

	if (tmx_entity.get_v_general_tages().size() != 1)
		return false;
	tage entity_tage = tmx_entity.get_v_general_tages()[0];
	if (entity_tage.get_tage_name() != "entity")
		return false;
	/*if (entity_tage.get_v_subtages().size() != 3)
		return false;*/
	
	bool ok = true;
	ok = ok && is_number(entity_tage.get_characteristic("max_health"));
	ok = ok && is_number(entity_tage.get_characteristic("run_speed"));
	ok = ok && is_number(entity_tage.get_characteristic("jump_speed"));
	if (!ok)
		return false;
	max_health = atoi(entity_tage.get_characteristic("max_health").c_str());
	run_speed = atoi(entity_tage.get_characteristic("run_speed").c_str());
	jump_speed = atoi(entity_tage.get_characteristic("jump_speed").c_str());

	if (entity_tage.get_v_subtages_with_name("body").size() != 1)
		return false;
	tage body_tage = entity_tage.get_v_subtages_with_name("body")[0];
	if (body_tage.get_v_subtages().size() != 0)
		return false;

	ok = is_number(body_tage.get_characteristic("width"));
	ok = ok && is_number(body_tage.get_characteristic("height"));
	ok = ok && is_number(body_tage.get_characteristic("sticky"));
	if (!ok)
		return false;
	if (!matter_part.init(X, Y, atoi(body_tage.get_characteristic("width").c_str()), atoi(body_tage.get_characteristic("height").c_str()), 0, 0, atoi(body_tage.get_characteristic("sticky").c_str())))
		return false;

	tilt = tilt_enum::down;
	dir = direct_enum::dir_zero;
	health = max_health;
	damage_clock.restart();
	alive = true;
	id = ID;

	if (entity_tage.get_v_subtages_with_name("statuses").size() != 1)
		return false;
	return true;
}
bool entity::load_from_file(string ADRESS, float X, float Y, unsigned int ID)
{
	tmx_file tmx_enemy;
	if (!tmx_enemy.load_from_file(ADRESS))
		return false;
	if(!init(tmx_enemy, X, Y, ID))
		return false;
	return true;
}
void entity::cause_damage(int DAMAGE)
{
	if (damage_clock.getElapsedTime().asSeconds() >= 0.5)
	{
		if (health - DAMAGE > 0)
			health -= DAMAGE;
		else
		{
			health = 0;
			alive = false;
		}
		damage_clock.restart();
	}
}
void entity::replenish_health(int ADD_HEALTH)
{
	if (health + ADD_HEALTH <= max_health)
		health += ADD_HEALTH;
	else
		health = max_health;
}
void entity::set_speed(float VX, float VY)
{
	matter_part.set_speed(VX, VY);
}
void entity::set_position(position_enum pos)
{
	matter_part.set_position(pos);
}
position_enum entity::get_position()
{
	return matter_part.get_position();
}
float entity::get_v_x()
{
	return matter_part.get_v_x();
}
float entity::get_v_y()
{
	return matter_part.get_v_y();
}
float entity::get_x()
{
	return matter_part.get_x();
}
float entity::get_y()
{
	return matter_part.get_y();
}
float entity::get_width()
{
	return matter_part.get_width();
}
float entity::get_height()
{
	return matter_part.get_height();
}
float entity::get_dist_right()
{
	return matter_part.get_dist_right();
}
float entity::get_dist_left()
{
	return matter_part.get_dist_left();
}
float entity::get_dist_up()
{
	return matter_part.get_dist_up();
}
float entity::get_dist_down()
{
	return matter_part.get_dist_down();
}
float entity::is_sticky()
{
	return matter_part.is_sticky();
}
tilt_enum entity::get_tilt()
{
	return tilt;
}
direct_enum entity::get_dir()
{
	return dir;
}
int entity::get_health()
{
	return health;
}
int entity::get_max_health()
{
	return max_health;
}
bool entity::is_alive()
{
	return alive;
}
float entity::get_run_speed()
{
	return run_speed;
}
float entity::get_jump_speed()
{
	return jump_speed;
}
unsigned int entity::get_id()
{
	return id;
}
void entity::run_left()
{
	if (alive)
	{
		//if (matter_part.get_position() == on_ground || matter_part.get_position() == stick_up || matter_part.get_position() == flying || matter_part.get_position() == in_right_down_angle || matter_part.get_position() == in_right_up_angle || matter_part.get_position() == in_right_down_border || matter_part.get_position() == in_right_up_border)
		if (matter_part.get_position() == on_ground || matter_part.get_position() == flying || matter_part.get_position() == in_right_down_angle/* || matter_part.get_position() == in_right_up_angle || matter_part.get_position() == in_right_down_border*/ || matter_part.get_position() == in_right_up_border)
			matter_part.set_speed(-run_speed, matter_part.get_v_y());
	}
}
void entity::run_right()
{
	if (alive)
	{
		if (matter_part.get_position() == on_ground/* || matter_part.get_position() == stick_up*/ || matter_part.get_position() == flying || matter_part.get_position() == in_left_down_angle/* || matter_part.get_position() == in_left_up_angle || matter_part.get_position() == in_left_down_border*/ || matter_part.get_position() == in_left_up_border)
			matter_part.set_speed(run_speed, matter_part.get_v_y());
	}
}
void entity::run_up()
{
	if (alive)
	{
		if (matter_part.get_position() == stick_left || matter_part.get_position() == stick_right || matter_part.get_position() == in_left_down_angle || matter_part.get_position() == in_right_down_angle);// || matter_part.get_position() == in_left_down_border || matter_part.get_position() == in_right_down_border)
			matter_part.set_speed(matter_part.get_v_x(), -run_speed);
	}
}
void entity::run_down()
{
	if (alive)
	{
		if (matter_part.get_position() == stick_left || matter_part.get_position() == stick_right/* || matter_part.get_position() == in_left_up_angle || matter_part.get_position() == in_right_up_angle*/ || matter_part.get_position() == in_left_up_border || matter_part.get_position() == in_right_up_border)
			matter_part.set_speed(matter_part.get_v_x(), run_speed);
	}
}
void entity::jump()
{
	if (alive)
	{
		float new_v_y;
		bool jumped = false;
		if (matter_part.get_position() == on_ground)
		{
			//new_v_y = -jumping_acceleration * time;
			matter_part.set_speed(matter_part.get_v_x(), -jump_speed);
			jumped = true;
		}
		/*if (matter_part.get_position() == stick_up)
		{
			matter_part.set_speed(matter_part.get_v_x(), jump_speed);
			jumped = true;
		}*/
		if (matter_part.get_position() == stick_left)
		{
			matter_part.set_speed(run_speed, -jump_speed);
			jumped = true;
		}
		if (matter_part.get_position() == stick_right)
		{
			matter_part.set_speed(-run_speed, -jump_speed);
			jumped = true;
		}
		/*if (matter_part.get_position() == in_left_down_border)
		{
			matter_part.set_speed(-run_speed, jump_speed);
			jumped = true;
		}*/
		if (matter_part.get_position() == in_left_up_border)
		{
			matter_part.set_speed(-run_speed, -jump_speed);
			jumped = true;
		}
		/*if (matter_part.get_position() == in_right_down_border)
		{
			matter_part.set_speed(run_speed, jump_speed);
			jumped = true;
		}*/
		if (matter_part.get_position() == in_right_up_border)
		{
			matter_part.set_speed(run_speed, -jump_speed);
			jumped = true;
		}
		if (jumped)
			matter_part.unstick();
	}
}
void entity::stay()
{
	if (alive)
	{
		if (matter_part.get_position() == on_ground/* || matter_part.get_position() == stick_up*/ || matter_part.get_position() == flying)
			matter_part.set_speed(0, matter_part.get_v_y());
		//if (matter_part.get_position() == stick_left || matter_part.get_position() == stick_right)
		//	matter_part.set_speed(matter_part.get_v_x(), (1 - matter_part.get_stickiness()) * run_speed);
		if (/*matter_part.get_position() == in_left_down_border ||*/ matter_part.get_position() == in_left_up_border/* || matter_part.get_position() == in_right_down_border*/ || matter_part.get_position() == in_right_up_border)
			matter_part.set_speed(0, 0);
	}
}
void entity::unstick()
{
	matter_part.unstick();
}
unsigned int entity::number_frame_of_all_frames(unsigned int  all_frames)
{
	int result = frame_clock.getElapsedTime().asMilliseconds() / 40;
	if (result >= all_frames)
	{
		result = 0;
		frame_clock.restart();
	}

	return result;
}
bool entity::update(float time)
{
	matter_part.update(time);
	if (matter_part.get_position() == flying || matter_part.get_position() == on_ground/* || matter_part.get_position() == in_right_down_border*/ || matter_part.get_position() == in_right_down_angle)
		tilt = tilt_enum::down;
	if (/*matter_part.get_position() == stick_up || */matter_part.get_position() == in_left_up_border)// || matter_part.get_position() == in_left_up_angle)
		tilt = tilt_enum::up;
	if (matter_part.get_position() == stick_left/* || matter_part.get_position() == in_left_down_border*/ || matter_part.get_position() == in_left_down_angle)
		tilt = tilt_enum::left;
	if (matter_part.get_position() == stick_right || matter_part.get_position() == in_right_up_border)// || matter_part.get_position() == in_right_up_angle)
		tilt = tilt_enum::right;
	
	if (matter_part.get_position() == on_ground)
	{
		if (matter_part.get_v_x() == 0 && dir == dir_zero)
			dir = dir_right;
		if (matter_part.get_v_x() < 0 && (dir == dir_right || dir == dir_zero))
			dir = dir_left;
		if (matter_part.get_v_x() > 0 && (dir == dir_left || dir == dir_zero))
			dir = dir_right;
	}
	else if (matter_part.get_position() == stick_left)
	{
		if (matter_part.get_v_y() == 0 && dir == dir_zero)
			dir = dir_right;
		if (matter_part.get_v_y() < 0 && (dir == dir_right || dir == dir_zero))
			dir = dir_left;
		if (matter_part.get_v_y() > 0 && (dir == dir_left || dir == dir_zero))
			dir = dir_right;
	}
	else if (matter_part.get_position() == stick_right)
	{
		if (matter_part.get_v_y() == 0 && dir == dir_zero)
			dir = dir_right;
		if (matter_part.get_v_y() > 0 && (dir == dir_right || dir == dir_zero))
			dir = dir_left;
		if (matter_part.get_v_y() < 0 && (dir == dir_left || dir == dir_zero))
			dir = dir_right;
	}
	/*else if (matter_part.get_position() == stick_up)
	{
		if (matter_part.get_v_x() == 0 && dir == dir_zero)
			dir = dir_right;
		if (matter_part.get_v_x() > 0 && (dir == dir_right || dir == dir_zero))
			dir = dir_left;
		if (matter_part.get_v_x() < 0 && (dir == dir_left || dir == dir_zero))
			dir = dir_right;
	}*/
	else if (matter_part.get_position() == in_left_down_angle/* || matter_part.get_position() == in_left_up_angle || matter_part.get_position() == in_right_up_angle*/ || matter_part.get_position() == in_right_down_angle/* || matter_part.get_position() == in_left_down_border */|| matter_part.get_position() == in_left_up_border || matter_part.get_position() == in_right_up_border)// || matter_part.get_position() == in_right_down_border)
	{
		if (dir == dir_zero)
			dir = dir_right;
	}
	else
		dir = dir_zero;
	return true;
}

bool mud::init(float GROUND_Y_POSITION, float X_LEFT, float X_RIGHT, float THICKNESS, float MAX_LENGTH, float LOSS_RATE, int DAMAGE_VALUE, dirt*p_DIRT)
{
	walk_history.clear();
	ground_y_position = GROUND_Y_POSITION;
	if (X_LEFT > X_RIGHT)
		return false;
	x_left = X_LEFT;
	x_right = X_RIGHT;
	if (THICKNESS < 0)
		return false;
	thickness = THICKNESS;
	max_length = MAX_LENGTH;
	min_left = -INFINITY;
	max_right = INFINITY;
	if (DAMAGE_VALUE < 0)
		return false;
	loss_rate = LOSS_RATE;
	damage_value = DAMAGE_VALUE;
	active = true;
	p_dirt = p_DIRT;
	if (p_dirt == nullptr)
		return false;
	if (p_dirt->get_width() > max_length)
		return false;
	walk_history.push_back({ x_left, x_right , dir_zero});
	
	if (!circumstance::set(x_left, ground_y_position + thickness, x_right - x_left, thickness, 0, 0, 0, 0))
		return false;
	
	return true;
}
int mud::get_damage_value()
{
	return damage_value;
}
bool mud::is_active()
{
	return active;
}
float mud::get_length()
{
	float result = 0;
	for (int i(0); i < walk_history.size(); i++)
		result += walk_history[i].right - walk_history[i].left;
	return result;
}
void mud::deactivate()
{
	active = false;
}
bool mud::update(float time)
{
	if (walk_history.size() == 0)
	{
		if (p_dirt->get_v_x() < 0)
			walk_history.push_back({ p_dirt->get_x(), p_dirt->get_x() + p_dirt->get_width(), dir_left});
		if (p_dirt->get_v_x() > 0)
			walk_history.push_back({ p_dirt->get_x(), p_dirt->get_x() + p_dirt->get_width(), dir_right });
		if (p_dirt->get_v_x() == 0)
			walk_history.push_back({ p_dirt->get_x(), p_dirt->get_x() + p_dirt->get_width(), dir_zero });
	}
	for (int i(0); i < walk_history.size(); i++)
		if (walk_history[i].left > walk_history[i].right)
			return false;

	if (active)
	{
		float left_last = walk_history[walk_history.size() - 1].left;
		float right_last = walk_history[walk_history.size() - 1].right;
		direct_enum dir_last = walk_history[walk_history.size() - 1].dir;

		if (dir_last == direct_enum::dir_zero)
		{
			if (p_dirt->get_v_x() < 0)
				dir_last = direct_enum::dir_left;
			if (p_dirt->get_v_x() > 0)
				dir_last = direct_enum::dir_right;
		}
		if (p_dirt->get_x() <= left_last)
			left_last = p_dirt->get_x();
		if (p_dirt->get_x() + p_dirt->get_width() >= right_last)
			right_last = p_dirt->get_x() + p_dirt->get_width();
		walk_history[walk_history.size() - 1] = { left_last, right_last, dir_last };

		if (p_dirt->get_v_x() < 0 && dir_last == direct_enum::dir_right)
			walk_history.push_back({ p_dirt->get_x(), p_dirt->get_x() + p_dirt->get_width(), direct_enum::dir_left });
		if (p_dirt->get_v_x() > 0 && dir_last == direct_enum::dir_left)
			walk_history.push_back({ p_dirt->get_x(), p_dirt->get_x() + p_dirt->get_width(), direct_enum::dir_right });
		
		if (p_dirt->get_position() == in_left_up_border)
			min_left = p_dirt->get_x() + p_dirt->get_width();
		if (p_dirt->get_position() == in_right_up_border)
			max_right = p_dirt->get_x();
	}

	for (int i(0); i < walk_history.size(); i++)
	{
		walk_history[i].left = max(walk_history[i].left, min_left);
		walk_history[i].right = max(walk_history[i].left, walk_history[i].right);
	}
	for (int i(0); i < walk_history.size(); i++)
	{
		walk_history[i].right = min(walk_history[i].right, max_right);
		walk_history[i].left = min(walk_history[i].left, walk_history[i].right);
	}

	if (!active)
	{
		float delta_length = loss_rate * time;
		int t = 0;
		while (delta_length != 0 && t < walk_history.size())
		{
			if (walk_history[t].right - walk_history[t].left < delta_length)
			{
				delta_length -= walk_history[t].right - walk_history[t].left;
				t++;
			}
			else
			{
				if (walk_history[t].dir == dir_left)
					walk_history[t].right -= delta_length;
				if (walk_history[t].dir == dir_right)
					walk_history[t].left += delta_length;
				if (walk_history[t].dir == dir_zero)
				{
					walk_history[t].left += delta_length / 2.0;
					walk_history[t].right -= delta_length / 2.0;
				}
				delta_length = 0;
			}
		}
		if (t >= 1)
			walk_history.erase(walk_history.begin(), walk_history.begin() + t);
	}
	else
	{
		float eps = 10;
		if (abs(p_dirt->get_v_x()) < eps)
		{ 
			float delta_length = loss_rate * time;
			int t = 0;
			while (delta_length != 0 && t < walk_history.size())
			{
				if (walk_history[t].right - walk_history[t].left < delta_length)
				{
					delta_length -= walk_history[t].right - walk_history[t].left;
					t++;
				}
				else
				{
					if (t == walk_history.size() - 1)
					{
						if (walk_history[t].dir == dir_left)
							walk_history[t].right = max(walk_history[t].right - delta_length, p_dirt->get_x() + p_dirt->get_width());
						if (walk_history[t].dir == dir_right)
							walk_history[t].left = min(walk_history[t].left + delta_length, p_dirt->get_x());
					}
					else
					{
						if (walk_history[t].dir == dir_left)
							walk_history[t].right -= delta_length;
						if (walk_history[t].dir == dir_right)
							walk_history[t].left += delta_length;
					}
					delta_length = 0;
				}
			}
			if (t >= 1)
				walk_history.erase(walk_history.begin(), walk_history.begin() + t);
		}
		else
		{
			float length = 0;
			float last_length = max_length;
			int t = -1;
			bool cont = true;
			for (int i(walk_history.size() - 1); i >= 0 && cont; i--)
			{
				length += walk_history[i].right - walk_history[i].left;
				if (length > max_length)
				{
					t = i;
					cont = false;
				}
				if (cont)
					last_length -= walk_history[i].right - walk_history[i].left;
			}
			if (t >= 0)
			{
				if (walk_history[t].dir == dir_left)
					walk_history[t].right = walk_history[t].left + last_length;
				if (walk_history[t].dir == dir_right)
					walk_history[t].left = walk_history[t].right - last_length;
			}
			if (t > 0)
				walk_history.erase(walk_history.begin(), walk_history.begin() + t);
		}
	}

	if (walk_history.size() != 0)
	{
		x_left = walk_history[0].left;
		x_right = walk_history[0].right;
		for (int i(0); i < walk_history.size(); i++)
		{
			x_left = min(x_left, walk_history[i].left);
			x_right = max(x_right, walk_history[i].right);
		}
		if (!circumstance::set(x_left, ground_y_position - thickness, x_right - x_left, thickness, 0, 0, 0, 0))
			return false;
	}
	else
		if (!circumstance::set(0, ground_y_position - thickness, 0, thickness, 0, 0, 0, 0))
			return false;

	return true;
}

bool dirt::init(std::string ADRESS, float X, float Y, unsigned int ID)
{
	tmx_file tmx_dirt;
	if (!tmx_dirt.load_from_file(ADRESS))
		return false;
	if (!entity::init(tmx_dirt, X, Y, ID))
		return false;
	status = dirt_fly;
	v_muds.clear();
	active_mud = false;
	return true;
}
dirt_status_enum dirt::get_status()
{
	return status;
}
std::vector <mud>& dirt::get_v_muds()
{
	return v_muds;
}
bool dirt::update(float time)
{
	for (int i(0); i < int(v_muds.size()) - 1; i++)
		if (v_muds[i].is_active())
			return false;
	if (v_muds.size() != 0)
		if (v_muds[v_muds.size() - 1].is_active() != active_mud)
			return false;
	if (v_muds.size() == 0 && active_mud)
		return false;
	if ((get_position() == on_ground || get_position() == in_left_down_angle || get_position() == in_right_down_angle) && !active_mud)
	{
		mud add_mud;
		entity::get_x();
		if (!add_mud.init(entity::get_y() + entity::get_height() + entity::get_dist_down(), entity::get_x(), entity::get_x() + entity::get_width(), 10, 150, 600, 1, this))
			return false;
		v_muds.push_back(add_mud);
		active_mud = true;
	}
	else
	{
		if ((get_position() != on_ground && get_position() != in_left_down_angle && get_position() != in_right_down_angle && get_position() != in_left_up_border && get_position() != in_right_up_border) && active_mud)
		{
			v_muds[v_muds.size() - 1].deactivate();
			active_mud = false;
		}
	}

	float delete_eps = 3;
	for (int i(0); i < v_muds.size(); i++)
	{
		if (!v_muds[i].update(time))
			return false;
		if (v_muds[i].get_length() <= delete_eps && !v_muds[i].is_active())
			v_muds.erase(v_muds.begin() + i);
	}
	
	if (!entity::update(time))
		return false;
	if (get_position() == flying)
		status = dirt_fly;
	if (get_position() == on_ground)// || get_position() == stick_up)
		if (get_v_x() != 0)
			status = dirt_run;
		else
			status = dirt_stick;
	if (get_position() == stick_left || get_position() == stick_right)
		if (get_v_y() != 0)
			status = dirt_run;
		else
			status = dirt_stick;
	if (get_position() == in_left_down_angle /*|| get_position() == in_left_up_angle*/ || get_position() == in_right_down_angle)// || get_position() == in_right_up_angle)
		status = dirt_in_corner;
	if (/*get_position() == in_left_down_border || */get_position() == in_left_up_border/* || get_position() == in_right_down_border*/ || get_position() == in_right_up_border)
		status = dirt_in_border;
	return true;
}

bool drop::init(std::string ADRESS, float X, float Y, unsigned int ID)
{
	tmx_file tmx_drop;
	if (!tmx_drop.load_from_file(ADRESS))
		return false;
	if (!entity::init(tmx_drop, X, Y, ID))
		return false;
	status = drop_calm;
	return true;
};
drop_status_enum drop::get_status()
{
	return status;
}
bool drop::update(float time)
{
	if (!entity::update(time))
		return false;
	if (get_position() == on_ground)
		if (get_v_x() != 0)
			status = drop_run;
		else
			status = drop_calm;
	else
		status = drop_calm;
	return true;
}