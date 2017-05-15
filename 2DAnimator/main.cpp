#include <SFML/Graphics.hpp>
#include "Scene.h"
#include <string>

void redrawWindow(sf::RenderWindow& window, sf::Sprite& sprite, sf::Text& framerate)
{
	window.clear(sf::Color::White);

	window.draw(sprite);
	window.draw(framerate);

	window.display();
}

int main()
{
	bool stepMode = true;

	sf::RenderWindow window(sf::VideoMode(800, 600), "2DAnimator");

	Scene scene;

	sf::Texture texture;
	texture.create(800, 600);

	sf::Sprite sprite(texture);

	sf::Font font;
	font.loadFromFile("arial.ttf");

	sf::Text framerate;
	framerate.setFont(font);
	framerate.setCharacterSize(18);
	framerate.setColor(sf::Color::Red);
	framerate.setPosition(0, 0);
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					stepMode = !stepMode;
				}
				else if (event.key.code == sf::Keyboard::Return && stepMode)
				{
					framerate.setString("");
					texture.update(scene.nextFrame());
					redrawWindow(window, sprite, framerate);
				}
				else if(event.key.code == sf::Keyboard::Num1)
				{
					scene.setInterpolationMode(None);
				}
				else if(event.key.code == sf::Keyboard::Num2)
				{
					scene.setInterpolationMode(Nearest);
				}
				else if (event.key.code == sf::Keyboard::Num3)
				{
					scene.setInterpolationMode(Linear);
				}
			}
		}

		if (!stepMode)
		{
			framerate.setString(std::to_string(1.f/clock.restart().asSeconds()));
			texture.update(scene.nextFrame());
			redrawWindow(window, sprite, framerate);
		}
	}

	return 0;
}

