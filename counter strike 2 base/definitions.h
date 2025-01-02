#pragma once

#define STRINGIFY(x) #x
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define CAST_TO_PTR(type, value) reinterpret_cast<type*>(value)

template<typename t, t v>
struct constant_holder
{
    enum class val_holder : t
    {
        val = v
    };
};

#define CONSTANT(value) ((decltype(value))constant_holder<decltype(value), value>::val_holder::val)

// Adds 'sz' bytes of padding to adjust the memory layout, ensuring subsequent members are placed at the desired memory locations.
#define Memory_pad(sz) private: char CONCAT(pad_, __LINE__)[sz]; public:

#define TICK_INTERVAL (manager->global_vars->interval_per_tick)//0.015625f
#define TIME_TO_TICKS(TIME) (static_cast<int>(0.5f + static_cast<float>(TIME) / TICK_INTERVAL))
#define TICKS_TO_TIME(TICKS) (TICK_INTERVAL * static_cast<float>(TICKS))
#define ROUND_TO_TICKS(TIME) (TICK_INTERVAL * TIME_TO_TICKS(TIME))
#define TICK_NEVER_THINK (-1)

#define M_PI (float)3.14159265358979323846f
#define M_RADPI 57.295779513082f
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI))
#define DEG2RAD(x) ((float)(x) * (float)(M_PI / 180.f))

//local crap
#define local_pawn ( manager->game_vars.Local_Pawn )
#define local_controller ( manager->game_vars.Local_controller )

#define FIND_PATTERN(m, x) ::utils::find_pattern(FNV1A(m), XOR(x))
#define FIND_EXPORT(name, exp) ::utils::find_export(FNV1A(name), FNV1A(exp))