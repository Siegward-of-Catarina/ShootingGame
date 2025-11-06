#include <pch.hpp>
// my header
#include <engine/components/text.hpp>
// resource manager
#include <engine/managers/resource_manager.hpp>
// resource
#include <engine/rendering/resource/font_resource.hpp>
namespace sdl_engine
{

   Text createText( entt::resource<sdl_engine::FontResource>& resource_, json& data_ )
   {
      sdl_engine::Text text { resource_, "none" };
      auto             text_data { sdl_engine::getRequireData<std::string>( data_, "text" ) };
      text.text = text_data;
      return text;
   }
}    // namespace sdl_engine