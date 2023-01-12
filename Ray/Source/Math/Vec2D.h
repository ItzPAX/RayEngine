#pragma once

class Vec2D
{
public:
	// default constructor and destructor
	Vec2D() = default;

	Vec2D(float x, float y)
		: X(x), Y(y)
	{

	}

	Vec2D(const Vec2D& v)
		: X(v.X), Y(v.Y)
	{

	}


	// getters
public:
	FLOAT	x() { return X; }
	FLOAT	y() { return Y; }

	//setters
public:
	VOID	x(float _x) { X = _x; }
	VOID	y(float _y) { Y = _y; }

private:
	FLOAT X, Y;
};