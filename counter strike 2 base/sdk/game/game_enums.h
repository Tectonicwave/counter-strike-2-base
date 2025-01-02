#pragma once

namespace sdk
{
	enum client_frame_stage : int
	{
		FRAME_UNDEFINED = -1,
		FRAME_START,
		FRAME_SIMULATE_START,
		FRAME_NET_UPDATE_START,
		FRAME_NET_UPDATE_PREPROCESS,
		FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE,
		FRAME_NET_UPDATE_POSTDATAUPDATE_START,
		FRAME_NET_UPDATE_POSTDATAUPDATE_END,
		FRAME_NET_UPDATE_END,
		FRAME_NET_CREATION,
		FRAME_SIMULATE_END,
	};

	enum ProtobufSlot : uint64_t
	{
		PROTOSLOT_1 = 0x1,
		PROTOSLOT_2 = 0x2,
		PROTOSLOT_3 = 0x4,
		PROTOSLOT_4 = 0x8,
		PROTOSLOT_5 = 0x10,
		PROTOSLOT_6 = 0x20,
		PROTOSLOT_7 = 0x40,
		PROTOSLOT_8 = 0x80,
		PROTOSLOT_9 = 0x100,
		PROTOSLOT_10 = 0x200,
		PROTOSLOT_11 = 0x400,
		PROTOSLOT_12 = 0x800,
		PROTOSLOT_13 = 0x1000,
		PROTOSLOT_14 = 0x2000,
		PROTOSLOT_15 = 0x4000,
		PROTOSLOT_16 = 0x8000
	};

	enum ECommandButtons : std::uint64_t
	{
		IN_ATTACK = 1 << 0,
		IN_JUMP = 1 << 1,
		IN_DUCK = 1 << 2,
		IN_FORWARD = 1 << 3,
		IN_BACK = 1 << 4,
		IN_USE = 1 << 5,
		IN_LEFT = 1 << 7,
		IN_RIGHT = 1 << 8,
		IN_MOVELEFT = 1 << 9,
		IN_MOVERIGHT = 1 << 10,
		IN_SECOND_ATTACK = 1 << 11,
		IN_RELOAD = 1 << 13,
		IN_SPRINT = 1 << 16,
		IN_JOYAUTOSPRINT = 1 << 17,
		IN_SHOWSCORES = 1ULL << 33,
		IN_ZOOM = 1ULL << 34,
		IN_LOOKATWEAPON = 1ULL << 35
	};

	// compiled protobuf messages and looked at what bits are used in them
	enum ESubtickMoveStepBits : std::uint32_t
	{
		MOVESTEP_BITS_BUTTON = 0x1U,
		MOVESTEP_BITS_PRESSED = 0x2U,
		MOVESTEP_BITS_WHEN = 0x4U,
		MOVESTEP_BITS_ANALOG_FORWARD_DELTA = 0x8U,
		MOVESTEP_BITS_ANALOG_LEFT_DELTA = 0x10U
	};

	enum EInputHistoryBits : std::uint32_t
	{
		INPUT_HISTORY_BITS_VIEWANGLES = 0x1U,
		INPUT_HISTORY_BITS_SHOOTPOSITION = 0x2U,
		INPUT_HISTORY_BITS_TARGETHEADPOSITIONCHECK = 0x4U,
		INPUT_HISTORY_BITS_TARGETABSPOSITIONCHECK = 0x8U,
		INPUT_HISTORY_BITS_TARGETANGCHECK = 0x10U,
		INPUT_HISTORY_BITS_CL_INTERP = 0x20U,
		INPUT_HISTORY_BITS_SV_INTERP0 = 0x40U,
		INPUT_HISTORY_BITS_SV_INTERP1 = 0x80U,
		INPUT_HISTORY_BITS_PLAYER_INTERP = 0x100U,
		INPUT_HISTORY_BITS_RENDERTICKCOUNT = 0x200U,
		INPUT_HISTORY_BITS_RENDERTICKFRACTION = 0x400U,
		INPUT_HISTORY_BITS_PLAYERTICKCOUNT = 0x800U,
		INPUT_HISTORY_BITS_PLAYERTICKFRACTION = 0x1000U,
		INPUT_HISTORY_BITS_FRAMENUMBER = 0x2000U,
		INPUT_HISTORY_BITS_TARGETENTINDEX = 0x4000U
	};

	enum EButtonStatePBBits : uint32_t
	{
		BUTTON_STATE_PB_BITS_BUTTONSTATE1 = 0x1U,
		BUTTON_STATE_PB_BITS_BUTTONSTATE2 = 0x2U,
		BUTTON_STATE_PB_BITS_BUTTONSTATE3 = 0x4U
	};

	enum EBaseCmdBits : std::uint32_t
	{
		BASE_BITS_MOVE_CRC = 0x1U,
		BASE_BITS_BUTTONPB = 0x2U,
		BASE_BITS_VIEWANGLES = 0x4U,
		BASE_BITS_COMMAND_NUMBER = 0x8U,
		BASE_BITS_CLIENT_TICK = 0x10U,
		BASE_BITS_FORWARDMOVE = 0x20U,
		BASE_BITS_LEFTMOVE = 0x40U,
		BASE_BITS_UPMOVE = 0x80U,
		BASE_BITS_IMPULSE = 0x100U,
		BASE_BITS_WEAPON_SELECT = 0x200U,
		BASE_BITS_RANDOM_SEED = 0x400U,
		BASE_BITS_MOUSEDX = 0x800U,
		BASE_BITS_MOUSEDY = 0x1000U,
		BASE_BITS_CONSUMED_SERVER_ANGLE = 0x2000U,
		BASE_BITS_CMD_FLAGS = 0x4000U,
		BASE_BITS_ENTITY_HANDLE = 0x8000U
	};

	enum ECSGOUserCmdBits : std::uint32_t
	{
		CSGOUSERCMD_BITS_BASECMD = 0x1U,
		CSGOUSERCMD_BITS_LEFTHAND = 0x2U,
		CSGOUSERCMD_BITS_ATTACK3START = 0x4U,
		CSGOUSERCMD_BITS_ATTACK1START = 0x8U,
		CSGOUSERCMD_BITS_ATTACK2START = 0x10U
	};

	enum BONEINDEX : int
	{
		head = 6,
		neck_0 = 5,
		spine_1 = 4,
		spine_2 = 2,
		pelvis = 0,
		arm_upper_L = 8,
		arm_lower_L = 9,
		hand_L = 10,
		arm_upper_R = 13,
		arm_lower_R = 14,
		hand_R = 15,
		leg_upper_L = 22,
		leg_lower_L = 23,
		ankle_L = 24,
		leg_upper_R = 25,
		leg_lower_R = 26,
		ankle_R = 27
	};

	enum flags : uint32_t
	{
		fl_onground = (1 << 0),
		fl_ducking = (1 << 1),
		fl_waterjump = (1 << 3),
		fl_ontrain = (1 << 4),
		fl_inrain = (1 << 5),
		fl_frozen = (1 << 6),
		fl_atcontrols = (1 << 7),
		fl_client = (1 << 8),
		fl_fakeclient = (1 << 9),
		fl_inwater = (1 << 10),
	};

	enum weapon_id : uint32_t
	{
		weapon_deagle = 1,
		weapon_elite = 2,
		weapon_fiveseven = 3,
		weapon_glock = 4,
		weapon_ak47 = 7,
		weapon_aug = 8,
		weapon_awp = 9,
		weapon_famas = 10,
		weapon_g3sg1 = 11,
		weapon_galilar = 13,
		weapon_m249 = 14,
		weapon_m4a1 = 16,
		weapon_mac10 = 17,
		weapon_p90 = 19,
		weapon_zone_repulsor = 20,
		weapon_mp5sd = 23,
		weapon_ump45 = 24,
		weapon_xm1014 = 25,
		weapon_bizon = 26,
		weapon_mag7 = 27,
		weapon_negev = 28,
		weapon_sawedoff = 29,
		weapon_tec9 = 30,
		weapon_taser = 31,
		weapon_hkp2000 = 32,
		weapon_mp7 = 33,
		weapon_mp9 = 34,
		weapon_nova = 35,
		weapon_p250 = 36,
		weapon_shield = 37,
		weapon_scar20 = 38,
		weapon_sg556 = 39,
		weapon_ssg08 = 40,
		weapon_knifegg = 41,
		weapon_knife = 42,
		weapon_flashbang = 43,
		weapon_hegrenade = 44,
		weapon_smokegrenade = 45,
		weapon_molotov = 46,
		weapon_decoy = 47,
		weapon_incgrenade = 48,
		weapon_c4 = 49,
		weapon_healthshot = 57,
		weapon_knife_t = 59,
		weapon_m4a1_silencer = 60,
		weapon_usp_silencer = 61,
		weapon_cz75a = 63,
		weapon_revolver = 64,
		weapon_tagrenade = 68,
		weapon_fists = 69,
		weapon_breachcharge = 70,
		weapon_tablet = 72,
		weapon_melee = 74,
		weapon_axe = 75,
		weapon_hammer = 76,
		weapon_spanner = 78,
		weapon_knife_ghost = 80,
		weapon_firebomb = 81,
		weapon_diversion = 82,
		weapon_fraggrenade = 83,
		weapon_snowball = 84,
		weapon_bumpmine = 85,
		weapon_bayonet = 500,
		weapon_knife_css = 503,
		weapon_knife_flip = 505,
		weapon_knife_gut = 506,
		weapon_knife_karambit = 507,
		weapon_knife_m9bayonet = 508,
		weapon_knife_tactical = 509,
		weapon_knife_falchion = 512,
		weapon_knife_survival_bowie = 514,
		weapon_knife_butterfly = 515,
		weapon_knife_push = 516,
		weapon_knife_cord = 517,
		weapon_knife_canis = 518,
		weapon_knife_ursus = 519,
		weapon_knife_gypsy_jackknife = 520,
		weapon_knife_outdoor = 521,
		weapon_knife_stiletto = 522,
		weapon_knife_widowmaker = 523,
		weapon_knife_skeleton = 525
	};

	enum MoveType_t : std::uint8_t {
		MOVETYPE_NONE = 0,
		MOVETYPE_OBSOLETE = 1,
		MOVETYPE_WALK = 2,
		MOVETYPE_FLY = 3,
		MOVETYPE_FLYGRAVITY = 4,
		MOVETYPE_VPHYSICS = 5,
		MOVETYPE_PUSH = 6,
		MOVETYPE_NOCLIP = 7,
		MOVETYPE_OBSERVER = 8,
		MOVETYPE_LADDER = 9,
		MOVETYPE_CUSTOM = 10,
		MOVETYPE_LAST = 11,
		MOVETYPE_INVALID = 11,
		MOVETYPE_MAX_BITS = 5,
	};

	enum KeyState_t
	{
		KEY_STATE_NONE = 0,
		KEY_STATE_DOWN,
		KEY_STATE_UP,
		KEY_STATE_RELEASED
	};

	enum prediction_reason
	{
		client_command_tick,
		demo_preentity,
		demo_simulation,
		postnetupdate,
		server_starved_and_added_usercmds,
		client_frame_simulate
	};
}