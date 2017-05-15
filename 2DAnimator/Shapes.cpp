#include "Shapes.h"

extern void loadTransforms(Transform& transform, std::vector<std::tuple<std::string, float, float>>& transforms, int i);

void Point::applyTransform(Transform& transform)
{
	float n_x = transform.a11 * x + transform.a12 * y + transform.a13;
	float n_y = transform.a21 * x + transform.a22 * y + transform.a23;
	x = n_x;
	y = n_y;
}


Polygon::Polygon(std::string name, std::vector<Point> pointsList)
{
	this->name = name;
	points = pointsList;
}

std::pair<int, int> Polygon::getOrigin(std::vector<Point>& points)
{
	Point off = points[0];
	float twicearea = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
	Point p1, p2;
	float f;
	for (size_t i = 0, j = points.size() - 1; i < points.size(); j = i++) {
		p1 = points[i];
		p2 = points[j];
		f = (p1.x - off.x) * (p2.y - off.y) - (p2.x - off.x) * (p1.y - off.y);
		twicearea += f;
		x += (p1.x + p2.x - 2 * off.x) * f;
		y += (p1.y + p2.y - 2 * off.y) * f;
	}

	f = twicearea * 3;

	return std::pair<int, int>(static_cast<int>(x / f + off.x), static_cast<int>(y / f + off.y));

}

std::vector<Point> Polygon::getTransformedPoints(Transform& transform)
{
	++transform_iter;

	Transform localTransform;

	loadTransforms(localTransform, shape_transforms, transform_iter);

	std::vector<Point> transformedPoints = points;

	std::pair<int, int> localOrigin = getOrigin(transformedPoints);

	for (size_t i = 0; i < transformedPoints.size(); ++i)
	{
		transformedPoints[i].x -= localOrigin.first;
		transformedPoints[i].y -= localOrigin.second;
		transformedPoints[i].applyTransform(localTransform);
		transformedPoints[i].x += localOrigin.first;
		transformedPoints[i].y += localOrigin.second;
	}

	std::pair<int, int> globalOrigin = {400, 300};

	for (size_t i = 0; i < transformedPoints.size(); ++i)
	{
		transformedPoints[i].x -= globalOrigin.first;
		transformedPoints[i].y -= globalOrigin.second;
		transformedPoints[i].applyTransform(transform);
		transformedPoints[i].x += globalOrigin.first;
		transformedPoints[i].y += globalOrigin.second;
	}

	return transformedPoints;
}

Rectangle::Rectangle(std::string name, float x, float y, float width, float height):
	Polygon(name,
			{
				{x, y}, 
				{x + width, y}, 
				{x + width, y + height}, 
				{x , y + height}
			})
{
	
}

Ellipse::Ellipse(std::string name, float x, float y, float radiusX, float radiusY): Polygon(name, {})
{
	for (float angle : {0.0f, 0.0872f, 0.1745f, 0.3490f, 0.5235f, 0.6981f, 0.8726f, 1.0471f, 1.2217f, 1.3962f, 1.4835f,
						1.5707f, 1.6580f, 1.7453f, 1.9198f, 2.0943f, 2.2689f, 2.4434f, 2.6179f, 2.7925f, 2.9670f, 3.0543f,
						3.1415f, 3.2288f, 3.3161f, 3.4906f, 3.6651f, 3.8397f, 4.0142f, 4.1887f, 4.3633f, 4.5378f, 4.6251f,
						4.7123f, 4.7996f, 4.8869f, 5.0614f, 5.2359f, 5.4105f, 5.5850f, 5.7595f, 5.9341f, 6.1086f, 6.1959f})
	{
		points.push_back({ radiusX*cos(angle) + x, radiusY*sin(angle) + y});
	}
}