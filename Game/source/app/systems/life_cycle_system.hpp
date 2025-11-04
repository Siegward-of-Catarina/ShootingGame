#pragma once

#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace sdl_engine
{
   struct SpriteAnimEndEvent;
}
namespace myge
{
   struct DeadEvent;
   struct DeadLaserEvent;
   class LifeCycleSystem final : public sdl_engine::SystemInterface
   {
   public:
      LifeCycleSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~LifeCycleSystem() override;
      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      void WaitEntityUpdate( const sdl_engine::FrameData& frame_ );
      void EnteringEntityUpdate( const sdl_engine::FrameData& frame_ );
      void ActiveEntityUpdate( const sdl_engine::FrameData& frame_ );
      void DyingEntityUpdate( const sdl_engine::FrameData& frame_ );
      void DeadEntityUpdate( const sdl_engine::FrameData& frame_ );

   private:
      void onEntityDead( DeadEvent& e );
      void onLaserDead( DeadLaserEvent& e );
      void onSpriteAnimEnd( sdl_engine::SpriteAnimEndEvent& e );

   private:
      static constexpr u8 NEED_SORT_BACKGROUND_TAG = 1;
      static constexpr u8 NEED_SORT_GAMESPRITE_TAG = 2;
      static constexpr u8 NEED_SORT_UI_TAG         = 4;
      static constexpr u8 NEED_SORT_TEXT_TAG       = 8;
      static constexpr u8 NEED_SORT_FADE_TAG       = 16;
      static constexpr u8 NEED_SORT_ALL            = 31;

   private:
      sdl_engine::EventListener& _event_listener;
      std::vector<entt::entity>  _change_tag_entities;
      std::vector<entt::entity>  _dead_entities;
      u8                         _need_sort { 0 };
   };
}    // namespace myge
