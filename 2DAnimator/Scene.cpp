#include "Scene.h"
#include <fstream>
#include <thread>
#include <stack>

void loadTransforms(Transform& transform, std::vector<std::tuple<std::string, float, float>>& transforms, int i);

void Scene::loadFromFiles()
{
	std::string line;
	std::ifstream fileStream("..\\scene.txt");
	if(fileStream.is_open())
	{
		while (getline(fileStream, line))
		{
			if (line.find("background:") != std::string::npos)
			{
				size_t pathPos = line.find("path=\"") + 6;
				size_t pathEndPos = line.find_last_of("\"");
				background.loadFromFile(line.substr(pathPos, pathEndPos - pathPos));
			}
			else if(line.find("polygon:") != std::string::npos)
			{
				size_t namePos = line.find("name=\"") + 6;
				size_t nameEndPos = line.find_last_of("\"");
				std::string name = line.substr(namePos, nameEndPos - namePos);

				size_t pointsPos = line.find("points={") + 8;
				size_t pointsEndPos = line.find_last_of("}");
				std::string points = line.substr(pointsPos, pointsEndPos - pointsPos);

				std::vector<Point> pointsList(0);

				size_t pointPos = points.find("{") + 1;
				size_t separatorPos = points.find(",");
				size_t pointEndPos = points.find("}") - 1;

				while(true)
				{
					std::string x_s = points.substr(pointPos, separatorPos - pointPos);
					std::string y_s = points.substr(separatorPos + 2, pointEndPos - separatorPos - 1);
					float x = std::stof(x_s);
					float y = std::stof(y_s);

					pointsList.push_back({ x, y });

					if (pointEndPos == points.size() - 2) break;

					pointPos = points.find("{", pointEndPos + 2) + 1;
					separatorPos = points.find(",", pointPos);
					pointEndPos = points.find("}", pointPos) - 1;
				}

				shapes.push_back(new Polygon(name, pointsList));
			}
			else if (line.find("rectangle: ") != std::string::npos)
			{
				size_t namePos = line.find("name=\"") + 6;
				size_t nameEndPos = line.find_last_of("\"");
				std::string name = line.substr(namePos, nameEndPos - namePos);

				size_t xPos = line.find("x=") + 2;
				size_t xEndPos = line.find(" ", xPos);
				float x = std::stof(line.substr(xPos, xEndPos - xPos));

				size_t yPos = line.find("y=") + 2;
				size_t yEndPos = line.find(" ", yPos);
				float y = std::stof(line.substr(yPos, yEndPos - yPos));

				size_t widthPos = line.find("width=") + 6;
				size_t widthEndPos = line.find(" ", widthPos);
				float width = std::stof(line.substr(widthPos, widthEndPos - widthPos));

				size_t heightPos = line.find("height=") + 7;
				size_t heightEndPos = line.find(" ", heightPos);
				float height = std::stof(line.substr(heightPos, heightEndPos - heightPos));

				shapes.push_back(new Rectangle(name, x, y, width, height));
			}
			else if (line.find("ellipse:") != std::string::npos)
			{
				size_t namePos = line.find("name=\"") + 6;
				size_t nameEndPos = line.find_last_of("\"");
				std::string name = line.substr(namePos, nameEndPos - namePos);

				size_t xPos = line.find("x=") + 2;
				size_t xEndPos = line.find(" ", xPos);
				float x = std::stof(line.substr(xPos, xEndPos - xPos));

				size_t yPos = line.find("y=") + 2;
				size_t yEndPos = line.find(" ", yPos);
				float y = std::stof(line.substr(yPos, yEndPos - yPos));

				size_t xRadiusPos = line.find("xRadius=") + 8;
				size_t xRadiusEndPos = line.find(" ", xRadiusPos);
				float xRadius = std::stof(line.substr(xRadiusPos, xRadiusEndPos - xRadiusPos));

				size_t yRadiusPos = line.find("yRadius=") + 8;
				size_t yRadiusEndPos = line.find(" ", yRadiusPos);
				float yRadius = std::stof(line.substr(yRadiusPos, yRadiusEndPos - yRadiusPos));

				shapes.push_back(new Ellipse(name, x, y, xRadius, yRadius));
			}
		}
		fileStream.close();
	}

	fileStream.open("..\\transforms.txt");
	if (fileStream.is_open())
	{
		while (getline(fileStream, line))
		{
			std::tuple<std::string, float, float> transform;
			if(line.find("rotate:") != std::string::npos)
			{
				size_t anglePos = line.find("angle=") + 6;
				size_t angleEndPos = line.find(" ", anglePos);
				float angle = std::stof(line.substr(anglePos, angleEndPos - anglePos));

				size_t targetPos = line.find("target=\"") + 8;
				size_t targetEndPos = line.find_last_of("\"");
				std::string target = line.substr(targetPos, targetEndPos - targetPos);

				transform = std::tuple<std::string, float, float>{ "rotate", angle, 0.0f };

				if (target == "background")
				{
					bg_transforms.push_back(transform);
				}
				else if(target == "shapes")
				{
					shapes_transforms.push_back(transform);
				}
				else
				{
					for (size_t i = 0; i < shapes.size(); ++i)
					{
						if(shapes[i]->getName() == target)
						{
							shapes[i]->addTransform(transform);
							break;
						}
					}
				}
			}
			else if(line.find("translate:") != std::string::npos)
			{
				size_t xPos = line.find("x=") + 2;
				size_t xEndPos = line.find(" ", xPos);
				float x = std::stof(line.substr(xPos, xEndPos - xPos));

				size_t yPos = line.find("y=") + 2;
				size_t yEndPos = line.find(" ", yPos);
				float y = std::stof(line.substr(yPos, yEndPos - yPos));

				size_t targetPos = line.find("target=\"") + 8;
				size_t targetEndPos = line.find_last_of("\"");
				std::string target = line.substr(targetPos, targetEndPos - targetPos);

				transform = std::tuple<std::string, float, float>{ "translate", x, y };

				if (target == "background")
				{
					bg_transforms.push_back(transform);
				}
				else if (target == "shapes")
				{
					shapes_transforms.push_back(transform);
				}
				else
				{
					for (size_t i = 0; i < shapes.size(); ++i)
					{
						if (shapes[i]->getName() == target)
						{
							shapes[i]->addTransform(transform);
							break;
						}
					}
				}
			}
			else if(line.find("scale:") != std::string::npos)
			{
				size_t xPos = line.find("x=") + 2;
				size_t xEndPos = line.find(" ", xPos);
				float x = std::stof(line.substr(xPos, xEndPos - xPos));

				size_t yPos = line.find("y=") + 2;
				size_t yEndPos = line.find(" ", yPos);
				float y = std::stof(line.substr(yPos, yEndPos - yPos));

				size_t targetPos = line.find("target=\"") + 8;
				size_t targetEndPos = line.find_last_of("\"");
				std::string target = line.substr(targetPos, targetEndPos - targetPos);

				transform = std::tuple<std::string, float, float>{ "scale", x, y };

				if (target == "background")
				{
					bg_transforms.push_back(transform);
				}
				else if (target == "shapes")
				{
					shapes_transforms.push_back(transform);
				}
				else
				{
					for (size_t i = 0; i < shapes.size(); ++i)
					{
						if (shapes[i]->getName() == target)
						{
							shapes[i]->addTransform(transform);
							break;
						}
					}
				}
			}
		}
		fileStream.close();
	}
}

Scene::Scene()
{
	frame.create(800, 600);

	loadFromFiles();
}

sf::Color Scene::getColor(Point& p)
{
	if(interpol_mode == Nearest)
	{
		int x = static_cast<int>(p.x + 0.5f);
		int y = static_cast<int>(p.y + 0.5f);
		if (x >= 0 && y >= 0 && x < background.getSize().x && y < background.getSize().y)
			return background.getPixel(x, y);
		return sf::Color::White;
	}
	if (interpol_mode == Linear)
	{
		int x = static_cast<int>(p.x);
		int y = static_cast<int>(p.y);

		if (x >= 0 && y >= 0 && x < background.getSize().x && y < background.getSize().y)
		{
			bool xBound = x + 1 < background.getSize().x;
			bool yBound = y + 1 < background.getSize().y;

			sf::Color A = background.getPixel(x, y);
			sf::Color B;
			if (yBound)
				B = background.getPixel(x, y + 1);
			else
				B = A;
			sf::Color C; 
			if (xBound)
				C = background.getPixel(x + 1, y);
			else
				C = A;
			sf::Color D;
			if (xBound && yBound)
				D = background.getPixel(x + 1, y + 1);
			else
				D = A;

			float alpha = p.y - y;
			float beta = p.x - x;
			float dalpha = 1 - alpha;
			float dbeta = 1 - beta;

			sf::Color AB{ static_cast<sf::Uint8>(dalpha * A.r + alpha * B.r), 
						  static_cast<sf::Uint8>(dalpha * A.g + alpha * B.g), 
						  static_cast<sf::Uint8>(dalpha * A.b + alpha * B.b) };
			sf::Color CD{ static_cast<sf::Uint8>(dalpha * C.r + alpha * D.r),
						  static_cast<sf::Uint8>(dalpha * C.g + alpha * D.g), 
						  static_cast<sf::Uint8>(dalpha * C.b + alpha * D.b) };

			sf::Color ABCD{ static_cast<sf::Uint8>(dbeta * AB.r + beta * CD.r),
							static_cast<sf::Uint8>(dbeta * AB.g + beta * CD.g), 
							static_cast<sf::Uint8>(dbeta * AB.b + beta * CD.b) };

			return ABCD;
		}
			
		return sf::Color::White;
	}

	int x = static_cast<int>(p.x);
	int y = static_cast<int>(p.y);
	if (x >= 0 && y >= 0 && x < background.getSize().x && y < background.getSize().y)
		return background.getPixel(x, y);
	return sf::Color::White;
}

sf::Image& Scene::nextFrame()
{
	++frame_nmbr;

	Transform bg_transform;

	loadTransforms(bg_transform, bg_transforms, frame_nmbr);
	
	bg_transform.invert();

	std::thread t1(&Scene::drawBackground_p1, this, bg_transform);
	std::thread t2(&Scene::drawBackground_p2, this, bg_transform);

	t1.join();
	t2.join();

	Transform shapes_transform;

	loadTransforms(shapes_transform, shapes_transforms, frame_nmbr);

	for (Shape* shape : shapes)
	{
		drawShape(shape, shapes_transform);
	}

	return frame;
}

void Scene::drawShape(Shape* shape, Transform& transform)
{
	Polygon* poly = static_cast<Polygon*>(shape);
	std::vector<Point> polyPoints = poly->getTransformedPoints(transform);

	for (size_t i = 0; i < polyPoints.size(); ++i)
	{
		if (i == polyPoints.size() - 1)
			drawLine(&polyPoints[i], &polyPoints[0]);
		else
			drawLine(&polyPoints[i], &polyPoints[i + 1]);
	}
}

void Scene::drawLine(Point* p1, Point* p2)
{
	if(p1->x > p2->x)
	{
		Point* t = p1;
		p1 = p2;
		p2 = t;
	}

	int dx = static_cast<int>(p2->x - p1->x);
	int dy = static_cast<int>(p2->y - p1->y);

	if(dy >= 0)
	{
		if(dx >= dy)
		{
			int y = static_cast<int>(p1->y);
			int eps = 0;

			for (int x = static_cast<int>(p1->x); x <= p2->x; x++)
			{
				if (x >= 0 && y >= 0 && x < frame.getSize().x && y < frame.getSize().y)
					frame.setPixel(x, y, sf::Color::Black);
				eps += dy;
				if ((eps << 1) >= dx)
				{
					y++;
					eps -= dx;
				}
			}
		}
		else
		{
			int x = static_cast<int>(p1->x);
			int eps = 0;

			for (int y = static_cast<int>(p1->y); y <= p2->y; y++)
			{
				if (x >= 0 && y >= 0 && x < frame.getSize().x && y < frame.getSize().y)
					frame.setPixel(x, y, sf::Color::Black);
				eps += dx;
				if ((eps << 1) >= dy)
				{
					x++;
					eps -= dy;
				}
			}
		}
	}
	else
	{
		dy = -dy;

		if (dx >= dy)
		{
			int y = static_cast<int>(p1->y);
			int eps = 0;

			for (int x = static_cast<int>(p1->x); x <= p2->x; x++)
			{
				if (x >= 0 && y >= 0 && x < frame.getSize().x && y < frame.getSize().y)
					frame.setPixel(x, y, sf::Color::Black);
				eps += dy;
				if ((eps << 1) >= dx)
				{
					y--;
					eps -= dx;
				}
			}
		}
		else
		{
			int x = static_cast<int>(p1->x);
			int eps = 0;

			for (int y = static_cast<int>(p1->y); y >= p2->y; y--)
			{
				if (x >= 0 && y >= 0 && x < frame.getSize().x && y < frame.getSize().y)
					frame.setPixel(x, y, sf::Color::Black);
				eps += dx;
				if ((eps << 1) >= dy)
				{
					x++;
					eps -= dy;
				}
			}
		}
	}
}

void Scene::drawBackground_p1(Transform bg_transform)
{
	std::pair<int, int> globalOrigin = { 400, 300 };

	for (size_t j = 0; j < 300; ++j)
	{
		for (size_t i = 0; i < frame.getSize().x; ++i)
		{
			Point p(static_cast<float>(i), static_cast<float>(j));
			p.x -= globalOrigin.first;
			p.y -= globalOrigin.second;
			p.applyTransform(bg_transform);
			p.x += globalOrigin.first;
			p.y += globalOrigin.second;
			frame.setPixel(i, j, getColor(p));
		}
	}
}

void Scene::drawBackground_p2(Transform bg_transform)
{
	std::pair<int, int> globalOrigin = { 400, 300 };

	for (size_t j = 300; j < 600; ++j)
	{
		for (size_t i = 0; i < frame.getSize().x; ++i)
		{
			Point p(static_cast<float>(i), static_cast<float>(j));
			p.x -= globalOrigin.first;
			p.y -= globalOrigin.second;
			p.applyTransform(bg_transform);
			p.x += globalOrigin.first;
			p.y += globalOrigin.second;
			frame.setPixel(i, j, getColor(p));
		}
	}
}

void loadTransforms(Transform& transform, std::vector<std::tuple<std::string, float, float>>& transforms, int n)
{
	for (size_t i = 0; i < transforms.size(); ++i)
	{
		if (std::get<0>(transforms[i]) == "rotate")
		{
			float angle;

			if (std::get<1>(transforms[i]) < 0)
			{
				angle = -0.0175f * n;
				if (angle < std::get<1>(transforms[i]))
					angle = std::get<1>(transforms[i]);
			}
			else
			{
				angle = 0.0175f * n;
				if (angle > std::get<1>(transforms[i]))
					angle = std::get<1>(transforms[i]);
			}

			transform.addRotation(angle);
		}
		else if (std::get<0>(transforms[i]) == "translate")
		{
			float x;
			float y;

			if (std::get<1>(transforms[i]) < 0)
			{
				x = -n;
				if (x < std::get<1>(transforms[i]))
					x = std::get<1>(transforms[i]);
			}
			else
			{
				x = n;
				if (x > std::get<1>(transforms[i]))
					x = std::get<1>(transforms[i]);
			}

			if (std::get<2>(transforms[i]) < 0)
			{
				y = -n;
				if (y < std::get<2>(transforms[i]))
					y = std::get<2>(transforms[i]);
			}
			else
			{
				y = n;
				if (y > std::get<2>(transforms[i]))
					y = std::get<2>(transforms[i]);
			}

			transform.addTranslation(x, y);
		}
		else if (std::get<0>(transforms[i]) == "scale")
		{
			float x;
			float y;

			if (std::get<1>(transforms[i]) < 1)
			{
				x = 1 - 0.01f * n;
				if (x < std::get<1>(transforms[i]))
					x = std::get<1>(transforms[i]);
			}
			else
			{
				x = 1 + 0.01f * n;
				if (x > std::get<1>(transforms[i]))
					x = std::get<1>(transforms[i]);
			}

			if (std::get<2>(transforms[i]) < 1)
			{
				y = 1 - 0.01f * n;
				if (y < std::get<2>(transforms[i]))
					y = std::get<2>(transforms[i]);
			}
			else
			{
				y = 1 + 0.01f * n;
				if (y > std::get<2>(transforms[i]))
					y = std::get<2>(transforms[i]);
			}

			transform.addScale(x, y);
		}
	}
}