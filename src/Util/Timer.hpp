#pragma once

#include "Core.h"
#include <bitset>

struct Timer {
    explicit Timer() {
        start(std::numeric_limits<f32>::min());
    }
    
    explicit Timer(f32 t) {
        start(t);
    }
    virtual ~Timer() = default;

    auto start(const f32 t) -> void {
        time = t;
    }

    auto update(const f32 dt) -> void {
        time -= dt;
    }

    auto is_done() -> bool {
        return time != std::numeric_limits<f32>::min() && time <= 0.0f;
    }

    f32 time{0.0f};
};

struct CallbackTimer : Timer {
    static auto create(const f32 p_time, const std::function<void(void)>&& p_callback) -> void
    {
        for (auto i{0}; i < MAX_INST; i++)
        {
            if (!use_list[i])
            {
                new (&inst[i]) CallbackTimer(p_time, std::move(p_callback)); 
                use_list.set(i, true);
                return;
            }
        }

        throw std::runtime_error("CallbackTimer: no free timers");
    }

    static auto update_timers(const f32 dt) -> void
    {
        for (auto i{0}; i < MAX_INST; i++)
            if (use_list[i])
            {
                inst[i].update(dt);
                if (inst[i].is_done())
                {
                    inst[i].callback();
                    use_list.set(i, false);
                }
            }
    }

    // invalid timer constructor
    explicit CallbackTimer() = default;

private:
    static inline constexpr size_t MAX_INST{16};
    static std::array<CallbackTimer, MAX_INST> inst;
    static inline std::bitset<MAX_INST> use_list;
    std::function<void(void)> callback;


    explicit CallbackTimer(const f32 p_time, const std::function<void(void)>&& p_callback)
        : Timer(p_time), callback{std::move(p_callback)} 
    {
    }
};
