#pragma once
#include <engine/forward.hpp>
#include <engine/systems/system_interface.hpp>
#include <random>
namespace myge
{
   struct EnemyBossAI;
   struct ChargeEndEvent;

   class EnemyBossAISystem final : public sdl_engine::SystemInterface
   {
   public:
      EnemyBossAISystem( i32 priority_, entt::registry& registry_, sdl_engine::EventListener& event_listener_ );
      ~EnemyBossAISystem() override;

      // SystemInterface を介して継承されました
      virtual void update( const sdl_engine::FrameData& frame_ ) override;

   private:
      void onChargeEnd( ChargeEndEvent& e );

   private:
      std::mt19937                 _random_engine;
      std::discrete_distribution<> _distribution;
      sdl_engine::EventListener&   _event_listener;
   };
}    // namespace myge
