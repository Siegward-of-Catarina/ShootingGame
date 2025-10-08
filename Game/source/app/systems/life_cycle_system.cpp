#include <app/components/bounding_box.hpp>
#include <app/components/lifecycle_tags.hpp>
#include <app/systems/life_cycle_system.hpp>
#include <engine/basic_component.hpp>
#include <engine/core.hpp>
namespace
{
   // �\�[�g��K�v�Ƃ��郌�C���̔��ʃr�b�g�t���O
   constexpr u8 NEED_SORT_BACKGROUND_TAG = 1;
   constexpr u8 NEED_SORT_GAMESPRITE_TAG = 2;
   constexpr u8 NEED_SORT_UI_TAG         = 4;
   constexpr u8 NEED_SORT_TEXT_TAG       = 8;
   constexpr u8 NEED_SORT_FADE_TAG       = 16;
   constexpr u8 NEED_SORT_ALL            = 31;
   u8           need_sort { 0 };

}    // namespace
namespace myge
{
   LifeCycleSystem::LifeCycleSystem( i32 priority_ ) : SystemInterface { priority_ } {}
   LifeCycleSystem::~LifeCycleSystem() {}
   void LifeCycleSystem::update( sdl_engine::GameContext& context_ )
   {
      //
      auto&                     registry { context_.getRegistry() };
      std::vector<entt::entity> change_tag_entities {};
      // ��ʊO�ő��݂��Ă���Ƃ�
      for ( auto [ entity ] : registry.view<EnteringTag>().each() )
      {
         // �o�ꎞ�Ƀ\�[�g�Ώۂɂ���
         need_sort = NEED_SORT_ALL;

         // �o�E���f�B���O�{�b�N�X�����I�u�W�F�N�g�̏ꍇ
         if ( registry.all_of<BoundingBox>( entity ) )
         {
            // ��x�Q�[���G���A���ɓ�������A�N�e�B�u��ԂƂ���
            auto& box { registry.get<BoundingBox>( entity ) };
            if ( box.state == BoundingBox::State::Inside ) { change_tag_entities.emplace_back( entity ); }
         }
         else    // ���̑��͏o��������A�N�e�B�u��
         {
            change_tag_entities.emplace_back( entity );
         }
      }
      for ( auto [ entity ] : registry.view<ActiveTag>().each() ) {}
      for ( auto [ entity ] : registry.view<DyingTag>().each() ) {}
      for ( auto [ entity ] : registry.view<DeadTag>().each() )
      {
         // �폜�Ώۂ̃G���e�B�e�B���ǂ̃��C���ɂ��邩���ׂ�
         if ( registry.all_of<sdl_engine::RenderBackgroundTag>( entity ) ) { need_sort |= NEED_SORT_BACKGROUND_TAG; }
         else if ( registry.all_of<sdl_engine::RenderGameSpriteTag>( entity ) )
         {
            need_sort |= NEED_SORT_GAMESPRITE_TAG;
         }
         else if ( registry.all_of<sdl_engine::RenderUITag>( entity ) ) { need_sort |= NEED_SORT_UI_TAG; }
         else if ( registry.all_of<sdl_engine::RenderTextTag>( entity ) ) { need_sort |= NEED_SORT_TEXT_TAG; }
         else if ( registry.all_of<sdl_engine::RenderFadeTag>( entity ) ) { need_sort |= NEED_SORT_FADE_TAG; }
         registry.destroy( entity );
         // �P�̂ł��폜������ă\�[�g
      }

      // �~���ɃG���e�B�e�B���\�[�g
      if ( need_sort )
      {
         // �܂���Sprite���\�[�g����͌Œ�
         registry.sort<sdl_engine::Sprite>( []( const sdl_engine::Sprite& l_sprt, const sdl_engine::Sprite& r_sprt )
                                            { return l_sprt.render_order > r_sprt.render_order; } );
         // Sprite�����Transform���\�[�g������Œ�
         registry.sort<sdl_engine::Transform, sdl_engine::Sprite>();
         // ��������͍폜���ꂽ�G���e�B�e�B���������Ă������C�����ƂɃ\�[�g���K�v�Ȃ�
         // Sprite����Ƀ\�[�g����
         if ( need_sort & NEED_SORT_BACKGROUND_TAG )
         {
            registry.sort<sdl_engine::RenderBackgroundTag, sdl_engine::Sprite>();
         }
         if ( need_sort & NEED_SORT_GAMESPRITE_TAG )
         {
            registry.sort<sdl_engine::RenderGameSpriteTag, sdl_engine::Sprite>();
         }
         if ( need_sort & NEED_SORT_UI_TAG ) { registry.sort<sdl_engine::RenderUITag, sdl_engine::Sprite>(); }
         if ( need_sort & NEED_SORT_TEXT_TAG ) { registry.sort<sdl_engine::RenderTextTag, sdl_engine::Sprite>(); }
         if ( need_sort & NEED_SORT_FADE_TAG ) { registry.sort<sdl_engine::RenderFadeTag, sdl_engine::Sprite>(); }
         // �\�[�g����
         need_sort = 0;
      }

      for ( auto& entity : change_tag_entities )
      {
         if ( registry.all_of<EnteringTag>( entity ) )
         {

            registry.remove<EnteringTag>( entity );
            registry.emplace<ActiveTag>( entity );
            registry.emplace<sdl_engine::LogicUpdateableTag>( entity );
         }
      }
   }
}    // namespace myge
