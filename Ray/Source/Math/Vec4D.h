#pragma once

class Vec4D
{
public:
	// default constructor and destructor
	Vec4D() = default;

	Vec4D(float x, float y, float z, float w)
		: X(x), Y(y), Z(z), W(w)
	{

	}

	Vec4D(const Vec4D& v)
		: X(v.X), Y(v.Y), Z(v.Z), W(v.W)
	{

	}


	// getters
public:
	FLOAT	x() { return X; }
	FLOAT	y() { return Y; }
	FLOAT	z() { return Z; }
	FLOAT	w() { return W; }

	//setters
public:
	VOID	x(float _x) { X = _x; }
	VOID	y(float _y) { Y = _y; }
	VOID	z(float _z) { Z = _z; }
	VOID	w(float _w) { W = _w; }

private:
	FLOAT X, Y, Z, W;
};