#pragma once
#include <memory>
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>

#include "Vendor/json.hpp"
#include "box2d/box2d.h"

using nlohmann::json;

const std::string S_ASSETS_PATH = ASSETS_PATH;


constexpr float WORLD_TO_BOX = 1.0f/50.f;  
constexpr float BOX_TO_WORLD = 1.0f/WORLD_TO_BOX;

constexpr int BIT(int x)
{
	return 1 << x;
}

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Unique<T> CreateUnique(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}


using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

#include "raylib.h"
#include "raymath.h"
#include "extras/physac.h"
#include "extras/raygui.h"

Vector2 operator*(const Vector2& a, const f32& b)
{
	return Vector2Scale(a, b);
}

Vector2 operator+(const Vector2& a, const Vector2& b)
{
	return Vector2Add(a, b);
}

Vector2 operator-(const Vector2& a, const Vector2& b)
{
	return Vector2Subtract(a, b);
}

bool operator==(const Vector2& a, const Vector2& b)
{
	return a.x == b.x && a.y == b.y;
}
bool operator!=(const Vector2& a, const Vector2& b)
{
	return !(a==b);
}

inline Color lerp(const Color& a, const Color& b, const f32 t)
{
	return Color{
		u8(f32(b.r) * t + f32(a.r) * (1.0f - t)),
		u8(f32(b.g) * t + f32(a.g) * (1.0f - t)),
		u8(f32(b.b) * t + f32(a.b) * (1.0f - t)),
		0xff
	};
}

template <typename T>
inline T lerp(T a, T b, f32 t)
{
	return b * t + a * (1.0f - t);
}

template <typename T>
inline T lerp_dt(T a, T b, f32 s, f32 dt)
{
	return lerp(a, b, 1.0f - std::pow(s, dt));
}
template <typename T>
inline T damp(T a, T b, f32 lambda, f32 dt)
{
	return lerp(a, b, 1.0f - std::exp(-lambda * dt));
}



static f32 X(const Vector2& v) { return v.x; }
static f32 Y(const Vector2& v) { return v.y; }

static f32 X(const Vector3& v) { return v.x; }
static f32 Y(const Vector3& v) { return v.y; }
static f32 Z(const Vector3& v) { return v.z; }
static u8 X(const Color& v) { return v.r; }
static u8 Y(const Color& v) { return v.g; }
static u8 Z(const Color& v) { return v.b; }
static f32 X(const b2Vec2& v) { return v.x; }
static f32 Y(const b2Vec2& v) { return v.y; }


template <typename FROM>
Color convert(const FROM& v)
{
    return Color{ u8(255 * X(v)), u8(Y(v) * 255), u8(255 * Z(v)), 255 };
}

Vector2 convert(const b2Vec2& v)
{
	return Vector2{X(v), Y(v)};
}


b2Vec2 convert(const Vector2& v)
{
	return b2Vec2{X(v), Y(v)};
}


template <typename TO, typename FROM>
TO convert(const FROM& v)
{
    return { X(v), Y(v), Z(v) };
}



#include "rlgl.h"

struct Serializable {
    virtual void save(json& save_file) = 0;
    virtual void load(json& save_file) = 0;
    virtual ~Serializable() = default;
};

auto& xplat_getline(std::istream& stream, std::string& string)
{
#if __EMSCRIPTEN__
    auto& ret = std::getline(stream, string);
    
    if(!string.empty())
        string.pop_back();
    return ret;
#endif
    return std::getline(stream, string);
}

#include <thread>
#include <chrono>

template <typename Fn>
void delay_callback(f32 wait_time, Fn funct )
{
#if __EMSCRIPTEN__ && !__EMSCRIPTEN_PTHREADS__
		funct();
		return;
#endif
	auto t = std::thread([funct, wait_time](){
#if __EMSCRIPTEN__
		std::this_thread::sleep_for(std::chrono::seconds((int)wait_time));
#else
		std::this_thread::sleep_for(std::chrono::seconds::duration<f32>(wait_time));
#endif
		funct();
	});
	t.detach();
}
