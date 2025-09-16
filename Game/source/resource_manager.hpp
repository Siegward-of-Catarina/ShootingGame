#pragma once
#include <core/fwd.hpp>
#include <string>
#include <unordered_map>
namespace myge
{

   class ResourceManager
   {
   public:
      ResourceManager();
      ~ResourceManager();
      void loadResources( SDL_Renderer* const renderer_, std::string_view file_path_ );

      // 今回は規模的に個々のデータごとにgetterを用意する
      std::shared_ptr<SDL_Texture> getTexture( std::string_view key );

   private:
      std::unordered_map<std::string, std::shared_ptr<SDL_Texture> > _textures;
   };
}    // namespace myge