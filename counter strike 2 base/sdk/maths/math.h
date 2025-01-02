#pragma once

#include "matrix.h"
#include "../../definitions.h"

namespace math
{
	constexpr float XM_PI = 3.14159265358979323846f; // Define Pi constant

	__forceinline float normalize_yaw(float angle)
	{
		auto revolutions = angle / 360.f;
		if (angle > 180.f || angle < -180.f)
		{
			revolutions = round(abs(revolutions));
			if (angle < 0.f)
				angle = (angle + 360.f * revolutions);
			else
				angle = (angle - 360.f * revolutions);
			return angle;
		}
		return angle;
	}

	__forceinline void sin_cos(float rad, float* sine, float* cosine) {
		*sine = std::sinf(rad);
		*cosine = std::cosf(rad);
	}

	__forceinline float get_pi()
	{
		return 3.14159265358f;
	}

	__forceinline float rad2deg(float rad) {
		float result = rad * (180.0f / get_pi());
		return result;
	}

	__forceinline float deg2rad(float deg) {
		float result = deg * (get_pi() / 180.0f);
		return result;
	}


	__forceinline void angle_vectors(const sdk::vector& angles, sdk::vector& forward)
	{
		float	sp, sy, cp, cy;

		sin_cos(DEG2RAD(angles[1]), &sy, &cy);
		sin_cos(DEG2RAD(angles[0]), &sp, &cp);

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}

	__forceinline void vector_angles(const sdk::vector& forward, sdk::qangle& angles, sdk::vector* up = nullptr)
	{
		float pitch, yaw, roll;

		const auto len = forward.length_2d();
		if (up && len > 0.001f)
		{
			pitch = RAD2DEG(std::atan2(-forward.z, len));
			yaw = RAD2DEG(std::atan2(forward.y, forward.x));

			const auto left = (*up).cross(forward).normalized();

			const float up_z = (left.y * forward.x) - (left.x * forward.y);

			roll = RAD2DEG(std::atan2(left.z, up_z));
		}

		else
		{
			if (len > 0.f)
			{
				pitch = RAD2DEG(std::atan2(-forward.z, len));
				yaw = RAD2DEG(std::atan2(forward.y, forward.x));
				roll = 0.f;
			}

			else
			{
				pitch = forward.z > 0.f ? -90.f : 90.f;
				yaw = 0.f;
				roll = 0.f;
			}
		}

		angles = { pitch, yaw, roll };
	}

	__forceinline sdk::qangle calc_angle(const sdk::vector& src, const sdk::vector& dst)
	{
		sdk::qangle ret;
		vector_angles((dst - src).normalized(), ret);
		return ret;
	}

	inline const sdk::matrix4x4_t& world_to_screen_matrix() {
		return *reinterpret_cast<sdk::matrix4x4_t*>(FIND_PATTERN("client.dll", "48 8D 0D ?? ?? ?? ?? 48 C1 E0 06"));
	}

	//const vector& vecOrigin, ImVec2* pvecScreen
	inline bool world_to_screen(const sdk::vector& vecOrigin, sdk::vector2d& pvecScreen)
	{
		const auto& w2_s_matrix = manager->game_vars.view_matrix;
		const float flWidth = w2_s_matrix[3][0] * vecOrigin.x + w2_s_matrix[3][1] * vecOrigin.y + w2_s_matrix[3][2] * vecOrigin.z + w2_s_matrix[3][3];

		// check is point can't fit on screen, because it's behind us
		if (flWidth < 0.001f)
			return false;

		// compute the scene coordinates of a point in 3D
		const float flInverse = 1.0f / flWidth;
		pvecScreen.x = (w2_s_matrix[0][0] * vecOrigin.x + w2_s_matrix[0][1] * vecOrigin.y + w2_s_matrix[0][2] * vecOrigin.z + w2_s_matrix[0][3]) * flInverse;
		pvecScreen.y = (w2_s_matrix[1][0] * vecOrigin.x + w2_s_matrix[1][1] * vecOrigin.y + w2_s_matrix[1][2] * vecOrigin.z + w2_s_matrix[1][3]) * flInverse;

		// screen transform
		// get the screen position in pixels of given point
		const ImVec2 vecDisplaySize = ImGui::GetIO().DisplaySize;
		pvecScreen.x = (vecDisplaySize.x * 0.5f) + (pvecScreen.x * vecDisplaySize.x) * 0.5f;
		pvecScreen.y = (vecDisplaySize.y * 0.5f) - (pvecScreen.y * vecDisplaySize.y) * 0.5f;

		return true;
	}

	// Convert degrees to radians
	inline float XMConvertToRadians(float degrees) {
		return degrees * (XM_PI / 180.0f);  // Convert degrees to radians
	}

	__forceinline sdk::vector calculate_camera_position(sdk::vector anchorPos, float distance, sdk::qangle viewAngles)
	{
		float yaw = XMConvertToRadians(viewAngles.yaw);
		float pitch = XMConvertToRadians(viewAngles.pitch);

		float x = anchorPos.x + distance * cosf(yaw) * cosf(pitch);
		float y = anchorPos.y + distance * sinf(yaw) * cosf(pitch);
		float z = anchorPos.z + distance * sinf(pitch);

		return sdk::vector{ x, y, z };
	}

	__forceinline sdk::qangle normalize_angles(sdk::qangle angles)
	{
		while (angles.pitch > 89.0f)
			angles.pitch -= 180.0f;
		while (angles.pitch < -89.0f)
			angles.pitch += 180.0f;
		while (angles.yaw > 180.0f)
			angles.yaw -= 360.0f;
		while (angles.yaw < -180.0f)
			angles.yaw += 360.0f;
		angles.roll = 0.0f;
		return angles;
	}

	inline float approach(const float target, float value, const float speed)
	{
		const auto delta = target - value;
		if (delta > speed)
			value += speed;
		else if (delta < -speed)
			value -= speed;
		else
			value = target;

		return value;
	}

}