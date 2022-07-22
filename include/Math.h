#pragma once

#include <nds.h>
#include <nds/arm9/math.h>

#ifdef __cplusplus

struct Vec3
{
    int32 x, y, z;

	Vec3() = default;
	Vec3(const Vec3&) = default;
	Vec3(int32 x, int32 y, int32 z) { this->x = x; this->y = y; this->z = z; }

	static int32 dist(const Vec3& A, const Vec3& B)
	{
		Vec3 diff = (Vec3)B - (Vec3)A;
		return sqrtf32( mulf32(diff.x, diff.x) + mulf32(diff.y, diff.y) + mulf32(diff.z, diff.z));
	}

	Vec3 operator*(int operand)
	{
		Vec3 result;
		result.x = mulf32(x, operand);
		result.y = mulf32(y, operand);
		result.z = mulf32(z, operand);
		return result;
	}
	Vec3 operator+(const Vec3& other)
	{
		Vec3 result;
		result.x = x + other.x;
		result.y = y + other.y;
		result.z = z + other.z;
		return result;
	}
	const Vec3& operator+=(const Vec3& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}
	Vec3 operator-(const Vec3& other)
	{
		Vec3 result;
		result.x = x - other.x;
		result.y = y - other.y;
		result.z = z - other.z;
		return result;
	}
	const Vec3& operator-=(const Vec3& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}
	Vec3 operator-()
	{
		Vec3 result;
		result.x = -x;
		result.y = -y;
		result.z = -z;
		return result;
	}
};

struct Vec2
{
    int32 x, y;

	Vec2() = default;
	Vec2(const Vec2&) = default;
	Vec2(int32 x, int32 y) { this->x = x; this->y = y; }
};

struct Mat3
{
	int32 values[9];

	Vec3 operator*(const Vec3& other)
	{
		Vec3 result;
		result.x = mulf32(values[0], other.x) + mulf32(values[1], other.y) + mulf32(values[2], other.z);
		result.y = mulf32(values[3], other.x) + mulf32(values[4], other.y) + mulf32(values[5], other.z);
		result.z = mulf32(values[6], other.x) + mulf32(values[7], other.y) + mulf32(values[8], other.z);
		return result;
	}
};

#endif /* __cplusplus */