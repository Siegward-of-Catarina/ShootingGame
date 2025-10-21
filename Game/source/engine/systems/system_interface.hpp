#pragma once
#include <engine/components/enable_tag_components.hpp>
#include <engine/core/forward_declarations.hpp>
#include <entt/entt.hpp>
namespace sdl_engine
{
   class SystemInterface
   {
   public:
      SystemInterface( i32 priority_, entt::registry& registry_ );
      virtual ~SystemInterface();
      virtual void    update( const FrameData& frame_ ) = 0;
      i32             priority() const { return _priority; };
      entt::registry& registry() { return registry_; };

      template<typename... Components, typename... Exclude>
      auto getRenderable( entt::registry& registry_, const entt::exclude_t<Exclude...>& exclude_ = entt::exclude_t {} );
      template<typename... Components, typename... Exclude>
      auto getUpdateable( entt::registry& registry_, const entt::exclude_t<Exclude...>& exclude_ = entt::exclude_t {} );
      template<typename... Components, typename... Exclude>
      auto getLogicUpdateable( entt::registry&                    registry_,
                               const entt::exclude_t<Exclude...>& exclude_ = entt::exclude_t {} );

   private:
      i32             _priority;
      entt::registry& registry_;
   };

   template<typename... Components, typename... Exclude>
   inline auto SystemInterface::getRenderable( entt::registry& registry_, const entt::exclude_t<Exclude...>& exclude_ )
   {
      return registry_.view<Components..., RenderableTag>();
   }
   template<typename... Components, typename... Exclude>
   inline auto SystemInterface::getUpdateable( entt::registry& registry_, const entt::exclude_t<Exclude...>& exclude_ )
   {
      return registry_.view<Components..., UpdateableTag>();
   }
   template<typename... Components, typename... Exclude>
   inline auto SystemInterface::getLogicUpdateable( entt::registry&                    registry_,
                                                    const entt::exclude_t<Exclude...>& exclude_ )
   {
      return registry_.view<Components..., LogicUpdateableTag>( exclude_ );
   }
}    // namespace sdl_engine
