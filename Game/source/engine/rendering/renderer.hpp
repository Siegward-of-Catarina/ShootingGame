#pragma once
#include <engine/core/forward_declarations.hpp>
#include <SDL3_ttf/SDL_ttf.h>
using SDL_RendererPtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
namespace sdl_engine
{
	// SDL_Rendererラップクラス
	class Renderer
	{
	public:
		Renderer();
		Renderer(SDL_Window* window_);
		~Renderer();

		SDL_Texture* loadTexture(std::string_view path);
		SDL_Texture* createWhiteTexture();

		void setRenderClearColor(float r_, float g_, float b_, float a_);
		void setRenderClearColor(Color& color_);

		void renderClear();
		void renderClear(Color color_);
		void renderClear(float r_, float g_, float b_, float a_);
		void renderDebugText(std::string_view text, f32 x_, f32 y_);
		void renderTexture(SDL_Texture* texture_, const SDL_FRect* src_, const SDL_FRect* dst_, f32 angle_);

		void renderPresent();

	private:
		SDL_RendererPtr _sdl_renderer;
	};
}    // namespace sdl_engine
