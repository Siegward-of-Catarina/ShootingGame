#include <engine/components/Velocity.hpp>
#include <engine/components/transform.hpp>
#include <engine/systems/transfrom_update_system.hpp>

namespace sdl_engine
{
   TransformUpdateSystem::TransformUpdateSystem() {}
   TransformUpdateSystem::~TransformUpdateSystem() {}
   void TransformUpdateSystem::update( entt::registry& registry_, GameContext& context_, f32 delta_time_ )
   {
      for ( auto [ entity, velo ] : registry_.view<Velocity>().each() )
      {
         auto& trfm = registry_.get<Transform>( entity );
         trfm.x += velo.dx * delta_time_;
         trfm.y += velo.dy * delta_time_;
         trfm.angle += velo.anguler * delta_time_;
         trfm.scale += velo.scale_rate * delta_time_;
      }
   }

   int TransformUpdateSystem::priority() const { return 98; }
}    // namespace sdl_engine
