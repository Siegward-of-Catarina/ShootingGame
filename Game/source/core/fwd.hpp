#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
namespace myge
{
   // クラスの前方宣言
   class Game;
   class Scene;
   class Sequencer;
   class ResourceManager;
   class Vec4;
   class Renderer;

   class SpriteLoader;
   class SpriteAnimLoader;
   struct SpriteAnimResource;

   using Color = myge::Vec4;

}    // namespace myge
// SDLの前方宣言
struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

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

using SDL_WindowPtr = std::unique_ptr<SDL_Window, decltype( &SDL_DestroyWindow )>;