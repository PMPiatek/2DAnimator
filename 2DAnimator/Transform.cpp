#include "Transform.h"
#include <SFML/Graphics.hpp>


void Transform::addRotation(float alpha)
{
	float sina = sin(alpha);
	float cosa = cos(alpha);

	float n_a11 = cosa * a11 + sina * a21;
	float n_a12 = cosa * a12 + sina * a22;
	float n_a13 = cosa * a13 + sina * a23;
	float n_a21 = -sina * a11 + cosa * a21;
	float n_a22 = -sina * a12 + cosa * a22;
	float n_a23 = -sina * a13 + cosa * a23;

	a11 = n_a11;
	a12 = n_a12;
	a13 = n_a13;
	a21 = n_a21;
	a22 = n_a22;
	a23 = n_a23;
}

void Transform::addTranslation(float x, float y)
{
	a13 = a13 + x;
	a23 = a23 + y;
}

void Transform::addScale(float x, float y)
{
	a11 = x*a11;
	a12 = x*a12;
	a13 = x*a13;
	a21 = y*a21;
	a22 = y*a22;
	a23 = y*a23;
}

Transform::Transform()
{
	a11 = 1.0;
	a12 = 0.0;
	a13 = 0.0;
	a21 = 0.0;
	a22 = 1.0;
	a23 = 0.0;
}

Transform Transform::Scale(float x, float y)
{
	Transform transform;

	transform.a11 = x;
	transform.a22 = y;

	return transform;
}

Transform Transform::Rotation(float alpha)
{
	float sina = sin(alpha);
	float cosa = cos(alpha);

	Transform transform;

	transform.a11 = cosa;
	transform.a12 = -sina;
	transform.a21 = sina;
	transform.a22 = cosa;

	return transform;
}

Transform Transform::Translation(float x, float y)
{
	Transform transform;

	transform.a13 = x;
	transform.a23 = y;

	return transform;
}

Transform& Transform::operator+(Transform& transform)
{
	Transform combinedTransform;

	combinedTransform.a11 = a11 * transform.a11 + a21 * transform.a12 ;
	combinedTransform.a12 = a12 * transform.a11 + a22 * transform.a12 ;
	combinedTransform.a13 = a13 * transform.a11 + a23 * transform.a12  + transform.a13;
	combinedTransform.a21 = a11 * transform.a21 + a21 * transform.a22 ;
	combinedTransform.a22 = a12 * transform.a21 + a22 * transform.a22 ;
	combinedTransform.a23 = a13 * transform.a21 + a23 * transform.a22 + transform.a23;

	return combinedTransform;
}

void Transform::invert()
{
	float detA  = a11 * a22 - a12 * a21;
	if (detA)
	{
		float detA11 = a22;
		float detA12 = -a12;
		float detA13 = a12 * a23 - a13 * a22;
		float detA21 = -a21;
		float detA22 = a11;
		float detA23 = a13 * a21 - a11 * a23;

		a11 = detA11 / detA;
		a12 = detA12 / detA;
		a13 = detA13 / detA;
		a21 = detA21 / detA;
		a22 = detA22 / detA;
		a23 = detA23 / detA;
	}
	else
	{
		a11 = 1.0f;
		a12 = 0.0f;
		a13 = 0.0f;
		a21 = 0.0f;
		a22 = 1.0f;
		a23 = 0.0f;
	}
}