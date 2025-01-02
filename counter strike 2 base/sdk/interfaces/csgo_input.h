#pragma

#include "../maths/qangle.h"
#include "../maths/vector2d.h"
#include "../maths/vector.h"
#include "../../definitions.h"

#include "../../utils/memory.h"
#include "../../utils/utl/UtlVector.hpp"

namespace sdk
{
	class ccs_input_message
	{
	public:

	}; // Size: 0x005C

#define MULTIPLAYER_BACKUP 150

	class subtick_button
	{
	public:

	};

	class subtick_button_pb
	{
	public:

	};

	class subtick_buttons
	{
	public:

	};

	class CTinyMoveStepData
	{
	public:

	}; //Size: 0x0018

	class CMoveStepButtons
	{
	public:

	}; //Size: 0x0020

	// @credits: www.unknowncheats.me/forum/members/2943409.html
	class CExtendedMoveData : public CMoveStepButtons
	{
	public:

	}; //Size:0x0168

	class CCSInputMessage {
	public:

	};

	class CCSInputMoves {
	public:

	};

	class CSubtickMoves {
	public:

	};

	class ccsgo_input
	{
	public:
		char pad_0000[592]; //0x0000
		bool bBlockShot; //0x0250
		bool bInThirdPerson; //0x0251
		char pad_0252[6]; //0x0252
		qangle angThirdPersonAngles; //0x0258
		char pad_0264[20]; //0x0264
		uint64_t nKeyboardPressed; //0x0278
		uint64_t nMouseWheelheelPressed; //0x0280
		uint64_t nUnPressed; //0x0288
		uint64_t nKeyboardCopy; //0x0290
		float flForwardMove; //0x0298
		float flSideMove; //0x029C
		float flUpMove; //0x02A0
		vector2d nMousePos; //0x02A4
		int32_t SubticksCount; //0x02AC
		CTinyMoveStepData Subticks[12]; //0x02B0
		vector vecViewAngle; //0x03D0
		int32_t nTargetHandle; //0x03DC
		char pad_03E0[560]; //0x03E0
		int32_t nAttackStartHistoryIndex1; //0x0610
		int32_t nAttackStartHistoryIndex2; //0x0614
		int32_t nAttackStartHistoryIndex3; //0x0618
		char pad_061C[4]; //0x061C
		int32_t MessageSize; //0x0620
		char pad_0624[4]; //0x0624
		utils::CUtlVector<CCSInputMessage*> Message; //0x0628

		sdk::qangle get_view_angles()
		{
			using fnGetViewAngles = std::int64_t(__fastcall*)(ccsgo_input*, std::int32_t);
			static fnGetViewAngles oGetViewAngles = reinterpret_cast<fnGetViewAngles>(utils::find_pattern(FNV1A("client.dll"), "4C 8B C1 85 D2 74 08 48 8D 05 ? ? ? ? C3"));

			return *reinterpret_cast<sdk::qangle*>(oGetViewAngles(this, 0));
		}
	};
}