#pragma once

#include <core/fwd.hpp>
#include <core/core.hpp>

namespace myge
{
   class Scene
   {
   public:
      Scene( SceneInitDesc desc_ );
      virtual ~Scene() {};
      virtual void proc( float _delta ) = 0;

      Renderer& renderer();
      ResourceManager& resourceManager();
   private:
       Renderer& _renderer;
       Sequencer& _sequencer;
       ResourceManager& _resource_manager;
   };
}    // namespace myge