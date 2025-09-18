#pragma once
#include "../../utils/memory.h"

namespace sdk
{
	class INetChannelInfo
	{
	public:

		float get_network_latency()
		{
			return utils::CallVFunc<float, 10U>(this);
		}

		float get_engine_latency()
		{
			return utils::CallVFunc<float, 11U>(this);
		}
	};

	struct cengine_client
	{
		bool in_game()
		{
			return utils::CallVFunc<bool, 38U>(this);
		}

		bool is_connected()
		{
			return utils::CallVFunc<bool, 39U>(this);
		}

		INetChannelInfo* get_net_channel_info(int nSplitScreenSlot = 0)
		{
			return utils::CallVFunc<INetChannelInfo*, 39U>(this, nSplitScreenSlot);
		}

		int get_local_player()
		{
			int nIndex = -1;

			utils::CallVFunc<void, 53U>(this, std::ref(nIndex), 0);//52

			return nIndex + 1;
		}

	};

} // namespace sdk