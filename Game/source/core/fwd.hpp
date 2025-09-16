#pragma once
#include <cstdint>
#include <memory>
namespace myge
{
   // クラスの前方宣言
   class Game;
   class Scene;
   class Sequencer;
   class ResourceManager;
}    // namespace myge
// SDLの前方宣言
struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

// 型エイリアス
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8  = std::uint8_t;
using i32 = std::int32_t;
using i16 = std::int16_t;
using i8  = std::int8_t;
using f32 = float;
using f64 = double;