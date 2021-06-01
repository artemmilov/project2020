#pragma once
#ifndef MCntnt_def
#define MCntnt_def
#pragma warning(disable : 2365)


#include "general_terms_for_level_part.h"
#include "tmx_reader.h"

static enum entity_status_enum { entity_calm, entity_run, entity_jump, entity_turn, entity_fly, entity_stick, entity_in_corner, entity_in_border };
class entity
{
private:
	body matter_part;
	tilt_enum tilt;
	direct_enum dir;
	int health, max_health;
	float run_speed, jump_speed;
	float jump_pulse;
	sf::Clock jump_clock;
	sf::Clock damage_clock;
	bool alive;

	unsigned int id;

	entity_status_enum status;

	sf::Clock frame_clock;

	friend class mechanical_interactor;
	friend class dirt_interactor;

	unsigned int jumping_up_counter;

	void cause_damage(int);
	void replenish_health(int);
	void set_speed(float, float);
	void set_position(position_enum);
public:
	bool init(tmx_file, float, float, unsigned int);
	bool load_from_file(std::string, float, float, unsigned int);

	position_enum get_position();
	float get_v_x();
	float get_v_y();
	float get_x();
	float get_y();
	float get_width();
	float get_height();
	float get_dist_left();
	float get_dist_right();
	float get_dist_up();
	float get_dist_down();
	float is_sticky();
	tilt_enum get_tilt();
	direct_enum get_dir();
	int get_health();
	int get_max_health();
	bool is_alive();
	float get_run_speed();
	float get_jump_speed();
	unsigned int get_id();
	entity_status_enum get_status();

	void run_left();
	void run_right();
	void run_up();
	void run_down();
	bool jump();
	void stay();
	void unstick();

	unsigned int number_frame_of_all_frames(unsigned int, unsigned int);

	bool update(float);
};

class dirt;
static enum dirt_status_enum { dirt_calm, dirt_run, dirt_jump, dirt_turn, dirt_fly, dirt_stick, dirt_in_corner, dirt_in_border };

struct mud_segment
{
	float left;
	float right;
	direct_enum dir;
};
class mud : public circumstance
{
private:
	//float x_checkbox;
	//float extinction_speed;
	
	float ground_y_position;
	float x_left, x_right;
	float thickness;
	
	std::vector<mud_segment> walk_history;
	float max_length;
	float min_left, max_right;
	float loss_rate;

	int damage_value;
	
	dirt* p_dirt;
	bool active;
public:
	bool init(float, float, float, float, float, float, int, dirt*);

	int get_damage_value();
	bool is_active();
	float get_length();

	void deactivate();

	bool update(float);
};

class dirt : public entity
{
private:
	dirt_status_enum status;
	std::vector <mud> v_muds;
	bool active_mud;
	
	bool translate_statuses_from_entity_to_dirt();
public:
	bool init(std::string, float, float, unsigned int);

	dirt_status_enum get_status();
	std::vector <mud>& get_v_muds();
	
	bool update(float);
};

static enum drop_status_enum { drop_calm, drop_run, drop_turn};
class drop : public entity
{
	drop_status_enum status;
public:
	bool init(std::string, float, float, unsigned int);

	drop_status_enum get_status();

	bool update(float);
};

struct population
{
	dirt spark;
	std::map <unsigned int, drop> map_waterfears;

	std::map <unsigned int, dirt*> put_all_dirts_map();
	std::map <unsigned int, drop*> put_all_drops_map();
};


#endif

