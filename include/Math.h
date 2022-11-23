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

struct Vec4
{
	int32 x, y, z, w;

	Vec4() = default;
};

struct Mat3
{
	int32 values[9];

	void Identity()
	{
		memset(values, 0, sizeof(int32) * 9);
		values[0] = inttof32(1);
		values[4] = inttof32(1);
		values[8] = inttof32(1);
	}

	Vec3 operator*(const Vec3& other)
	{
		Vec3 result;
		result.x = mulf32(values[0], other.x) + mulf32(values[1], other.y) + mulf32(values[2], other.z);
		result.y = mulf32(values[3], other.x) + mulf32(values[4], other.y) + mulf32(values[5], other.z);
		result.z = mulf32(values[6], other.x) + mulf32(values[7], other.y) + mulf32(values[8], other.z);
		return result;
	}
};

struct Mat4x4
{
	int32 values[16];

	Mat4x4()
	{
		Identity();
	}

	void Identity()
	{
		memset(values, 0, 16 * sizeof(int32));
		values[0]  = inttof32(1);
		values[5]  = inttof32(1);
		values[10] = inttof32(1);
		values[15] = inttof32(1);
	}

	Mat4x4 operator*(const Mat4x4& other)
	{
		Mat4x4 result;

		for(uint32_t y = 0; y < 4; y++)
		{
			for(uint32_t x = 0; x < 4; x++)
			{
				result.values[y * 4 + x] = mulf32(values[y * 4 + 0], other.values[0 * 4 + x]) + mulf32(values[y * 4 + 1], other.values[1 * 4 + x]) + mulf32(values[y * 4 + 2], other.values[2 * 4 + x]) + mulf32(values[y * 4 + 3], other.values[3 * 4 + x]);
			}
		}
		return result;
	}
	
	Mat4x4 operator*(const int32& value)
	{
		Mat4x4 result;

		for(uint32_t y = 0; y < 4; y++)
		{
			for(uint32_t x = 0; x < 4; x++)
			{
				result.values[y * 4 + x] = mulf32(values[y * 4 + x], value);
			}
		}
		return result;
	}

	Vec4 operator*(const Vec4& other)
	{
		Vec4 result;

		result.x = mulf32(values[0], other.x) + mulf32(values[1], other.y) + mulf32(values[2], other.z) + mulf32(values[3], other.w);
		result.y = mulf32(values[4], other.x) + mulf32(values[5], other.y) + mulf32(values[6], other.z) + mulf32(values[7], other.w);
		result.z = mulf32(values[8], other.x) + mulf32(values[9], other.y) + mulf32(values[10], other.z) + mulf32(values[11], other.w);
		result.w = mulf32(values[12], other.x) + mulf32(values[13], other.y) + mulf32(values[14], other.z) + mulf32(values[15], other.w);

		return result;
	}
};

#endif /* __cplusplus */