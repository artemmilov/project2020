//#include "connection.h"
//
//using namespace std;
//
//bool capsule_from_level_to_animation::pack(level& pack_level)
//{
//	game_map_near_player_code.v_layers_near_player_code.clear();
//
//	float tile_width = pack_level.get_level_map().get_tilewidth();
//	float tile_height = pack_level.get_level_map().get_tileheight();
//	
//	player_data.coord_x = pack_level.get_player_spark().get_x();
//	player_data.coord_y = pack_level.get_player_spark().get_y();
//
//	game_map_near_player_code.v_layers_near_player_code.clear();
//	vector <tile_info> add_code;
//	tile_info add_tile;
//	for (int k(0); k < pack_level.get_level_map().get_v_layers().size(); k++)
//	{
//		add_code.clear();
//		for (int y(0); y < pack_level.get_level_map().get_v_layers()[k].get_height(); y++)
//			for (int x(0); x < pack_level.get_level_map().get_v_layers()[k].get_width(); x++)
//			{
//				if (sqrt((x * tile_width - player_data.coord_x) * (x * tile_width - player_data.coord_x) + (y * tile_height - player_data.coord_y) * (y * tile_height - player_data.coord_y)) <= 1000)
//				{
//					add_tile.x = x * tile_width;
//					add_tile.y = y * tile_height;
//					add_tile.code = pack_level.get_level_map().get_v_layers()[k].get_tile_with_coordinate(x, y);
//					add_code.push_back(add_tile);
//				}
//			}
//		game_map_near_player_code.v_layers_near_player_code.push_back(add_code);
//	}
//	/*
//	entity_info add_info;
//	for (int i(0); i < pack_level.get_v_level_entities().size(); i++)
//	{
//		if (sqrt((pack_level.get_v_level_entities()[i].get_x() - player_x) * (pack_level.get_v_level_entities()[i].get_x() - player_x) + (pack_level.get_v_level_entities()[i].get_y() - player_y) * (pack_level.get_v_level_entities()[i].get_y() - player_y)) <= 1000)
//		{
//				add_info.coord_x = pack_level.get_v_level_entities()[i].get_x();
//				add_info.coord_y = pack_level.get_v_level_entities()[i].get_y();
//				map_near_entities[pack_level.get_v_level_entities()[i].get_id()] = add_info;
//		}
//	}*/
//	
//	return true;
//}
//
//map_info& capsule_from_level_to_animation::get_map_near_player_code()
//{
//	return game_map_near_player_code;
//}
///*map <unsigned int, entity_info>& capsule_from_level_to_animation::get_map_near_entities()
//{
//	return map_near_entities;
//}*/
//spark_info& capsule_from_level_to_animation::get_player_data()
//{
//	return player_data;
//}
