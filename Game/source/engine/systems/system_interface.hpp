#pragma once
#include <engine/components/enable_tag_components.hpp>
#include <engine/core/forward_declarations.hpp>
#include <entt/entt.hpp>

namespace sdl_engine
{
   // 全システムの共通基底
   // - priority による実行順制御
   // - Update 対象のビュー補助（Renderable/Updateable/LogicUpdateable）
   class SystemInterface
   {
   public:
      SystemInterface( i32 priority_, entt::registry& registry_ );
      virtual ~SystemInterface();

      // フレーム更新の入口（派生が実装）
      virtual void update( const FrameData& frame_ ) = 0;

      // 実行順（小さいほど先）
      i32 priority() const { return _priority; };

      // レジストリ参照
      entt::registry& registry() { return registry_; };

      // Render 対象のビュー取得
      template<typename... Components, typename... Exclude>
      auto getRenderable( entt::registry& registry_, const entt::exclude_t<Exclude...>& exclude_ = entt::exclude_t {} );

      // Update 対象のビュー取得
      template<typename... Components, typename... Exclude>
      auto getUpdateable( entt::registry& registry_, const entt::exclude_t<Exclude...>& exclude_ = entt::exclude_t {} );

      // LogicUpdate 対象のビュー取得
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
      return registry_.view<Components..., RenderableTag>( exclude_ );
   }

   template<typename... Components, typename... Exclude>
   inline auto SystemInterface::getUpdateable( entt::registry& registry_, const entt::exclude_t<Exclude...>& exclude_ )
   {
      return registry_.view<Components..., UpdateableTag>( exclude_ );
   }

   template<typename... Components, typename... Exclude>
   inline auto SystemInterface::getLogicUpdateable( entt::registry&                    registry_,
                                                    const entt::exclude_t<Exclude...>& exclude_ )
   {
      return registry_.view<Components..., LogicUpdateableTag>( exclude_ );
   }
}    // namespace sdl_engine