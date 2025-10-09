
#include <engine/rendering/resource/loader/sprite_anim_loader.hpp>
#include <engine/rendering/resource/sprite_anim_resource.hpp>
#include <engine/utils/json_utilities.hpp>
namespace sdl_engine
{
    SpriteAnimLoader::result_type SpriteAnimLoader::operator()( const json& data ) const
   {

      SpriteAnimResource res;

      // keynameとスプライトの列、行数を取得
      auto anim_name{ getJsonData<std::string>(data,"name") };
      u32 sprite_col{ getJsonData<u32>(data,"sprite_col").value() };
      u32 sprite_row{ getJsonData<u32>(data,"sprite_row").value() };
      // SpriteAnimResourceに情報を格納
      res.frame_width = getJsonData<u32>(data, "frame_width").value();
      res.frame_height = getJsonData<u32>(data, "frame_height").value();
      res.frame_time = getJsonData<f64>(data, "frame_time").value();

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

      for ( u32 i = 0; i < res.frame_num; i++ )
      {
         res.frames[ i ].x = static_cast<f32>( res.frame_width * ( i % sprite_col ) );
         res.frames[ i ].y = static_cast<f32>( res.frame_height * ( i / sprite_col ) );
         res.frames[ i ].w = static_cast<f32>( res.frame_width );
         res.frames[ i ].h = static_cast<f32>( res.frame_height );
      }

      return std::make_shared<SpriteAnimResource>( res );
   }
}    // namespace sdl_engine