#include <unordered_set>

#include <SFML/Graphics.hpp>

#include "Mandelbrot.h"
#include "Utility.h"

int main()
{
	unsigned size{ 900 };

	sf::Vector2u windowSize(size, size);

	std::unordered_set<sf::Keyboard::Key> pressedKeys;

	Mandelbrot mandelbrot;
	mandelbrot.SetImageFactor(size / mandelbrot.GetSize().x);

	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);

	sf::Sprite sprite;
	sprite.setTexture(texture);

	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Fractals");

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				pressedKeys.insert(event.key.code);
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				pressedKeys.erase(event.key.code);
			}
		}

		double deltaTime{ clock.getElapsedTime().asSeconds() };
		clock.restart();

		double velocity{ 1.0 * deltaTime };

		if (pressedKeys.contains(sf::Keyboard::W))
		{
			mandelbrot.Move({ 0, -velocity });
		}

		if (pressedKeys.contains(sf::Keyboard::A))
		{
			mandelbrot.Move({ -velocity, 0 });
		}

		if (pressedKeys.contains(sf::Keyboard::S))
		{
			mandelbrot.Move({ 0, velocity });
		}

		if (pressedKeys.contains(sf::Keyboard::D))
		{
			mandelbrot.Move({ velocity, 0 });
		}

		if (pressedKeys.contains(sf::Keyboard::R))
		{
			mandelbrot.Zoom(-velocity);
		}

		if (pressedKeys.contains(sf::Keyboard::F))
		{
			mandelbrot.Zoom(velocity);
		}

		if (pressedKeys.contains(sf::Keyboard::T))
		{
			++mandelbrot.GetMaxIterations();
			pressedKeys.erase(sf::Keyboard::T);
		}

		if (pressedKeys.contains(sf::Keyboard::G))
		{
			if (mandelbrot.GetMaxIterations() != 0)
			{
				--mandelbrot.GetMaxIterations();
			}
			pressedKeys.erase(sf::Keyboard::G);
		}

		sf::Image image(mandelbrot.GenerateImage());

		texture.loadFromImage(image);

		window.setTitle("FPS: " + Utility::ToString(1 / deltaTime) + ", iterations: " + Utility::ToString(mandelbrot.GetMaxIterations()));
		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}