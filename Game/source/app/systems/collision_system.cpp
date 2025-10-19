#include <app/components/bounding_box.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/systems/collision_system.hpp>
#include <engine/components/transform.hpp>
#include <engine/events/event_listener.hpp>
#include <engine/math.hpp>
// event
#include <app/event/dead_event.hpp>
namespace
{
   constexpr f32 CELL_SIZE   = 100.0f;
   constexpr u32 GRID_WIDTH  = 6;
   constexpr u32 GRID_HEIGHT = 8;

   auto getGridCoord( f32 x_, f32 y_ )
   {
      return std::pair { std::clamp( static_cast<u32>( x_ / CELL_SIZE ), 0u, GRID_WIDTH - 1 ),
                         std::clamp( static_cast<u32>( y_ / CELL_SIZE ), 0u, GRID_HEIGHT - 1 ) };
   }
   struct Enemy
   {
      entt::entity           entity;
      sdl_engine::Transform& trfm;
      myge::BoundingBox&     box;
   };
}    // namespace
namespace myge
{
   CollisionSystem::CollisionSystem( i32                        priority_,
                                     entt::registry&            registry_,
                                     sdl_engine::EventListener& event_listener_ )
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }
   {
   }
   CollisionSystem::~CollisionSystem() {}
   void CollisionSystem::update( [[maybe_unused]] const sdl_engine::FrameData& frame_ )
   {
      // first transform second boundingbox
      std::unordered_map<u32, std::vector<Enemy> > enemies {};

      for ( auto [ entt, trfm, box ] :
            getLogicUpdateable<sdl_engine::Transform, BoundingBox, EnemyTag>( registry() ).each() )
      {
         if ( box.harf_width * 2 < CELL_SIZE || box.harf_hegiht * 2 < CELL_SIZE )
         {
            auto [ gx, gy ] { getGridCoord( trfm.x, trfm.y ) };
            // グリッドを線形化
            u32 id { gy * GRID_WIDTH + gx };

            enemies[ id ].emplace_back( Enemy { entt, trfm, box } );
         }
      }

      std::unordered_set<entt::entity> hit_entites {};

      for ( auto [ bu, bu_trfm, bu_box ] :
            getLogicUpdateable<sdl_engine::Transform, BoundingBox, PlayerBulletTag>( registry() ).each() )
      {

         auto [ gx, gy ] { getGridCoord( bu_trfm.x, bu_trfm.y ) };

         for ( i32 row { -1 }; row <= 1; row++ )
         {
            for ( i32 col { -1 }; col <= 1; col++ )
            {

               auto tx { gx + col };
               auto ty { gy + row };

               if ( tx < 0 || ty < 0 || tx >= GRID_WIDTH || ty >= GRID_HEIGHT ) { continue; }

               auto id { ty * GRID_WIDTH + tx };

               for ( auto& enemy : enemies[ id ] )
               {
                  sdl_engine::Vector2_f32 dir { bu_trfm.x - enemy.trfm.x, bu_trfm.y - enemy.trfm.y };
                  f32                     rad = bu_box.radius + enemy.box.radius;
                  rad *= rad;
                  auto len { dir.lengthSq() };
                  if ( len <= rad )
                  {
                     SDL_Log( "hit" );
                     hit_entites.emplace( bu );
                     hit_entites.emplace( enemy.entity );
                     // 一つの弾丸は一つの敵にしか当たらないためgotoで抜ける
                     goto next_bullet;
                  }
               }
            }
         }
      // 次の弾丸へ
      next_bullet:;
      }

      if ( !hit_entites.empty() ) { _event_listener.trigger<DeadEvent>( { hit_entites } ); }
   }
}    // namespace myge
