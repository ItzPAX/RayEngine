#pragma once

class Vec3D
{
public:
	// default constructor and destructor
	Vec3D() = default;

	Vec3D(float x, float y, float z)
		: X(x), Y(y), Z(z)
	{

	}

	Vec3D(const Vec3D& v)
		: X(v.X), Y(v.Y), Z(v.Z)
	{

	}

	Vec3D operator * (const float& rhs)
	{
		return Vec3D(this->X * rhs, this->Y * rhs, this->Z * rhs);
	}

	// getters
public:
	FLOAT	x() { return X; }
	FLOAT	y() { return Y; }
	FLOAT	z() { return Z; }

	//setters
public:
	VOID	x(float _x) { X = _x; }
	VOID	y(float _y) { Y = _y; }
	VOID	z(float _z) { Z = _z; }

private:
	FLOAT X, Y, Z;
};