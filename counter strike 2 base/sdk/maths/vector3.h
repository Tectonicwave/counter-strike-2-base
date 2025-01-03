#pragma once

namespace sdk
{
	class vector3
	{
	public:
		vector3() = default;

		vector3(const float x, const float y, const float z) :
			x(x), y(y), z(z) { }

		float x, y, z;
	};
}