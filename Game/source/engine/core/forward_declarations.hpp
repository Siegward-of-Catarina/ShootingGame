#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <queue>
#include <entt/entt.hpp>
namespace sdl_engine
{
	// クラスの前方宣言
	class GameContext;
	class GameTimer;
	class Scene;

	class SceneManager;
	class ResourceManager;
	class InputManager;
	class SystemManager;

	class Vector4;
	class Vector2_f32;
	class Vector2_i32;
	class Renderer;

	class SpriteLoader;
	struct SpriteResource;
	class SpriteResourceDeleter;
	class SpriteAnimLoader;
	struct SpriteAnimResource;
	struct FontResource;
	class ColorRGBA;

	class SystemInterface;
	struct LogicUpdateableTag;
	struct RenderableTag;
	struct Text;
}    // namespace sdl_engine
// SDLの前方宣言
struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

// 型エイリアス
using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;

using i64 = std::int64_t;
using i32 = std::int32_t;
using i16 = std::int16_t;
using i8 = std::int8_t;
using f32 = float;
using f64 = double;

using SDL_WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;