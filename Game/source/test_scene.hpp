#pragma once
#include "scene.hpp"

#include <SDL3/SDL.h>
#include <vector>
namespace myge
{
   class TestScene : public Scene
   {
   public:
      TestScene( Game* game_ );
      ~TestScene();
      // Scene を介して継承されました
      void proc( float _delta ) override;

   private:
      SDL_Texture*           image;
      std::vector<SDL_FRect> sprite;
   };
}    // namespace myge