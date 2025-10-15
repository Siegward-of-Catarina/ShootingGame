#pragma once
#include <engine/core/forward_declarations.hpp>
#include <engine/components/enable_tag_components.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   class SystemInterface
   {
   public:
      SystemInterface( i32 priority_, entt::registry& registry_ );
      virtual ~SystemInterface();
      virtual void    update(const FrameData& frame_) = 0;
      i32             priority() const { return _priority; };
      entt::registry& registry() { return registry_; };
      template<typename... Components>
      auto getRenderable( entt::registry& registry_ );
      template<typename... Components>
      auto getUpdateable(entt::registry& registry_);
      template<typename... Components>
      auto getLogicUpdateable( entt::registry& registry_ );

   private:
      i32             _priority;
      entt::registry& registry_;
   };

   template<typename... Components>
   inline auto SystemInterface::getRenderable( entt::registry& registry_ )
   {
      return registry_.view<Components..., RenderableTag>();
   }
   template<typename ...Components>
   inline auto SystemInterface::getUpdateable(entt::registry& registry_)
   {
       return registry_.view<Components..., UpdateableTag>();
   }
   template<typename... Components>
   inline auto SystemInterface::getLogicUpdateable( entt::registry& registry_ )
   {
      return registry_.view<Components..., LogicUpdateableTag>();
   }
}    // namespace sdl_engine
