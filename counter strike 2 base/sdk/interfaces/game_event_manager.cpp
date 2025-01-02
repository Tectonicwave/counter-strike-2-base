#include "game_event_manager.h"
#include "../utils/memory.h"
#include "../manager.h"

#define STRINGTOKEN_MURMURHASH_SEED 0x31415926

inline uint32_t QAMurmurHash2(const void* key, int len, uint32_t seed)
{
	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	uint32_t h = seed ^ len;
	const unsigned char* data = (const unsigned char*)key;

	while (len >= 4)
	{
		uint32_t k = *(uint32_t*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch (len)
	{
	case 3:
		h ^= data[2] << 16;
	case 2:
		h ^= data[1] << 8;
	case 1:
		h ^= data[0];
		h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

struct EventHash_t
{
	EventHash_t(const char* String)
	{
		mString = String;
		mHashCode = QAMurmurHash2(String, std::strlen(String), STRINGTOKEN_MURMURHASH_SEED);
	}

	std::uint64_t mHashCode;
	const char* mString;
};


sdk::CCSPlayerController* sdk::CGameEventHelper::GetPlayerController()
{
	if (!Event)
		return nullptr;

	int controller_id{};

	CBuffer buffer;
	buffer.name = "userid";

	Event->GetControllerId(controller_id, &buffer);

	if (controller_id == -1)
		return nullptr;

	return (CCSPlayerController*)manager->resource_service->game_entity_system->get_entity_by_index(controller_id + 1);
}

sdk::CCSPlayerController* sdk::CGameEventHelper::GetAttackerController()
{
	if (!Event)
		return nullptr;

	int controller_id{};

	CBuffer buffer;
	buffer.name = "attacker";

	Event->GetControllerId(controller_id, &buffer);

	if (controller_id == -1)
		return nullptr;

	return (CCSPlayerController*)manager->resource_service->game_entity_system->get_entity_by_index(controller_id + 1);
}

int sdk::CGameEventHelper::GetDamage()
{
	if (!Event)
		return -1;

	return Event->GetInt2("dmg_health", false);
}

int sdk::CGameEventHelper::GetHealth()
{
	if (!Event)
		return -1;

	CBuffer buffer;
	buffer.name = "health";

	return Event->GetInt(&buffer);
}


int sdk::CGameEvent::GetInt2(const char* Name, bool Unk)
{
	using GetEventInt_t = int(__fastcall*)(void*, const char*, bool);
	static GetEventInt_t GetEventInt = reinterpret_cast<GetEventInt_t>(FIND_PATTERN("client.dll", "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 30 48 8B 01 41 8B F0 4C 8B F1 41 B0 01 48 8D 4C 24 20 48 8B DA 48 8B 78"));


	return GetEventInt(this, Name, Unk);
}


float sdk::CGameEvent::GetFloat2(const char* keyName)
{
	EventHash_t arg(keyName);
	return utils::CallVFunc<float, 9u>(this, &arg, 0.0f);
}
