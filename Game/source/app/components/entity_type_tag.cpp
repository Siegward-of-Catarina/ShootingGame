#include <app/components/entity_type_tag.hpp>
namespace myge
{

   void emplaceEntityTypeTag( entt::registry& registry_, entt::entity entity_, std::string_view type_ )
   {
      if ( type_ == "Background" ) { registry_.emplace<BackgroundTag>( entity_ ); }
      else if ( type_ == "Player" ) { registry_.emplace<PlayerTag>( entity_ ); }
      else if ( type_ == "Enemy" ) { registry_.emplace<EnemyTag>( entity_ ); }
   }
}    // namespace myge
