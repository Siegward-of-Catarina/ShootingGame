#pragma once

namespace sdl_engine
{
   class EngineContext;
   struct FrameData;
   class GameTimer;
   class Scene;
   struct SceneDependencies;
   class EventListener;
   class SceneManager;
   class ResourceManager;
   class InputManager;
   class SystemManager;

   class Vector2_f32;
   class Vector2_i32;
   class Renderer;
   class SoundMixer;

   class SpriteLoader;
   class SpriteResourceDeleter;
   class SpriteAnimLoader;
   class ColorRGBA;

   struct SpriteResource;
   struct SpriteAnimResource;
   struct FontResource;
   struct SoundResource;

   class SystemInterface;
   struct LogicUpdateableTag;
   struct UpdateableTag;
   struct RenderableTag;
   struct Text;
}    // namespace sdl_engine

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Mixer;

// Type aliases
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
