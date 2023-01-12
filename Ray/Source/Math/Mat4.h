#pragma once

class Mat4
{
public:
	Mat4()
	{
		SetIdentity();
	}

public:
	void SetIdentity()
	{
		::memset(m_Mat, 0, sizeof(m_Mat));
		m_Mat[0][0] = 1.f;
		m_Mat[1][1] = 1.f;
		m_Mat[2][2] = 1.f;
		m_Mat[3][3] = 1.f;
	}

	void SetScale(Vec3D scale)
	{
		m_Mat[0][0] = scale.x();
		m_Mat[1][1] = scale.y();
		m_Mat[2][2] = scale.z();
	}

	void SetTranslation(Vec3D translation)
	{
		m_Mat[3][0] = translation.x();
		m_Mat[3][1] = translation.y();
		m_Mat[3][2] = translation.z();
	}

	void SetRotationX(float x)
	{
		m_Mat[1][1] = cos(x);
		m_Mat[1][2] = sin(x);
		m_Mat[2][1] = -sin(x);
		m_Mat[2][2] = cos(x);
	}

	void SetRotationY(float y)
	{
		m_Mat[0][0] = cos(y);
		m_Mat[0][2] = -sin(y);
		m_Mat[2][0] = sin(y);
		m_Mat[2][2] = cos(y);
	}

	void SetRotationZ(float z)
	{
		m_Mat[0][0] = cos(z);
		m_Mat[0][1] = sin(z);
		m_Mat[1][0] = -sin(z);
		m_Mat[1][1] = cos(z);
	}

	void operator *= (const Mat4& rhs)
	{
		Mat4 out;

		for (auto i = 0; i < 4; i++)
		{
			for (auto j = 0; j < 4; j++)
			{
				out.m_Mat[i][j] =
					m_Mat[i][0] * rhs.m_Mat[0][j] +
					m_Mat[i][1] * rhs.m_Mat[1][j] +
					m_Mat[i][2] * rhs.m_Mat[2][j] +
					m_Mat[i][3] * rhs.m_Mat[3][j];
			}
		}

		*this = out;
	}

	void SetOrthoLH(float width, float height, float nearPlane, float farPlane)
	{
		m_Mat[0][0] = 2.f / width;
		m_Mat[1][1] = 2.f / height;
		m_Mat[2][2] = 1.f / (farPlane - nearPlane);
		m_Mat[3][2] = -(nearPlane / (farPlane - nearPlane));
	}

public:
	float m_Mat[4][4] = {};
};