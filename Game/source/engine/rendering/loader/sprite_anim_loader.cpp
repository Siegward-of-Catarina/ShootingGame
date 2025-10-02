
#include <engine/rendering/loader/sprite_anim_loader.hpp>
#include <engine/rendering/loader/sprite_anim_resource.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
   std::shared_ptr<SpriteAnimResource> SpriteAnimLoader::operator()( const json& data ) const
   {

      SpriteAnimResource res;

      // keynameとスプライトの列、行数を取得
      auto anim_name{ getJsonData<std::string>(data,"name") };
      u32 sprite_col{ getJsonData<u32>(data,"sprite_col") };
      u32 sprite_row{ getJsonData<u32>(data,"sprite_row") };
      // SpriteAnimResourceに情報を格納
      res.frame_width = getJsonData<u32>(data, "frame_width");
      res.frame_height = getJsonData<u32>(data, "frame_height");
      res.frame_time = getJsonData<f64>(data, "frame_time");

      auto type{ getJsonData<std::string>(data,"anim_type") };
      if (type == "Loop") { res.anim_type = AnimType::Loop; }
      else if (type == "Once") { res.anim_type = AnimType::Once; }
      else if (type == "Manual") { res.anim_type = AnimType::Manual; }
      else { throw GameException("指定されたAnimTypeが存在しません"); }

      // フレーム数を求めて格納
      res.frame_num = sprite_col * sprite_row;

      // フレーム情報を求めるて格納
      res.frames.reserve( res.frame_num );
      res.frames.resize( res.frame_num );

      for ( size_t i = 0; i < res.frame_num; i++ )
      {
         res.frames[ i ].x = static_cast<float>( res.frame_width * ( i % sprite_col ) );
         res.frames[ i ].y = static_cast<float>( res.frame_height * ( i / sprite_col ) );
         res.frames[ i ].w = static_cast<float>( res.frame_width );
         res.frames[ i ].h = static_cast<float>( res.frame_height );
      }

      return std::make_shared<SpriteAnimResource>( res );
   }
}    // namespace sdl_engine