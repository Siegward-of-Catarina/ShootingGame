#pragma once
#include <SDL3/SDL.h>
#include <entt/entt.hpp>
namespace sdl_engine
{

   // クラスの前方宣言
   class EngineContext;
   class GameTimer;
   class Scene;

   class SceneManager;
   class ResourceManager;
   class InputManager;
   class SystemManager;

   class SystemInterface;
   struct LogicUpdateableTag;
   struct RenderableTag;
}    // namespace sdl_engine

// 型エイリアス
using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8  = std::uint8_t;

using i64 = std::int64_t;
using i32 = std::int32_t;
using i16 = std::int16_t;
using i8  = std::int8_t;
using f32 = float;
using f64 = double;