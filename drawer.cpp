#include "drawer.h"
#pragma warning(disable : 4996)

using namespace std;
using namespace sf;

bool fone_drawer::print_tile(unsigned int tile_code, float x, float y)
{
	if (map_code.find(tile_code) == map_code.end())
		return false;
	v_map_sprites[map_code[tile_code].first].setTextureRect(map_code[tile_code].second);
	v_map_sprites[map_code[tile_code].first].setPosition(x, y);
	p_window->draw(v_map_sprites[map_code[tile_code].first]);
	return true;
}
bool fone_drawer::init(RenderWindow* WINDOW, map_class* p_GM)
{
	v_map_images.clear();
	v_map_textures.clear();
	v_map_sprites.clear();

	p_window = WINDOW;
	p_map = p_GM;
	Image add_image;
	Texture add_texture;
	Sprite add_sprite;
	for (int i(0); i < p_GM->get_v_tilesets().size(); i++)
	{
		if (!add_image.loadFromFile(p_GM->get_v_tilesets()[i].get_tileset_image().get_source()))
			return false;
		v_map_images.push_back(add_image);
		if (!add_texture.loadFromImage(v_map_images[v_map_images.size() - 1]))
			return false;
		v_map_textures.push_back(add_texture);
		add_sprite.setTexture(v_map_textures[v_map_textures.size() - 1]);
		v_map_sprites.push_back(add_sprite);
	}
	if (v_map_textures.size() != p_GM->get_v_tilesets().size())
		return false;
	if (v_map_textures.size() != v_map_sprites.size())
		return false;
	tilewidth = p_GM->get_tilewidth();
	tileheight = p_GM->get_tileheight();
	if (p_GM->get_v_tilesets().size() == 0)
		return false;
	unsigned int t = 1;
	float x1, y1, delta_x, delta_y;
	for (unsigned int i(0); i < p_GM->get_v_tilesets().size(); i++)
		while (t - p_GM->get_v_tilesets()[i].get_firstgid() < p_GM->get_v_tilesets()[i].get_tilecount())
		{
			x1 = (t - p_GM->get_v_tilesets()[i].get_firstgid()) % p_GM->get_v_tilesets()[i].get_columns() * tilewidth;
			y1 = (t - p_GM->get_v_tilesets()[i].get_firstgid()) / p_GM->get_v_tilesets()[i].get_columns() * tileheight;
			map_code[t].first = i;
			map_code[t].second = IntRect(x1, y1, tilewidth, tileheight);
			t++;
		}
	return true;
}
bool fone_drawer::show(float x, float y)
{
	float draw_radius = 1500;
	for (unsigned int i(0); i < p_map->get_v_layers().size(); i++)
		for (unsigned int j(0); j < p_map->get_v_layers()[i].get_width(); j++)
			for (unsigned int k(0); k < p_map->get_v_layers()[i].get_height(); k++)
			{
				if (sqrt((j*tilewidth  - x) * (j*tilewidth - x) + (k * tileheight - y) * (k * tileheight - y)) < draw_radius)
					if (!print_tile(p_map->get_v_layers()[i].get_tile_with_coordinate(j, k), j*tilewidth, k * tileheight))
						return false;
			}
}

bool dirt_drawer::init(RenderWindow* WINDOW, std::string ADRESS)
{
	map_statuses.clear();

	adress = ADRESS;
	p_window = WINDOW;
	
	tmx_file dirt_tmx;
	if (!dirt_tmx.load_from_file(adress))
		return false;
	if (dirt_tmx.get_v_general_tages().size() != 1)
		return false;
	tage dirt_tage = dirt_tmx.get_v_general_tages()[0];
	if (dirt_tage.get_tage_name() != "entity")
		return false;
	
	if (dirt_tage.get_v_subtages_with_name("tileset").size() != 1)
		return false;
	tileset spark_tileset_temp;
	if (!spark_tileset_temp.init(dirt_tage.get_v_subtages_with_name("tileset")[0], adress))
		return false;
	width = spark_tileset_temp.get_tilewidth();
	height = spark_tileset_temp.get_tileheight();
	if (!dirt_image.loadFromFile(spark_tileset_temp.get_tileset_image().get_source()))
		return false;
	if (!dirt_texture.loadFromImage(dirt_image))
		return false;
	dirt_sprite.setTexture(dirt_texture);
	dirt_texture.setSmooth(true); //////////////////
	
	if (dirt_tage.get_v_subtages_with_name("statuses").size() != 1)
		return false;
	tage statuses_tage = dirt_tage.get_v_subtages_with_name("statuses")[0];
	if (statuses_tage.get_tage_name() != "statuses")
		return false;
	bool ok = false;
	for (int i(0); i < statuses_tage.get_v_subtages().size(); i++)
	{
		tage status_add_tage = statuses_tage.get_v_subtages()[i];
		if (status_add_tage.get_v_subtages().size() != 0)
			return false;
		if (!is_number(status_add_tage.get_characteristic("image_indent_x").c_str()) || !is_number(status_add_tage.get_characteristic("image_indent_y").c_str()))
			return false;
		if (status_add_tage.get_tage_name() == "")
			return false;	
		dirt_status_enum current_status;
		if (status_add_tage.get_tage_name() == "dirt_calm")
		{
			ok = true;
			current_status = dirt_calm;
		}
		if (status_add_tage.get_tage_name() == "dirt_run")
		{
			ok = true;
			current_status = dirt_run;
		}
		if (status_add_tage.get_tage_name() == "dirt_jump")
		{
			ok = true;
			current_status = dirt_jump;
		}
		if (status_add_tage.get_tage_name() == "dirt_turn")
		{
			ok = true;
			current_status = dirt_turn;
		}
		if (status_add_tage.get_tage_name() == "dirt_fly")
		{
			ok = true;
			current_status = dirt_fly;
		}
		if (status_add_tage.get_tage_name() == "dirt_stick")
		{
			ok = true;
			current_status = dirt_stick;
		}
		if (status_add_tage.get_tage_name() == "dirt_in_corner")
		{
			ok = true;
			current_status = dirt_in_corner;
		}
		if (status_add_tage.get_tage_name() == "dirt_in_border")
		{
			ok = true;
			current_status = dirt_in_border;
		}
		if (!ok)
			return false;
		ok = false;

		entity_tile_data add_entity_tile_data;
		add_entity_tile_data.v_tile_IntRect.clear();
		
		add_entity_tile_data.image_indent = { atoi(status_add_tage.get_characteristic("image_indent_x").c_str()), atoi(status_add_tage.get_characteristic("image_indent_y").c_str()) };
		
		string code_text = status_add_tage.get_body();
		int t = 0, count = 0, cur_num;
		string new_num = "";
		char a;
		while (t < code_text.size())
		{
			a = code_text[t];
			if (is_number(a))
			{
				new_num += a;
			}
			else
			{
				cur_num = atoi(new_num.c_str()) - spark_tileset_temp.get_firstgid();
				add_entity_tile_data.v_tile_IntRect.push_back(IntRect((cur_num % spark_tileset_temp.get_columns()) * width, (cur_num / spark_tileset_temp.get_columns()) * height, width, height));
				new_num = "";
				count++;
			}
			t++;
		}
		count++;
		cur_num = atoi(new_num.c_str()) - spark_tileset_temp.get_firstgid();
		add_entity_tile_data.v_tile_IntRect.push_back(IntRect((cur_num % spark_tileset_temp.get_columns()) * width, (cur_num / spark_tileset_temp.get_columns()) * height, width, height));
		map_statuses[current_status] = add_entity_tile_data;
	}

	if (dirt_tage.get_v_subtages_with_name("mud").size() != 1)
		return false;
	tage mud_tage = dirt_tage.get_v_subtages_with_name("mud")[0];
	if (mud_tage.get_tage_name() != "mud")
		return false;
	if (mud_tage.get_v_subtages_with_name("image").size() != 1)
		return false;
	if (!mud_image.loadFromFile("levels/" + mud_tage.get_v_subtages_with_name("image")[0].get_characteristic("source")))
		return false;
	if (!mud_texture.loadFromImage(mud_image))
		return false;
	mud_sprite.setTexture(mud_texture);

	return true;
}
bool dirt_drawer::show(dirt* p_spark)
{
	if (p_spark->get_width() != p_spark->get_height())
		return false;

	for (auto it_mud = p_spark->get_v_muds().begin(); it_mud != p_spark->get_v_muds().end(); it_mud++)
	{
		mud_sprite.setTextureRect(IntRect(0, 0, it_mud->get_width(), it_mud->get_height()));
		mud_sprite.setPosition(it_mud->get_x(), it_mud->get_y());
		p_window->draw(mud_sprite);
	}

	int current_frame = p_spark->number_frame_of_all_frames(map_statuses[p_spark->get_status()].v_tile_IntRect.size());
	if (current_frame >= map_statuses[p_spark->get_status()].v_tile_IntRect.size())
		return false;
	dirt_sprite.setTextureRect(map_statuses[p_spark->get_status()].v_tile_IntRect[current_frame]);
	
	float indent_x = 0, indent_y = 0;
	

	dirt_sprite.setScale(1, 1);
	dirt_sprite.setRotation(0);
	dirt_sprite.setOrigin(width / 2.0, height / 2.0);
	if (p_spark->get_dir() == dir_left)
	{
		if (p_spark->get_status() == dirt_run || p_spark->get_status() == dirt_calm || p_spark->get_status() == dirt_stick)
		{
			dirt_sprite.setScale(-1, 1);
			indent_x = p_spark->get_width() - map_statuses[p_spark->get_status()].image_indent.first - width + width / 2.0;
			indent_y = map_statuses[p_spark->get_status()].image_indent.second + height / 2.0;;
		}
		if (p_spark->get_status() == dirt_in_corner || p_spark->get_status() == dirt_in_border)
		{
			dirt_sprite.setRotation(90);
			dirt_sprite.setScale(1, -1);
			indent_x = map_statuses[p_spark->get_status()].image_indent.second + height / 2.0;
			indent_y = map_statuses[p_spark->get_status()].image_indent.first + width / 2.0;
		}
	}
	else
	{
		if (p_spark->get_status() == dirt_run || p_spark->get_status() == dirt_calm || p_spark->get_status() == dirt_stick)
		{
			indent_x = map_statuses[p_spark->get_status()].image_indent.first + width / 2.0;
			indent_y = map_statuses[p_spark->get_status()].image_indent.second + height / 2.0;
		}
		if (p_spark->get_status() == dirt_in_corner || p_spark->get_status() == dirt_in_border)
		{
			indent_x = map_statuses[p_spark->get_status()].image_indent.first + width / 2.0;
			indent_y = map_statuses[p_spark->get_status()].image_indent.second + height / 2.0;
		}
	}
	float temp_ind_x, temp_ind_y;
	switch (p_spark->get_tilt())
	{
	case tilt_enum::down:
		break;
	case tilt_enum::up:
		temp_ind_x = p_spark->get_height() - indent_x;
		temp_ind_y = p_spark->get_width() - indent_y;
		indent_x = temp_ind_x;
		indent_y = temp_ind_y;
		dirt_sprite.setRotation(dirt_sprite.getRotation() + 180);
		break;
	case tilt_enum::left:
		temp_ind_x = p_spark->get_width() - indent_y;
		indent_y = indent_x;
		indent_x = temp_ind_x;
		dirt_sprite.setRotation(dirt_sprite.getRotation() + 90);
		break;
	case tilt_enum::right:
		temp_ind_y = p_spark->get_width() - indent_x;
		indent_x = indent_y;
		indent_y = temp_ind_y;
		dirt_sprite.setRotation(dirt_sprite.getRotation() + 270);
		break;
	}
	if (p_spark->get_status() == dirt_fly)
	{
		if (p_spark->get_v_x() >= 0)
			dirt_sprite.setScale(1, 1);
		else
			dirt_sprite.setScale(1, -1);
		dirt_sprite.setRotation(0);
		dirt_sprite.setOrigin(-map_statuses[dirt_fly].image_indent.first + p_spark->get_width() / 2.0, -map_statuses[dirt_fly].image_indent.second + p_spark->get_height() / 2.0);
		dirt_sprite.setRotation((360 - atan2(-p_spark->get_v_y(), p_spark->get_v_x()) / (2 * 3.14) * 360));
	}

	dirt_sprite.setPosition(p_spark->get_x() + indent_x, p_spark->get_y() + indent_y);
	p_window->draw(dirt_sprite);
	return true;
}

bool drop_drawer::init(sf::RenderWindow* WINDOW, std::string ADRESS)
{
	map_statuses.clear();

	adress = ADRESS;
	p_window = WINDOW;

	tmx_file drop_tmx;
	if (!drop_tmx.load_from_file(adress))
		return false;
	if (drop_tmx.get_v_general_tages().size() != 1)
		return false;
	tage drop_tage = drop_tmx.get_v_general_tages()[0];
	if (drop_tage.get_tage_name() != "entity")
		return false;

	if (drop_tage.get_v_subtages_with_name("tileset").size() != 1)
		return false;
	tileset drop_tileset_temp;
	if (!drop_tileset_temp.init(drop_tage.get_v_subtages_with_name("tileset")[0], adress))
		return false;
	width = drop_tileset_temp.get_tilewidth();
	height = drop_tileset_temp.get_tileheight();
	if (!drop_image.loadFromFile(drop_tileset_temp.get_tileset_image().get_source()))
		return false;
	if (!drop_texture.loadFromImage(drop_image))
		return false;
	drop_sprite.setTexture(drop_texture);
	drop_texture.setSmooth(true); //////////////////

	if (drop_tage.get_v_subtages_with_name("statuses").size() != 1)
		return false;
	tage statuses_tage = drop_tage.get_v_subtages_with_name("statuses")[0];
	if (statuses_tage.get_tage_name() != "statuses")
		return false;
	bool ok = false;
	for (int i(0); i < statuses_tage.get_v_subtages().size(); i++)
	{
		tage status_add_tage = statuses_tage.get_v_subtages()[i];
		if (status_add_tage.get_v_subtages().size() != 0)
			return false;
		if (!is_number(status_add_tage.get_characteristic("image_indent_x").c_str()) || !is_number(status_add_tage.get_characteristic("image_indent_y").c_str()))
			return false;
		if (status_add_tage.get_tage_name() == "")
			return false;
		drop_status_enum current_status;
		if (status_add_tage.get_tage_name() == "drop_calm")
		{
			ok = true;
			current_status = drop_calm;
		}
		if (status_add_tage.get_tage_name() == "drop_run")
		{
			ok = true;
			current_status = drop_run;
		}
		if (status_add_tage.get_tage_name() == "drop_turn")
		{
			ok = true;
			current_status = drop_turn;
		}
		if (!ok)
			return false;
		ok = false;

		entity_tile_data add_entity_tile_data;
		add_entity_tile_data.v_tile_IntRect.clear();

		add_entity_tile_data.image_indent = { atoi(status_add_tage.get_characteristic("image_indent_x").c_str()), atoi(status_add_tage.get_characteristic("image_indent_y").c_str()) };

		string code_text = status_add_tage.get_body();
		int t = 0, count = 0, cur_num;
		string new_num = "";
		char a;
		while (t < code_text.size())
		{
			a = code_text[t];
			if (is_number(a))
			{
				new_num += a;
			}
			else
			{
				cur_num = atoi(new_num.c_str()) - drop_tileset_temp.get_firstgid();
				add_entity_tile_data.v_tile_IntRect.push_back(IntRect((cur_num % drop_tileset_temp.get_columns()) * width, (cur_num / drop_tileset_temp.get_columns()) * height, width, height));
				new_num = "";
				count++;
			}
			t++;
		}
		count++;
		cur_num = atoi(new_num.c_str()) - drop_tileset_temp.get_firstgid();
		add_entity_tile_data.v_tile_IntRect.push_back(IntRect((cur_num % drop_tileset_temp.get_columns()) * width, (cur_num / drop_tileset_temp.get_columns()) * height, width, height));
		map_statuses[current_status] = add_entity_tile_data;
	}

	return true;
}
bool drop_drawer::show(drop* p_drop)
{
	int current_frame = p_drop->number_frame_of_all_frames(map_statuses[p_drop->get_status()].v_tile_IntRect.size());
	if (current_frame >= map_statuses[p_drop->get_status()].v_tile_IntRect.size())
		return false;

	drop_sprite.setTextureRect(map_statuses[p_drop->get_status()].v_tile_IntRect[current_frame]);

	float indent_x = 0, indent_y = 0;


	drop_sprite.setScale(1, 1);
	drop_sprite.setRotation(0);
	drop_sprite.setOrigin(width / 2.0, height / 2.0);
	if (p_drop->get_dir() == dir_left)
	{
		drop_sprite.setScale(-1, 1);
		indent_x = p_drop->get_width() - map_statuses[p_drop->get_status()].image_indent.first - width + width / 2.0;
		indent_y = map_statuses[p_drop->get_status()].image_indent.second + height / 2.0;;
	}
	else
	{
		indent_x = map_statuses[p_drop->get_status()].image_indent.first + width / 2.0;
		indent_y = map_statuses[p_drop->get_status()].image_indent.second + height / 2.0;
	}

	drop_sprite.setPosition(p_drop->get_x() + indent_x, p_drop->get_y() + indent_y);
	p_window->draw(drop_sprite);
	return true;
}

bool game_info_drawer::init(RenderWindow* WINDOW)
{
	p_window = WINDOW;

	tmx_file info_tmx;
	if (!info_tmx.load_from_file("other/game_info.tsv"))
		return false;
	if (info_tmx.get_v_general_tages().size() != 1)
		return false;
	tage game_info_tage = info_tmx.get_v_general_tages()[0];
	if (game_info_tage.get_tage_name() != "game_info")
		return false;
	if (game_info_tage.get_v_subtages_with_name("image").size() != 1)
		return false;

	tage image_tage = game_info_tage.get_v_subtages_with_name("image")[0];
	image new_image;
	if (!new_image.init(image_tage, "other/game_info.tsv"))
		return false;
	if (!info_image.loadFromFile(new_image.get_source()))
		return false;
	if (!info_texture.loadFromImage(info_image))
		return false;
	//info_texture.setSmooth(true); ////////////////////////
	info_sprite.setTexture(info_texture);

	if (game_info_tage.get_v_subtages_with_name("health").size() != 1)
		return false;
	tage health_tage = game_info_tage.get_v_subtages_with_name("health")[0];
	if (!is_number(health_tage.get_characteristic("x")) || !is_number(health_tage.get_characteristic("y")) || !is_number(health_tage.get_characteristic("width")) || !is_number(health_tage.get_characteristic("height")) || !is_number(health_tage.get_characteristic("indent_x")) || !is_number(health_tage.get_characteristic("indent_y")))
		return false;
	health_tile.rect = IntRect(atoi(health_tage.get_characteristic("x").c_str()), atoi(health_tage.get_characteristic("y").c_str()), atoi(health_tage.get_characteristic("width").c_str()), atoi(health_tage.get_characteristic("height").c_str()));
	health_tile.indent_x = atoi(health_tage.get_characteristic("indent_x").c_str());
	health_tile.indent_y = atoi(health_tage.get_characteristic("indent_y").c_str());

	return true;
}
bool game_info_drawer::show(dirt* p_dirt, float view_lu_x, float view_lu_y)
{
	for (int i(0); i < p_dirt->get_health(); i++)
	{
		info_sprite.setTextureRect(health_tile.rect);
		info_sprite.setPosition(view_lu_x + health_tile.indent_x + i * health_tile.rect.width, view_lu_y + health_tile.indent_y);
		p_window->draw(info_sprite);
	}
	return true;
}
bool game_info_drawer::show(drop* p_drop, float view_lu_x, float view_lu_y)
{
	for (int i(0); i < p_drop->get_health(); i++)
	{
		info_sprite.setTextureRect(health_tile.rect);
		info_sprite.setPosition(view_lu_x + health_tile.indent_x + i * health_tile.rect.width, view_lu_y + health_tile.indent_y);
		p_window->draw(info_sprite);
	}
	return true;
}

void drawer::set_view(map_class* p_map, entity* p_player_spark)
{
	float delta_view_x = 0;
	float delta_view_y = 0;
	float view_x, view_y;
	view_x = p_player_spark->get_x();
	view_y = p_player_spark->get_y();
	if (p_player_spark->get_x() - p_window->getSize().x / 2 < 0)
		view_x = p_window->getSize().x / 2;
	if (p_player_spark->get_y() - p_window->getSize().y / 2 < 0)
		view_y = p_window->getSize().y / 2;
	if (p_window->getSize().x / 2 + p_player_spark->get_x() >= p_map->get_width()* p_map->get_tilewidth())
		view_x = p_map->get_width() * p_map->get_tilewidth() - p_window->getSize().x / 2;
	if (p_window->getSize().y / 2 + p_player_spark->get_y() >= p_map->get_height()* p_map->get_tileheight())
		view_y = p_map->get_height()* p_map->get_tileheight() - p_window->getSize().y / 2;
	view.setCenter(view_x, view_y);
}
bool drawer::init(RenderWindow& WINDOW, map_class& map_draw)
{
	p_window = &WINDOW;
	view.reset(sf::FloatRect(0, 0, p_window->getSize().x, p_window->getSize().y));
	//if (!shader.loadFromFile("shaders/blur.frag", sf::Shader::Type::Fragment))
	//	return false;
	//shader.setUniform("offsetFactor", {1.0, 1.0});
	if (!fone_drawer_part.init(p_window, &map_draw))
		return false;
	if (!population_drawer_part.spark_drawer.init(p_window, "entities/spark.tsv"))
		return false;
	if (!population_drawer_part.waterfear_drawer.init(p_window, "entities/waterfear.tsv"))
		return false;
	if (!game_info_drawer_part.init(p_window))
		return false;
	return true;
}
bool drawer::show(map_class& map_draw, population& population_draw, level_mode_enum mode)
{
	if (mode == play)
	{
		view.reset(sf::FloatRect(0, 0, p_window->getSize().x, p_window->getSize().y));
		set_view(&map_draw, &population_draw.spark);
		p_window->setView(view);
		if (!fone_drawer_part.show(view.getCenter().x, view.getCenter().y))
			return false;
		for (auto it_draw_waterfear = population_draw.map_waterfears.begin(); it_draw_waterfear != population_draw.map_waterfears.end(); it_draw_waterfear++)
			if (!population_drawer_part.waterfear_drawer.show(&it_draw_waterfear->second))
				return false;
		if (!population_drawer_part.spark_drawer.show(&population_draw.spark))
			return false;
		if (!game_info_drawer_part.show(&population_draw.spark, view.getCenter().x - p_window->getSize().x / 2, view.getCenter().y - p_window->getSize().y / 2))
			return false;
	}
	if (mode == pause)
	{
		view.reset(sf::FloatRect(0, 0, p_window->getSize().x, p_window->getSize().y));
		set_view(&map_draw, &population_draw.spark);
		p_window->setView(view);
		if (!fone_drawer_part.show(view.getCenter().x, view.getCenter().y))
			return false;
		for (auto it_draw_waterfear = population_draw.map_waterfears.begin(); it_draw_waterfear != population_draw.map_waterfears.end(); it_draw_waterfear++)
			if (!population_drawer_part.waterfear_drawer.show(&it_draw_waterfear->second))
				return false;
		if (!population_drawer_part.spark_drawer.show(&population_draw.spark))
			return false;
	}

	return true;
}