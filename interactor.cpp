#include "interactor.h"

using namespace std;


bool mechanical_interactor::old_does_static_body_interfere_body_entity(static_body* p_sb, body* p_et)
{
	float eps = 0.0001;
	pair <float, float> p1, p2;
	p1.first = p_sb->get_x();
	p1.second = p_sb->get_y();
	p2.first = p_sb->get_x() + p_sb->get_width();
	p2.second = p_sb->get_y() + p_sb->get_height();
	pair <float, float> q1, q2;
	q1.first = p_et->get_x();
	q1.second = p_et->get_y();
	q2.first = p_et->get_x() + p_et->get_width();
	q2.second = p_et->get_y() + p_et->get_height();
	float left = max(p1.first, q1.first);
	float right = min(p2.first, q2.first);
	float top = max(p1.second, q1.second);
	float bottom = min(p2.second, q2.second);
	if ((left < right || (abs(left - right) < eps)) && (top < bottom || abs(top - bottom) < eps))
		return true;
	return false;
}
bool mechanical_interactor::old_can_go_x(map_class* p_game_mp, entity* p_et)
{
	bool ok = true;
	for (int i(0); i < v_actual_static_bodies_id.size(); i++)
	{
		float serv_x = min(p_et->get_x(), p_et->get_x() + p_et->get_v_x() * time);
		float serv_y = p_et->get_y();
		float serv_width = abs(p_et->get_v_x() * time) + p_et->get_width();
		float serv_height = p_et->get_height();
		body service_body;
		service_body.init(serv_x, serv_y, serv_width, serv_height, 0, 0, 0);
		if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &service_body))
			ok = false;
	}
	return ok;
}
bool mechanical_interactor::old_can_go_y(map_class* p_game_mp, entity* p_et)
{
	bool ok = true;
	for (int i(0); i < v_actual_static_bodies_id.size(); i++)
	{
		float serv_x = p_et->get_x();
		float serv_y = min(p_et->get_y(), p_et->get_y() + p_et->get_v_y() * time);
		float serv_width = p_et->get_width();
		float serv_height = abs(p_et->get_v_y() * time) + p_et->get_height();
		body service_body;
		service_body.init(serv_x, serv_y, serv_width, serv_height, 0, 0, 0);
		if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &service_body))
			ok = false;
	}
	return ok;
}
bool mechanical_interactor::old_can_go_xy(map_class* p_game_mp, entity* p_et)
{
	bool ok = true;
	for (int i(0); i < v_actual_static_bodies_id.size(); i++)
	{
		float serv_x = min(p_et->get_x(), p_et->get_x() + p_et->get_v_x() * time);
		float serv_y = min(p_et->get_y(), p_et->get_y() + p_et->get_v_y() * time);
		float serv_width = abs(p_et->get_v_x() * time) + p_et->get_width();
		float serv_height = abs(p_et->get_v_y() * time) + p_et->get_height();
		body service_body;
		service_body.init(serv_x, serv_y, serv_width, serv_height, 0, 0, 0);
		if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &service_body))
			ok = false;
	}
	return ok;
}
void mechanical_interactor::old_determ_position(map_class* p_game_mp, entity* p_et)
{
	bool can_left = true, can_right = true, can_up = true, can_down = true;
	body temp_bd_ent;
	static float near_body_eps_for_stick_down, near_body_eps_for_stick_left, near_body_eps_for_stick_right, near_body_eps_for_stick_up, near_body_eps_for_stick_max;
	
	if (p_et->get_position() == flying)
	{
		near_body_eps_for_stick_down = max(p_et->get_v_y() * time, 5);
		near_body_eps_for_stick_left = max(-p_et->get_v_x() * time, 5);
		near_body_eps_for_stick_right = max(p_et->get_v_x() * time, 5);
		near_body_eps_for_stick_up = max(-p_et->get_v_y() * time, 5);
	}
	else
	{
		near_body_eps_for_stick_down = max(near_body_eps_for_stick_down, 5);
		near_body_eps_for_stick_down = max(near_body_eps_for_stick_down, p_et->get_v_y() * time);
		near_body_eps_for_stick_left = max(near_body_eps_for_stick_left, 5);
		near_body_eps_for_stick_left = max(near_body_eps_for_stick_left, -p_et->get_v_x() * time);
		near_body_eps_for_stick_right = max(near_body_eps_for_stick_right, 5);
		near_body_eps_for_stick_right = max(near_body_eps_for_stick_right, p_et->get_v_x() * time);
		near_body_eps_for_stick_up = max(near_body_eps_for_stick_up, 5);
		near_body_eps_for_stick_up = max(near_body_eps_for_stick_up, -p_et->get_v_y() * time);
	}
	
	for (int i(0); i < v_actual_static_bodies_id.size(); i++)
	{
		temp_bd_ent.init(p_et->get_x() - near_body_eps_for_stick_left, p_et->get_y(), p_et->get_width(), p_et->get_height(), 0, 0, 0);
		if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &temp_bd_ent))
			can_left = false;
		temp_bd_ent.init(p_et->get_x() + near_body_eps_for_stick_right, p_et->get_y(), p_et->get_width(), p_et->get_height(), 0, 0, 0);
		if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &temp_bd_ent))
			can_right = false;
		temp_bd_ent.init(p_et->get_x(), p_et->get_y() - near_body_eps_for_stick_up, p_et->get_width(), p_et->get_height(), 0, 0, 0);
		if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &temp_bd_ent))
			can_up = false;
		temp_bd_ent.init(p_et->get_x(), p_et->get_y() + near_body_eps_for_stick_down, p_et->get_width(), p_et->get_height(), 0, 0, 0);
		if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &temp_bd_ent))
			can_down = false;
	}

	if (!can_down)
		can_up = true;
	if (!can_left)
		can_right = true;

	//if (p_et->is_sticky())
	//{
		if (can_left && can_right && can_up && can_down)
		{
			p_et->set_position(flying);
			for (int i(0); i < v_actual_static_bodies_id.size(); i++)
			{
				temp_bd_ent.init(p_et->get_x() - near_body_eps_for_stick_right, p_et->get_y() - near_body_eps_for_stick_down, p_et->get_width(), p_et->get_height(), 0, 0, 0);
				if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &temp_bd_ent))
					p_et->set_position(in_right_down_border);
				temp_bd_ent.init(p_et->get_x() - near_body_eps_for_stick_right, p_et->get_y() + near_body_eps_for_stick_up, p_et->get_width(), p_et->get_height(), 0, 0, 0);
				if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &temp_bd_ent))
					p_et->set_position(in_right_up_border);
				temp_bd_ent.init(p_et->get_x() + near_body_eps_for_stick_left, p_et->get_y() - near_body_eps_for_stick_down, p_et->get_width(), p_et->get_height(), 0, 0, 0);
				if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &temp_bd_ent))
					p_et->set_position(in_left_down_border);
				temp_bd_ent.init(p_et->get_x() + near_body_eps_for_stick_left, p_et->get_y() + near_body_eps_for_stick_down, p_et->get_width(), p_et->get_height(), 0, 0, 0);
				if (old_does_static_body_interfere_body_entity(&p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]], &temp_bd_ent))
					p_et->set_position(in_left_up_border);
			}
		}
		if (can_left && !can_right && can_up && can_down)
			p_et->set_position(stick_right);
		if (!can_left && can_right && can_up && can_down)
			p_et->set_position(stick_left);
		if (can_left && can_right && can_up && !can_down)
			p_et->set_position(on_ground);
		if (can_left && can_right && !can_up && can_down)
			p_et->set_position(stick_up);
		if (can_left && !can_right && can_up && !can_down)
			p_et->set_position(in_right_down_angle);
		if (!can_left && can_right && can_up && !can_down)
			p_et->set_position(in_left_down_angle);
		if (can_left && !can_right && !can_up && can_down)
			p_et->set_position(in_right_up_angle);
		if (!can_left && can_right && !can_up && can_down)
			p_et->set_position(in_left_up_angle);
	//}
	//else
	//{
	//	if (can_down)
	//		p_et->set_position(flying);
	//	if (!can_down)
	//		p_et->set_position(on_ground);
	//}
}
bool mechanical_interactor::old_handle(map_class* p_game_mp, entity* p_et)
{
	find_static_bodies_in_area(p_game_mp, p_et->get_x() + p_et->get_width() / 2, p_et->get_y() + p_et->get_height() / 2);
	if (!correct_position_and_set_distance(p_game_mp, p_et))
		return false;
	old_determ_position(p_game_mp, p_et);
	float speed_approx_rim = 100;
	bool can_x = old_can_go_x(p_game_mp, p_et);
	bool can_y = old_can_go_y(p_game_mp, p_et);
	if (!can_x)
		p_et->set_speed(0, p_et->matter_part.get_v_y());
	if (!can_y)
		p_et->set_speed(p_et->matter_part.get_v_x(), 0);
	if (can_x && can_y)
		if (!old_can_go_xy(p_game_mp, p_et))
			p_et->set_speed(0, 0);
	return true;
}


void mechanical_interactor::find_static_bodies_in_area(map_class* p_game_mp, float x0, float y0)
{
	v_actual_static_bodies_id.clear();
	
	float actual_dist = 250;
	std::pair<std::pair<float, float>, std::pair<float, float>> sensitive_rect;
	sensitive_rect.first = { x0 - actual_dist, y0 - actual_dist };
	sensitive_rect.second = { x0 + actual_dist, y0 + actual_dist };
	std::pair<std::pair<float, float>, std::pair<float, float>> current_body_rect;
	for (auto it = p_game_mp->get_map_static_bodies().begin(); it != p_game_mp->get_map_static_bodies().end(); it++)
	{
		current_body_rect.first = { it->second.get_x() , it->second.get_y() };
		current_body_rect.second = { it->second.get_x() + it->second.get_width(), it->second.get_y() + it->second.get_height() };
		if (does_rectangles_intersects(sensitive_rect, current_body_rect))
			v_actual_static_bodies_id.push_back(it->first);
	}
}
bool mechanical_interactor::correct_position_and_set_distance(map_class* p_game_mp, entity* p_et)
{
	bool correct = true;
	float new_dist_left = INFINITY;
	float new_dist_right = INFINITY;
	float new_dist_up = INFINITY;
	float new_dist_down = INFINITY;
	for (int i(0); i < v_actual_static_bodies_id.size() && correct; i++)
	{
		pair <float, float> a = { p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_x(), p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_y() };
		pair <float, float> b = { p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_x() + p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_width(), p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_y() + p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_height() };
		if ((a.second >= p_et->get_y()) && (a.second < p_et->get_y() + p_et->get_height()) || (b.second > p_et->get_y()) && (b.second <= p_et->get_y() + p_et->get_height()) || (a.second >= p_et->get_y()) && (b.second <= p_et->get_y() + p_et->get_height()) || (a.second <= p_et->get_y()) && (b.second >= p_et->get_y() + p_et->get_height()))
		{
			correct = false;
			if (b.first <= p_et->get_x())
			{
				new_dist_left = min(new_dist_left, p_et->get_x() - b.first);
				correct = true;
			}
			if (a.first >= p_et->get_x() + p_et->get_width())
			{
				new_dist_right = min(new_dist_right, a.first - p_et->get_x() - p_et->get_width());
				correct = true;
			}
		}
		if ((a.first >= p_et->get_x()) && (a.first < p_et->get_x() + p_et->get_width()) || (b.first > p_et->get_x()) && (b.first <= p_et->get_x() + p_et->get_width()) || (a.first <= p_et->get_x()) && (b.first >= p_et->get_x() + p_et->get_width()))
		{
			correct = false;
			if (b.second <= p_et->get_y())
			{
				new_dist_up = min(new_dist_up, p_et->get_y() - b.second);
				correct = true;
			}
			if (a.second >= p_et->get_y() + p_et->get_height())
			{
				new_dist_down = min(new_dist_down, a.second - p_et->get_y() - p_et->get_height());
				correct = true;
			}
		}
	}
	p_et->matter_part.set_dist_left(new_dist_left);
	p_et->matter_part.set_dist_right(new_dist_right);
	p_et->matter_part.set_dist_up(new_dist_up);
	p_et->matter_part.set_dist_down(new_dist_down);

	return correct;
}
void mechanical_interactor::determ_position(map_class* p_game_mp, entity* p_et)
{
	bool can_left = true, can_right = true, can_up = true, can_down = true;
	if (p_et->get_dist_left() < near_eps)
		can_left = false;
	if (p_et->get_dist_right() < near_eps)
		can_right = false;
	if (p_et->get_dist_up() < near_eps)
		can_up = false;
	if (p_et->get_dist_down() < near_eps)
		can_down = false;

	if (!can_down)
		can_up = true;
	if (!can_left)
		can_right = true;

	//if (!p_et->is_sticky())
	//{
	//	if (can_down)
	//		p_et->set_position(flying);
	//	if (!can_down)
	//		p_et->set_position(on_ground);
	//}
	//else
	//{
		if (can_left && can_right && can_up && can_down)
			p_et->set_position(flying);
		if (can_left && !can_right && can_up && can_down)
			p_et->set_position(stick_right);
		if (!can_left && can_right && can_up && can_down)
			p_et->set_position(stick_left);
		if (can_left && can_right && can_up && !can_down)
			p_et->set_position(on_ground);
		if (can_left && can_right && !can_up && can_down)
			p_et->set_position(stick_up);
		if (can_left && !can_right && can_up && !can_down)
			p_et->set_position(in_right_down_angle);
		if (!can_left && can_right && can_up && !can_down)
			p_et->set_position(in_left_down_angle);
		if (can_left && !can_right && !can_up && can_down)
			p_et->set_position(in_right_up_angle);
		if (!can_left && can_right && !can_up && can_down)
			p_et->set_position(in_left_up_angle);


		if (p_et->get_dist_left() < near_eps)
			can_left = false;
		if (p_et->get_dist_right() < near_eps)
			can_right = false;
		if (p_et->get_dist_up() < near_eps)
			can_up = false;
		if (p_et->get_dist_down() < near_eps)
			can_down = false;
		if (can_left && can_right && can_up && can_down)
			for (int i(0); i < v_actual_static_bodies_id.size(); i++)
			{
				pair <float, float> a0 = { p_et->get_x(), p_et->get_y() };
				pair <float, float> b0 = { p_et->get_x() + p_et->get_width(), p_et->get_y() };
				pair <float, float> c0 = { p_et->get_x() + p_et->get_width(), p_et->get_y() + p_et->get_height() };
				pair <float, float> d0 = { p_et->get_x(), p_et->get_y() + p_et->get_height() };
				pair <float, float> a1 = { p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_x(), p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_y() };
				pair <float, float> b1 = { p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_x() + p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_width(), p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_y() };
				pair <float, float> c1 = { p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_x() + p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_width(), p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_y() + p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_height() };
				pair <float, float> d1 = { p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_x(), p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_y() + p_game_mp->get_map_static_bodies()[v_actual_static_bodies_id[i]].get_height() };

				float new_v_x = p_et->get_v_x(), new_v_y = p_et->get_v_y();
				if ((abs(a0.first - c1.first) < near_eps) && (abs(a0.second - c1.second) < near_eps))
				{
					p_et->set_position(in_right_down_border);
					if (p_et->get_v_x() >= 0)
						new_v_x = (c1.first - a0.first) / time;
					else
						new_v_x = p_et->get_v_x();
					if (p_et->get_v_y() >= 0)
						new_v_y = (c1.second - a0.second) / time;
					else
						new_v_y = p_et->get_v_y();
				}
				if ((abs(b0.first - d1.first) < near_eps) && (abs(b0.second - d1.second) < near_eps))
				{
					p_et->set_position(in_left_down_border);
					if (p_et->get_v_x() <= 0)
						new_v_x = (d1.first - b0.first) / time;
					else
						new_v_x = p_et->get_v_x();
					if (p_et->get_v_y() >= 0)
						new_v_y = (d1.second - b0.second) / time;
					else
						new_v_y = p_et->get_v_y();
				}
				if ((abs(c0.first - a1.first) < near_eps) && (abs(c0.second - a1.second) < near_eps))
				{
					p_et->set_position(in_left_up_border);
					if (p_et->get_v_x() <= 0)
						new_v_x = (a1.first - c0.first) / time;
					else
						new_v_x = p_et->get_v_x();
					if (p_et->get_v_y() <= 0)
						new_v_y = (a1.second - c0.second) / time;
					else
						new_v_y = p_et->get_v_y();
				}
				if ((abs(d0.first - b1.first) < near_eps) && (abs(d0.second - b1.second) < near_eps))
				{
					p_et->set_position(in_right_up_border);
					if (p_et->get_v_x() >= 0)
						new_v_x = (b1.first - d0.first) / time;
					else
						new_v_x = p_et->get_v_x();
					if (p_et->get_v_y() <= 0)
						new_v_y = (b1.second - d0.second) / time;
					else
						new_v_y = p_et->get_v_y();
				}
				p_et->set_speed(new_v_x, new_v_y);
			}
	//}
}
bool mechanical_interactor::handle(map_class* p_game_mp, entity* p_et)
{
	find_static_bodies_in_area(p_game_mp, p_et->get_x() + p_et->get_width() / 2, p_et->get_y() + p_et->get_height() / 2);
	if (!correct_position_and_set_distance(p_game_mp, p_et))
		return false;
	determ_position(p_game_mp, p_et);
	float new_v_x, new_v_y;
	if (p_et->get_v_x() < 0)
		if (-p_et->get_v_x() * time >= p_et->get_dist_left())
			new_v_x = - p_et->get_dist_left() / time;
		else
			new_v_x = p_et->get_v_x();
	if (p_et->get_v_x() >= 0)
		if (p_et->get_v_x() * time >= p_et->get_dist_right())
			new_v_x = p_et->get_dist_right() / time;
		else
			new_v_x = p_et->get_v_x();
	if (p_et->get_v_y() < 0)
		if (-p_et->get_v_y() * time >= p_et->get_dist_up())
			new_v_y = - p_et->get_dist_up() / time;
		else
			new_v_y = p_et->get_v_y();
	if (p_et->get_v_y() >= 0)
		if (p_et->get_v_y() * time >= p_et->get_dist_down())
			new_v_y = p_et->get_dist_down() / time;
		else
			new_v_y = p_et->get_v_y();
	p_et->set_speed(new_v_x, new_v_y);
	return true;
}
bool mechanical_interactor::correct_speed(map_class* p_game_mp, map<unsigned int, entity*> map_entities, float cur_time)
{
	time = cur_time;
	for (auto it_entity = map_entities.begin(); it_entity != map_entities.end(); it_entity++)
	{
		if (!handle(p_game_mp, it_entity->second))
			return false;
	}
	return true;
}

void dirt_interactor::find_circumstances_in_area(map_class* p_game_mp, float x0, float y0)
{
	v_actual_dirt_damages_id.clear();
	v_actual_dirt_replenishes_id.clear();
	v_actual_dirt_unsticks_id.clear();

	float actual_dist = 500;
	std::pair<std::pair<float, float>, std::pair<float, float>> sensitive_rect;
	sensitive_rect.first = { x0 - actual_dist, y0 - actual_dist };
	sensitive_rect.second = { x0 + actual_dist, y0 + actual_dist };
	std::pair<std::pair<float, float>, std::pair<float, float>> current_body_rect;
	for (auto it = p_game_mp->get_dirt_circumstances().map_dirt_damages.begin(); it != p_game_mp->get_dirt_circumstances().map_dirt_damages.end(); it++)
	{
		current_body_rect.first = { it->second.get_x() , it->second.get_y() };
		current_body_rect.second = { it->second.get_x() + it->second.get_width(), it->second.get_y() + it->second.get_height() };
		if (does_rectangles_intersects(sensitive_rect, current_body_rect))
			v_actual_dirt_damages_id.push_back(it->first);
	}
	for (auto it = p_game_mp->get_dirt_circumstances().map_dirt_replenishes.begin(); it != p_game_mp->get_dirt_circumstances().map_dirt_replenishes.end(); it++)
	{
		current_body_rect.first = { it->second.get_x() , it->second.get_y() };
		current_body_rect.second = { it->second.get_x() + it->second.get_width(), it->second.get_y() + it->second.get_height() };
		if (does_rectangles_intersects(sensitive_rect, current_body_rect))
			v_actual_dirt_replenishes_id.push_back(it->first);
	}
	for (auto it = p_game_mp->get_dirt_circumstances().map_dirt_unsticks.begin(); it != p_game_mp->get_dirt_circumstances().map_dirt_unsticks.end(); it++)
	{
		current_body_rect.first = { it->second.get_x() , it->second.get_y() };
		current_body_rect.second = { it->second.get_x() + it->second.get_width(), it->second.get_y() + it->second.get_height() };
		if (does_rectangles_intersects(sensitive_rect, current_body_rect))
			v_actual_dirt_unsticks_id.push_back(it->first);
	}
}
bool dirt_interactor::does_circumstance_intersect_dirt(static_circumstance* p_cr, dirt* p_drt)
{
	float eps = 0.0001;
	pair <float, float> p1, p2;
	p1.first = p_cr->get_x();
	p1.second = p_cr->get_y();
	p2.first = p_cr->get_x() + p_cr->get_width();
	p2.second = p_cr->get_y() + p_cr->get_height();
	pair <float, float> q1, q2;
	q1.first = p_drt->get_x();
	q1.second = p_drt->get_y();
	q2.first = p_drt->get_x() + p_drt->get_width();
	q2.second = p_drt->get_y() + p_drt->get_height();
	float left = max(p1.first, q1.first);
	float right = min(p2.first, q2.first);
	float top = max(p1.second, q1.second);
	float bottom = min(p2.second, q2.second);
	if ((left < right || (abs(left - right) < eps)) && (top < bottom || abs(top - bottom) < eps))
		return true;
	return false;
}
void dirt_interactor::interact_with_circumstance(dirt_damage* p_dd, dirt* p_drt)
{
	if (does_circumstance_intersect_dirt(p_dd, p_drt))
	{
		p_drt->cause_damage(p_dd->get_damage_value());
	}
}
void dirt_interactor::interact_with_circumstance(dirt_replenish* p_dd, dirt* p_drt)
{
	if (does_circumstance_intersect_dirt(p_dd, p_drt))
	{
		p_drt->replenish_health(p_dd->get_replenish_value());
	}
}
void dirt_interactor::interact_with_circumstance(dirt_unstick* p_dd, dirt* p_drt)
{
	if (does_circumstance_intersect_dirt(p_dd, p_drt))
	{
		p_drt->unstick();
	}
}
void dirt_interactor::handle(map_class* p_game_mp, dirt* p_drt)
{
	find_circumstances_in_area(p_game_mp, p_drt->get_x(), p_drt->get_y());
	for (int i(0); i < v_actual_dirt_replenishes_id.size(); i++)
	{
		interact_with_circumstance(&p_game_mp->get_dirt_circumstances().map_dirt_replenishes[v_actual_dirt_replenishes_id[i]], p_drt);
	}
	for (int i(0); i < v_actual_dirt_damages_id.size(); i++)
	{
		interact_with_circumstance(&p_game_mp->get_dirt_circumstances().map_dirt_damages[v_actual_dirt_damages_id[i]], p_drt);
	}
	for (int i(0); i < v_actual_dirt_unsticks_id.size(); i++)
	{
		interact_with_circumstance(&p_game_mp->get_dirt_circumstances().map_dirt_unsticks[v_actual_dirt_unsticks_id[i]], p_drt);
	}
}
void dirt_interactor::provide_interaction(map_class* p_game_mp, map<unsigned int, dirt*> map_dirts)
{
	for (auto it_dirt = map_dirts.begin(); it_dirt != map_dirts.end(); it_dirt++)
		handle(p_game_mp, it_dirt->second);
}

bool interactor::interact(map_class& game_mp, std::map <unsigned int, dirt*> map_dirts, std::map <unsigned int, drop*> map_drops, float cur_time)
{
	map <unsigned int, entity*> map_entities;
	map_entities.clear();
	for (auto it_dirt = map_dirts.begin(); it_dirt != map_dirts.end(); it_dirt++)
		map_entities[it_dirt->first] = it_dirt->second;
	for (auto it_drop = map_drops.begin(); it_drop != map_drops.end(); it_drop++)
		map_entities[it_drop->first] = it_drop->second;

	if (!mechanical_part.correct_speed(&game_mp, map_entities, cur_time))
		return false;
	dirt_interactor_part.provide_interaction(&game_mp, map_dirts);
	return true;
}
