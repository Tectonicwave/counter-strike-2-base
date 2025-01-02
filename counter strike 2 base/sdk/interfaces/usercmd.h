#pragma once

#ifdef max
#undef max
#endif

namespace sdk
{
	template <typename T>
	struct RepeatedPtrField_t
	{
		struct Rep_t
		{
			int nAllocatedSize;
			T* tElements[(std::numeric_limits<int>::max() - 2 * sizeof(int)) / sizeof(void*)];
		};

		void* pArena;
		int nCurrentSize;
		int nTotalSize;
		Rep_t* pRep;

		T* add(T* element)
		{
			// Define the function pointer correctly
			static auto add_to_rep_addr = reinterpret_cast<T * (__fastcall*)(RepeatedPtrField_t*, T*)>(
				utils::get_absolute_address(FIND_PATTERN("client.dll", "E8 ? ? ? ? 4C 8B E0 48 8B 44 24 ? 4C 8B CF"), 0x1));

			// Use the function pointer to call the function
			return add_to_rep_addr(this, element);
		}
	};

	class CBasePB
	{
	public:
		Memory_pad(0x8) // 0x0 VTABLE
		std::uint32_t nHasBits; // 0x8
		std::uint64_t nCachedBits; // 0xC

		void SetBits(std::uint64_t nBits)
		{
			// @note: you don't need to check if the bits are already set as bitwise OR will not change the value if the bit is already set
			nCachedBits |= nBits;
		}
	};

	static_assert(sizeof(CBasePB) == 0x18);

	class CMsgQAngle : public CBasePB
	{
	public:
		sdk::qangle angValue; // 0x18
	};

	static_assert(sizeof(CMsgQAngle) == 0x28);

	class CMsgVector : public CBasePB
	{
	public:
		sdk::vector4d vecValue; // 0x18
	};

	static_assert(sizeof(CMsgVector) == 0x28);

	class CCSGOInterpolationInfoPB : public CBasePB
	{
	public:
		float flFraction; // 0x18
		int nSrcTick; // 0x1C
		int nDstTick; // 0x20
	};

	static_assert(sizeof(CCSGOInterpolationInfoPB) == 0x28);

	class CCSGOInputHistoryEntryPB : public CBasePB
	{
	public:
		CMsgQAngle* pViewAngles; // 0x18
		CMsgVector* pShootPosition; // 0x20
		CMsgVector* pTargetHeadPositionCheck; // 0x28
		CMsgVector* pTargetAbsPositionCheck; // 0x30
		CMsgQAngle* pTargetAngPositionCheck; // 0x38
		CCSGOInterpolationInfoPB* cl_interp; // 0x40
		CCSGOInterpolationInfoPB* sv_interp0; // 0x48
		CCSGOInterpolationInfoPB* sv_interp1; // 0x50
		CCSGOInterpolationInfoPB* player_interp; // 0x58
		int nRenderTickCount; // 0x60
		float flRenderTickFraction; // 0x64
		int nPlayerTickCount; // 0x68
		float flPlayerTickFraction; // 0x6C
		int nFrameNumber; // 0x70
		int nTargetEntIndex; // 0x74
	};

	static_assert(sizeof(CCSGOInputHistoryEntryPB) == 0x78);

	struct CInButtonStatePB : CBasePB
	{
		std::uint64_t nValue;
		std::uint64_t nValueChanged;
		std::uint64_t nValueScroll;
	};

	static_assert(sizeof(CInButtonStatePB) == 0x30);

	struct CSubtickMoveStep : CBasePB
	{
	public:
		std::uint64_t nButton;
		bool bPressed;
		float flWhen;
		float flAnalogForwardDelta;
		float flAnalogLeftDelta;
	};

	static_assert(sizeof(CSubtickMoveStep) == 0x30);

	class CBaseUserCmdPB : public CBasePB
	{
	public:
		RepeatedPtrField_t<CSubtickMoveStep> subtickMovesField;
		std::string* strMoveCrc;
		CInButtonStatePB* pInButtonState;
		CMsgQAngle* pViewAngles;
		std::int32_t nLegacyCommandNumber;
		std::int32_t nClientTick;
		float flForwardMove;
		float flSideMove;
		float flUpMove;
		std::int32_t nImpulse;
		std::int32_t nWeaponSelect;
		std::int32_t nRandomSeed;
		std::int32_t nMousedX;
		std::int32_t nMousedY;
		std::uint32_t nConsumedServerAngleChanges;
		std::int32_t nCmdFlags;
		std::uint32_t nPawnEntityHandle;

		int CalculateCmdCRCSize()
		{
			return utils::CallVFunc<int, 7U>(this);
		}

		int sequence_number()
		{
			return *reinterpret_cast<DWORD*>(*(__int64*)this + 0x5C00);
		}

		CSubtickMoveStep* add_sub_tick_move()
		{
			using fndef = CSubtickMoveStep * (__fastcall*)(void*);
			static fndef fnCreateNewSubtickMoveStep = reinterpret_cast<fndef>(
				utils::get_absolute_address(FIND_PATTERN("client.dll", "E8 ? ? ? ? 48 8B D0 48 8D 4F 18 E8 ? ? ? ? 48 8B D0"), 0x1));


			if (subtickMovesField.pRep && subtickMovesField.nCurrentSize < subtickMovesField.pRep->nAllocatedSize)
				return subtickMovesField.pRep->tElements[subtickMovesField.nCurrentSize++];

			CSubtickMoveStep* subtick = fnCreateNewSubtickMoveStep(nullptr);
			subtickMovesField.add(subtick);

			return subtick;
		}

	};

	static_assert(sizeof(CBaseUserCmdPB) == 0x80);

	class CCSGOUserCmdPB
	{
	public:
		std::uint32_t nHasBits;
		std::uint64_t nCachedSize;
		RepeatedPtrField_t<CCSGOInputHistoryEntryPB> inputHistoryField;
		CBaseUserCmdPB* pBaseCmd;
		bool bLeftHandDesired;
		std::int32_t nAttack3StartHistoryIndex;
		std::int32_t nAttack1StartHistoryIndex;
		std::int32_t nAttack2StartHistoryIndex;

		// @note: this function is used to check if the bits are set and set them if they are not
		void CheckAndSetBits(std::uint32_t nBits)
		{
			if (!(nHasBits & nBits))
				nHasBits |= nBits;
		}
	};
	static_assert(sizeof(CCSGOUserCmdPB) == 0x40);

	struct CInButtonState
	{
	public:
		Memory_pad(0x8) // 0x0 VTABLE
		std::uint64_t nValue; // 0x8
		std::uint64_t nValueChanged; // 0x10
		std::uint64_t nValueScroll; // 0x18
	};
	static_assert(sizeof(CInButtonState) == 0x20);

	class cuser_cmd
	{
		Memory_pad(0x8); // 0x0 VTABLE
		Memory_pad(0x10); // TODO: find out what this is, added 14.08.2024
		CCSGOUserCmdPB csgoUserCmd; // 0x18
		CInButtonState nButtons; // 0x58
		Memory_pad(0x10);
		bool bHasBeenPredicted;
		Memory_pad(0xF);

		CCSGOInputHistoryEntryPB* get_input_history_entry(int nIndex)
		{
			if (nIndex >= csgoUserCmd.inputHistoryField.pRep->nAllocatedSize || nIndex >= csgoUserCmd.inputHistoryField.nCurrentSize)
				return nullptr;

			return csgoUserCmd.inputHistoryField.pRep->tElements[nIndex];
		}
	};
}