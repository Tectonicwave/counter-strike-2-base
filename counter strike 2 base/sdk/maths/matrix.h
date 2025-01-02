#pragma once

#include "vector4d.h"

namespace sdk
{
	struct matrix3x4_t
	{
		float mat[3][4];

		__forceinline float* operator[](const int i) { return mat[i]; }

		__forceinline const float* operator[](const int i) const { return mat[i]; }

		float arrData[3][4] = {};
	};

	struct matrix3x4a_t : public matrix3x4_t {};

	struct matrix4x4_t
	{
		float mat[4][4];

		__forceinline float* operator[](const int i) { return mat[i]; }

		__forceinline const float* operator[](const int i) const { return mat[i]; }
	};

	struct ViewMatrix_t
	{
		ViewMatrix_t() = default;

		constexpr ViewMatrix_t(
			const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23,
			const float m30, const float m31, const float m32, const float m33)
		{
			arrData[0][0] = m00;
			arrData[0][1] = m01;
			arrData[0][2] = m02;
			arrData[0][3] = m03;
			arrData[1][0] = m10;
			arrData[1][1] = m11;
			arrData[1][2] = m12;
			arrData[1][3] = m13;
			arrData[2][0] = m20;
			arrData[2][1] = m21;
			arrData[2][2] = m22;
			arrData[2][3] = m23;
			arrData[3][0] = m30;
			arrData[3][1] = m31;
			arrData[3][2] = m32;
			arrData[3][3] = m33;
		}

		constexpr ViewMatrix_t(const matrix3x4_t& matFrom, const sdk::vector4d& vecAdditionalRow = {})
		{
			arrData[0][0] = matFrom.arrData[0][0];
			arrData[0][1] = matFrom.arrData[0][1];
			arrData[0][2] = matFrom.arrData[0][2];
			arrData[0][3] = matFrom.arrData[0][3];
			arrData[1][0] = matFrom.arrData[1][0];
			arrData[1][1] = matFrom.arrData[1][1];
			arrData[1][2] = matFrom.arrData[1][2];
			arrData[1][3] = matFrom.arrData[1][3];
			arrData[2][0] = matFrom.arrData[2][0];
			arrData[2][1] = matFrom.arrData[2][1];
			arrData[2][2] = matFrom.arrData[2][2];
			arrData[2][3] = matFrom.arrData[2][3];
			arrData[3][0] = vecAdditionalRow.x;
			arrData[3][1] = vecAdditionalRow.y;
			arrData[3][2] = vecAdditionalRow.z;
			arrData[3][3] = vecAdditionalRow.w;
		}

		[[nodiscard]] float* operator[](const int nIndex)
		{
			return arrData[nIndex];
		}

		[[nodiscard]] const float* operator[](const int nIndex) const
		{
			return arrData[nIndex];
		}

		[[nodiscard]] const bool operator==(const ViewMatrix_t& viewOther) const
		{
			return (
				arrData[0][0] == viewOther.arrData[0][0] && arrData[0][1] == viewOther.arrData[0][1] && arrData[0][2] == viewOther.arrData[0][2] && arrData[0][3] == viewOther.arrData[0][3] &&
				arrData[1][0] == viewOther.arrData[1][0] && arrData[1][1] == viewOther.arrData[1][1] && arrData[1][2] == viewOther.arrData[1][2] && arrData[1][3] == viewOther.arrData[1][3] &&
				arrData[2][0] == viewOther.arrData[2][0] && arrData[2][1] == viewOther.arrData[2][1] && arrData[2][2] == viewOther.arrData[2][2] && arrData[2][3] == viewOther.arrData[2][3] &&
				arrData[3][0] == viewOther.arrData[3][0] && arrData[3][1] == viewOther.arrData[3][1] && arrData[3][2] == viewOther.arrData[3][2] && arrData[3][3] == viewOther.arrData[3][3]);
		}

		[[nodiscard]] const matrix3x4_t& As3x4() const
		{
			return *reinterpret_cast<const matrix3x4_t*>(this);
		}

		[[nodiscard]] matrix3x4_t& As3x4()
		{
			return *reinterpret_cast<matrix3x4_t*>(this);
		}

		constexpr ViewMatrix_t& operator+=(const ViewMatrix_t& matAdd)
		{
			for (std::uint8_t c = 0U; c < 4U; c++)
			{
				for (std::uint8_t r = 0U; r < 4U; r++)
					arrData[c][r] += matAdd[c][r];
			}

			return *this;
		}

		constexpr ViewMatrix_t& operator-=(const ViewMatrix_t& matSubtract)
		{
			for (std::uint8_t c = 0U; c < 4U; c++)
			{
				for (std::uint8_t r = 0U; r < 4U; r++)
					arrData[c][r] -= matSubtract[c][r];
			}

			return *this;
		}

		constexpr void Identity()
		{
			for (std::uint8_t c = 0U; c < 4U; c++)
			{
				for (std::uint8_t r = 0U; r < 4U; r++)
					arrData[c][r] = (c == r) ? 1.0f : 0.0f;
			}
		}

		/// concatenate transformations of two matrices into one
		/// @returns: matrix with concatenated transformations
		[[nodiscard]] constexpr ViewMatrix_t ConcatTransforms(const ViewMatrix_t& matOther) const
		{
			return {
				arrData[0][0] * matOther.arrData[0][0] + arrData[0][1] * matOther.arrData[1][0] + arrData[0][2] * matOther.arrData[2][0] + arrData[0][3] * matOther.arrData[3][0],
				arrData[0][0] * matOther.arrData[0][1] + arrData[0][1] * matOther.arrData[1][1] + arrData[0][2] * matOther.arrData[2][1] + arrData[0][3] * matOther.arrData[3][1],
				arrData[0][0] * matOther.arrData[0][2] + arrData[0][1] * matOther.arrData[1][2] + arrData[0][2] * matOther.arrData[2][2] + arrData[0][3] * matOther.arrData[3][2],
				arrData[0][0] * matOther.arrData[0][3] + arrData[0][1] * matOther.arrData[1][3] + arrData[0][2] * matOther.arrData[2][3] + arrData[0][3] * matOther.arrData[3][3],

				arrData[1][0] * matOther.arrData[0][0] + arrData[1][1] * matOther.arrData[1][0] + arrData[1][2] * matOther.arrData[2][0] + arrData[1][3] * matOther.arrData[3][0],
				arrData[1][0] * matOther.arrData[0][1] + arrData[1][1] * matOther.arrData[1][1] + arrData[1][2] * matOther.arrData[2][1] + arrData[1][3] * matOther.arrData[3][1],
				arrData[1][0] * matOther.arrData[0][2] + arrData[1][1] * matOther.arrData[1][2] + arrData[1][2] * matOther.arrData[2][2] + arrData[1][3] * matOther.arrData[3][2],
				arrData[1][0] * matOther.arrData[0][3] + arrData[1][1] * matOther.arrData[1][3] + arrData[1][2] * matOther.arrData[2][3] + arrData[1][3] * matOther.arrData[3][3],

				arrData[2][0] * matOther.arrData[0][0] + arrData[2][1] * matOther.arrData[1][0] + arrData[2][2] * matOther.arrData[2][0] + arrData[2][3] * matOther.arrData[3][0],
				arrData[2][0] * matOther.arrData[0][1] + arrData[2][1] * matOther.arrData[1][1] + arrData[2][2] * matOther.arrData[2][1] + arrData[2][3] * matOther.arrData[3][1],
				arrData[2][0] * matOther.arrData[0][2] + arrData[2][1] * matOther.arrData[1][2] + arrData[2][2] * matOther.arrData[2][2] + arrData[2][3] * matOther.arrData[3][2],
				arrData[2][0] * matOther.arrData[0][3] + arrData[2][1] * matOther.arrData[1][3] + arrData[2][2] * matOther.arrData[2][3] + arrData[2][3] * matOther.arrData[3][3],

				arrData[3][0] * matOther.arrData[0][0] + arrData[3][1] * matOther.arrData[1][0] + arrData[3][2] * matOther.arrData[2][0] + arrData[3][3] * matOther.arrData[3][0],
				arrData[3][0] * matOther.arrData[0][1] + arrData[3][1] * matOther.arrData[1][1] + arrData[3][2] * matOther.arrData[2][1] + arrData[3][3] * matOther.arrData[3][1],
				arrData[3][0] * matOther.arrData[0][2] + arrData[3][1] * matOther.arrData[1][2] + arrData[3][2] * matOther.arrData[2][2] + arrData[3][3] * matOther.arrData[3][2],
				arrData[3][0] * matOther.arrData[0][3] + arrData[3][1] * matOther.arrData[1][3] + arrData[3][2] * matOther.arrData[2][3] + arrData[3][3] * matOther.arrData[3][3]
			};
		}

		float arrData[4][4] = {};
	};

} // namespace sdk