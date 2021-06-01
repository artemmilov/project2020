/*#include "game.h"

using namespace std;
using namespace sf;

bool game::start(RenderWindow& WINDOW)
{
	p_window = &WINDOW;
	if (!game_level.start_from_beginning("level3"))
		return false;
	if (!game_animator.init(*p_window, game_level))
		return false;
	return true;
}
bool game::update(float time)
{
	user_decision.clear();
	
	game_user.interact_with_user();
	user_decision = game_user.get_v_user_keyboard_commands();
	if (!game_level.update(user_decision, time))
		return false;
	if (!capsule_level_animation.pack_capsule_from_level(game_level))
		return false;
	if (!game_animator.show(capsule_level_animation))
		return false;
	return true;
}*/