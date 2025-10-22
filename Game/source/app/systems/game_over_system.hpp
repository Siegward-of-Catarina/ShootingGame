#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
namespace myge
{
   struct GameOverEvent;
   class GameOverSystem final : public sdl_engine::SystemInterface
   {
   public:
      GameOverSystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      virtual ~GameOverSystem() override;

      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      void onGameOver( GameOverEvent& e );

   private:
      sdl_engine::EventListener&                         _event_listener;
      std::vector<std::pair<entt::entity, entt::entity>> _hit_entity_pairs;
      f32                                                _game_over_timer { 0.0f };
   };
}    // namespace myge
