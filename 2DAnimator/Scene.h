#pragma once

#include <SFML/Graphics.hpp>
#include "Shapes.h"

enum InterpolationMode
{
	None,
	Nearest,
	Linear
};

class Scene
{
public:
	Scene();
	~Scene(){}
	sf::Color getColor(Point& p);
	sf::Image& nextFrame();
	void setInterpolationMode(InterpolationMode mode) { interpol_mode = mode; }

private:
	sf::Image frame;
	sf::Image background;
	sf::Clock clock;
	std::vector<Shape*> shapes;

	std::vector<std::tuple<std::string, float, float>> bg_transforms;
	std::vector<std::tuple<std::string, float, float>> shapes_transforms;

	int frame_nmbr = 0;
	InterpolationMode interpol_mode = InterpolationMode::None;

	void loadFromFiles();
	void drawShape(Shape* shape, Transform& transform);
	void drawLine(Point* p1, Point* p2);
	void drawBackground_p1(Transform bg_transform);
	void drawBackground_p2(Transform bg_transform);
};

