#pragma once

class Transform
{
public:
	Transform();
	
	static Transform Translation(float x, float y);
	static Transform Scale(float x, float y);
	static Transform Rotation(float alpha);

	Transform& operator+(Transform& transform);

	void invert();

	void addRotation(float alpha);
	void addTranslation(float x, float y);
	void addScale(float x, float y);
private:
	float a11, a12, a13,
		a21, a22, a23;

	friend struct Point;
};