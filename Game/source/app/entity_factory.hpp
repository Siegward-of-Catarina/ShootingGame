#pragma once
#include <engine/forward.hpp>
#include <engine/utils.hpp>
namespace myge
{
   class EntityFactory
   {
   public:
      void createEntities( sdl_engine::GameContext& context_, json& data_ );
   };
}    // namespace myge
