#include <app/components/bounding_box.hpp>
#include <app/components/damage.hpp>
#include <app/components/entity_type_tag.hpp>
#include <app/systems/collision_system.hpp>
#include <engine/components/transform.hpp>
#include <engine/events/event_listener.hpp>
#include <engine/math.hpp>
// event
#include <app/event/dead_event.hpp>
#include <app/event/hit_event.hpp>
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
     : SystemInterface { priority_, registry_ }, _event_listener { event_listener_ }, _hit_entity_pairs {}
   {
   }
   CollisionSystem::~CollisionSystem() {}
   void CollisionSystem::update( [[maybe_unused]] const sdl_engine::FrameData& frame_ )
   {
      _hit_entity_pairs.clear();
      // common collision
      auto commonCollision = [ & ]( entt::entity&                                target_,
                                    sdl_engine::Transform&                       target_trfm_,
                                    myge::BoundingBox&                           target_box_,
                                    std::unordered_map<u32, std::vector<Enemy>>& enemies_ )
      {
         auto [ gx, gy ] { getGridCoord( target_trfm_.x, target_trfm_.y ) };
         // 弾丸のいるグリッドとその周囲8マスをチェック
         for ( i32 row { -1 }; row <= 1; row++ )    // 行 [ 上(-1), 現在グリッド, 下(+1) ]
         {
            for ( i32 col { -1 }; col <= 1; col++ )    // 列 [ 左(-1), 現在グリッド, 右(+1) ]
            {
               /* 現在グリッドとその周囲8マス
                * [ gx-1,gy-1 ] [ gx,gy-1 ] [ gx+1,gy-1 ]
                * [ gx-1,gy   ] [ gx,gy   ] [ gx+1,gy   ]
                * [ gx-1,gy+1 ] [ gx,gy+1 ] [ gx+1,gy+1 ]
                */
               auto tx { gx + col };
               auto ty { gy + row };

               // グリッド外ならスキップ
               if ( tx < 0 || ty < 0 || tx >= GRID_WIDTH || ty >= GRID_HEIGHT ) { continue; }

               // グリッドを線形化
               auto id { ty * GRID_WIDTH + tx };

               // グリッド内のみ判定する
               for ( auto& enemy : enemies_[ id ] )
               {
                  sdl_engine::Vector2_f32 dir { target_trfm_.x - enemy.trfm.x, target_trfm_.y - enemy.trfm.y };
                  f32                     rad = target_box_.radius + enemy.box.radius;
                  rad *= rad;
                  auto len { dir.lengthSq() };
                  if ( len <= rad )
                  {
                     SDL_Log( "hit" );
                     // first:はエネミーサイド、second:はプレイヤーサイド
                     _hit_entity_pairs.emplace_back( enemy.entity, target_ );
                     // 一つの弾丸は一つの敵にしか当たらないためgotoで抜ける
                     return;
                  }
               }
            }
         }
      };
      // first transform second boundingbox
      std::unordered_map<u32, std::vector<Enemy>> enemies {};
      auto&                                       reg { registry() };
      for ( auto [ entt, trfm, box ] : getLogicUpdateable<sdl_engine::Transform, BoundingBox, EnemyTag>( reg ).each() )
      {
         if ( !reg.valid( entt ) ) { continue; }
         if ( box.harf_width * 2 < CELL_SIZE || box.harf_hegiht * 2 < CELL_SIZE )
         {
            auto [ gx, gy ] { getGridCoord( trfm.x, trfm.y ) };
            // グリッドを線形化
            u32 id { gy * GRID_WIDTH + gx };
            enemies[ id ].emplace_back( Enemy { entt, trfm, box } );
         }
      }

      // 弾丸と敵の当たり判定
      for ( auto [ bullet, bu_trfm, bu_box ] :
            getLogicUpdateable<sdl_engine::Transform, BoundingBox, PlayerBulletTag>( reg ).each() )
      {
         if ( !reg.valid( bullet ) ) { continue; }
         commonCollision( bullet, bu_trfm, bu_box, enemies );
      }

      for ( auto [ player, trfm, box ] :
            getLogicUpdateable<sdl_engine::Transform, BoundingBox, PlayerTag>( reg, entt::exclude<DamageEffect> )
              .each() )
      {
         if ( !reg.valid( player ) || reg.all_of<DamageEffect>( player ) ) { continue; }
         commonCollision( player, trfm, box, enemies );
      }

      if ( !_hit_entity_pairs.empty() ) { _event_listener.trigger<HitEvent>( { _hit_entity_pairs } ); }
   }
}    // namespace myge
