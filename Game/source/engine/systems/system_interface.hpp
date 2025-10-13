#pragma once
#include <engine/core/forward_declarations.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   class SystemInterface
   {
   public:
      SystemInterface( i32 priority_, entt::registry& registry_ );
      virtual ~SystemInterface();
      virtual void    update( sdl_engine::EngineContext& context_ ) = 0;
      i32             getPriority() const { return _priority; };
      entt::registry& getRegistry() { return registry_; };
      template<typename... Components>
      auto getLogicUpdateable( entt::registry& registry_ );
      template<typename... Components>
      auto getRenderable( entt::registry& registry_ );

   private:
      i32             _priority;
      entt::registry& registry_;
   };

   template<typename... Components>
   inline auto SystemInterface::getLogicUpdateable( entt::registry& registry_ )
   {
      return registry_.view<Components..., LogicUpdateableTag>();
   }
   template<typename... Components>
   inline auto SystemInterface::getRenderable( entt::registry& registry_ )
   {
      return registry_.view<Components..., RenderableTag>();
   }
}    // namespace sdl_engine
