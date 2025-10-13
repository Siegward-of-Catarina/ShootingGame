#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/utils/json_utilities.hpp>
#include <entt/entt.hpp>
namespace myge
{
   struct WaveDependencies
   {

      entt::registry&              registry;
      sdl_engine::ResourceManager& resource_manager;
   };

   class Wave
   {
   public:
      Wave( WaveDependencies& dependencies_ );
      virtual ~Wave();
      virtual void start()                   = 0;
      virtual void update( f32 delta_time_ ) = 0;
      void         loadWaveData( const std::string& wave_name_ );
      bool         isWaveEnd() { return _wave_end; }

   protected:
      json&                        getWaveData() { return _wave_data; }
      void                         endWave() { _wave_end = true; }
      entt::registry&              registry() { return _registry; };
      sdl_engine::ResourceManager& resourceManager() { return _resource_manager; };

   private:
      entt::registry&              _registry;
      sdl_engine::ResourceManager& _resource_manager;

   private:
      json _wave_data;
      bool _wave_end;
   };
}    // namespace myge
