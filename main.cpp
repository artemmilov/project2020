#include <iostream>
#include <SFML/Graphics.hpp>
#include "core.h"

using namespace std;

int main()
{
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(1024, 768, 64), "The dirt");// , sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	bool Esc_press = false;
	bool active = true;
	core test_level;
	test_level.start(window, "location1", "room", 78);
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		time = min(time, 0.05);
		//time = 0.01;
		clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if (!Esc_press)
			{
				if (active)
				{
					test_level.pause();
					active = false;
				}
				else
				{
					test_level.play();
					active = true;
				}
			}
			Esc_press = true;
		}
		else
			Esc_press = false;

		window.clear();
		test_level.update(time);
		window.display();
	}

	return 0;
}