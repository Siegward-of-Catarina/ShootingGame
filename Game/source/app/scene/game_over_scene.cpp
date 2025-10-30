// ゲームオーバーシーン
// - Continue(=Start) でリトライ、Exitでタイトルへ戻る
// my header
#include <app/scene/game_over_scene.hpp>
// scene
#include <app/scene/game_scene.hpp>
#include <app/scene/title_scene.hpp>
// factory
#include <app/entity_factory.hpp>
// component
#include <app/components/affiliation.hpp>
// system
#include <app/systems/highlight_system.hpp>
#include <app/systems/menu_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
// core
#include <engine/core.hpp>
// event
#include <app/event/menu_button_event.hpp>

myge::GameOverScene::GameOverScene( const sdl_engine::SceneDependencies& dependencies_ )
  : Scene { dependencies_, "game_data/scene_data/game_over_scene_data.json" }, _scene_elapsed_time { 0 }
{
}

myge::GameOverScene::~GameOverScene()
{
   // 追加したシステムの登録解除
   systemManager().removeSystem<HighlightSystem>();
   systemManager().removeSystem<MenuSystem>();
   systemManager().removeSystem<OutOfScreenSystem>();

   // 本シーン所属のエンティティを破棄
   auto& reg { registry() };
   for ( auto entt : reg.view<AffilTitleScene>() )
   {
      if ( reg.valid( entt ) ) { reg.destroy( entt ); }
   }
}

void myge::GameOverScene::start() {}    // 初期化処理（現状なし）

void myge::GameOverScene::update( [[maybe_unused]] const sdl_engine::FrameData& frame_ ) {}

void myge::GameOverScene::addSystems()
{
   // 実行順は第1引数の優先度で制御
   systemManager().addSystem( std::make_unique<HighlightSystem>( 95, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<MenuSystem>( 96, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<OutOfScreenSystem>( 97, registry(), eventListener() ) );
}

void myge::GameOverScene::createEntities()
{
   // JSONからUI等を生成
   auto& scene_data { sceneData() };
   if ( auto data { sdl_engine::tryGetJson( scene_data, "Entities" ) }; data )
   {
      EntityFactory factory { registry(), resourceManager() };
      factory.createEntities( *data, typeid( AffilTitleScene ) );
   }
}

void myge::GameOverScene::setupEventHandlers()
{
   // メニュー決定イベントを購読
   eventListener().connect<&GameOverScene::onContinueMenuAction, MenuButtonEvent>( this );
}

void myge::GameOverScene::onContinueMenuAction( const MenuButtonEvent& e )
{
   // Start=リトライ、Exit=タイトルへ
   switch ( e.button_type )
   {
      case ButtonUI::Type::Start :
         sceneManager().setNextScene( std::make_unique<GameScene>( sceneDependencies() ) );
         break;
      case ButtonUI::Type::Exit :
         sceneManager().setNextScene( std::make_unique<TitleScene>( sceneDependencies() ) );
         break;
   }
}