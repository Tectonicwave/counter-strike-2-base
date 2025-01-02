#pragma once

namespace sdk
{
	using register_tags_func = void(*)();
	using logging_channel_id_t = int;

	enum logging_response_t
	{
		LR_CONTINUE,
		LR_DEBUGGER,
		LR_ABORT,
	};


	enum logging_severity_t
	{
		//-----------------------------------------------------------------------------
		// An informative logging message.
		//-----------------------------------------------------------------------------
		LS_MESSAGE = 0,

		//-----------------------------------------------------------------------------
		// A warning, typically non-fatal
		//-----------------------------------------------------------------------------
		LS_WARNING = 1,

		//-----------------------------------------------------------------------------
		// A message caused by an Assert**() operation.
		//-----------------------------------------------------------------------------
		LS_ASSERT = 2,

		//-----------------------------------------------------------------------------
		// An error, typically fatal/unrecoverable.
		//-----------------------------------------------------------------------------
		LS_ERROR = 3,

		//-----------------------------------------------------------------------------
		// A placeholder level, higher than any legal value.
		// Not a real severity value!
		//-----------------------------------------------------------------------------
		LS_HIGHEST_SEVERITY = 4,
	};

	// note: this is incomplete.
	// offset to client time (realtime in s1)
	struct global_vars_t
	{
		float real_time; //0x0000
		int32_t frame_count; //0x0004
		float frame_time; //0x0008
		float frame_time2; //0x000C
		int32_t max_clients; //0x0010
		Memory_pad(28);
		float interval_per_tick; // 0x0008
		float cur_time; //0x000C
		float flCurrentTime2; //0x0010
		Memory_pad(20);
		int32_t tick_count; //0x0050
		Memory_pad(292);
		uint64_t current_map; //0x0178
		uint64_t current_map_name; //0x0180
	}; // Size: 0x0200
}
