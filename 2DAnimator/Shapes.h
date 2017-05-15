#pragma once
#include <vector>
#include <tuple>
#include "Transform.h"

enum ShapeType
{
	ST_Polygon,
	ST_Rectangle,
	ST_Ellipse
};

struct Point
{
	float x;
	float y;
	float w;

	Point(float x = 0, float y = 0, float w = 1)
	{
		this->x = x;
		this->y = y;
		this->w = w;
	}

	void applyTransform(Transform& transform);
};

class Shape
{
public:
	virtual ~Shape(){}

	virtual ShapeType getType() = 0;
	std::string getName() { return name; }
	void addTransform(std::tuple<std::string, float, float> transform) { shape_transforms.push_back(transform); }

protected:
	std::vector<std::tuple<std::string, float, float>> shape_transforms;
	int transform_iter = 0;
	std::string name;
};

class Polygon : public Shape
{
public:
	Polygon(std::string name, std::vector<Point> pointsList);

	virtual ShapeType getType() override { return ST_Polygon; };

	std::pair<int, int> getOrigin(std::vector<Point>& points);
	std::vector<Point> getTransformedPoints(Transform& transform);
protected:
	std::vector<Point> points;
};

class Rectangle : public Polygon
{
public:
	Rectangle(std::string name, float x, float y, float width, float height);

	ShapeType getType() override { return ST_Rectangle; };
};

class Ellipse : public Polygon
{
public:
	Ellipse(std::string name, float x, float y, float radiusX, float radiusY);

	ShapeType getType() override { return ST_Ellipse; };
};
