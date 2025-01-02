#pragma

#include "../../utils/memory.h"

namespace sdk
{
	struct cview_setup
	{
		Memory_pad(0x490);
		float flOrthoLeft; // 0x0494
		float flOrthoTop; // 0x0498
		float flOrthoRight; // 0x049C
		float flOrthoBottom; // 0x04A0
		Memory_pad(0x38);
		float Fov; // 0x04D8
		float fov_viewmodel; // 0x04DC
		////Vector origin; // 0x04E0
		Memory_pad(0xC); // 0x04EC
		///QAngle view; // 0x04F8
		Memory_pad(0x14); // 0x0504
		float aspect_ratio; // 0x0518
		Memory_pad(0x71);
		BYTE nSomeFlags;
	};

	struct cview_render
	{
		Memory_pad(0x10)
		cview_setup setup;
	};
}