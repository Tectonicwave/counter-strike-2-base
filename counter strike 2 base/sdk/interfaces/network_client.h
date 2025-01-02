#pragma once
#include "../utils/memory.h"

namespace sdk
{
	class CNetChannelInfo;
	class network_game_client_t
	{
	public:
		Memory_pad(0xE8); //0x0000
		CNetChannelInfo* netchanell_info; // 0x00E8
		Memory_pad(0x8); // 0x00F0
		bool should_predict; // 0x00F8
		Memory_pad(0x7B); // 0x00F9
		int SomePrediction_tick; // 0x0174
		Memory_pad(0x104); // 0x0178
		int delta_tick; // 0x027C

		void client_predict() {

			using prediction_fn = void(__fastcall*)(network_game_client_t*, unsigned int);

			static prediction_fn func = reinterpret_cast<prediction_fn>(FIND_PATTERN("engine2.dll", "40 55 41 56 48 83 EC 28 80 B9"));

			return func(this, 1);
		}

		void set_prediction(bool value)
		{
			*(bool*)(std::uintptr_t(this) + 0xDC) = value;
		}

		float get_client_interp() {
			return utils::CallVFunc<float, 61u>(this);
		}

		bool IsConnected()
		{
			return utils::CallVFunc<bool, 12U>(this);
		}

		int get_client_tick()
		{
			return utils::CallVFunc<int, 5U>(this);
		}

		int get_server_tick()
		{
			return utils::CallVFunc<int, 6U>(this);
		}
	};

	class network_client_service_t
	{
	public:
		network_game_client_t* get_network_game_client() {

			return utils::CallVFunc<network_game_client_t*, 23u>(this);
		}
	};

}
