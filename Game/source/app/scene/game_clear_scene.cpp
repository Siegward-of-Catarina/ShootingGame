// ゲームクリアシーン
// - Continue(=Start) でリトライ、Exitでタイトルへ戻る
// my header
#include <app/scene/game_clear_scene.hpp>
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
#include <app/systems/score_system.hpp>
// core
#include <engine/core.hpp>
// event
#include <app/event/key_down_event.hpp>
#include <app/event/menu_button_event.hpp>
// session
#include <app/session/score_session.hpp>

myge::GameClearScene::GameClearScene( const sdl_engine::SceneDependencies& dependencies_ )
  : Scene { dependencies_, "game_data/scene_data/game_clear_scene_data.json" }
{
}

myge::GameClearScene::~GameClearScene()
{
   // 追加したシステムの登録解除
   systemManager().removeSystem<HighlightSystem>();
   systemManager().removeSystem<MenuSystem>();
   systemManager().removeSystem<OutOfScreenSystem>();
   systemManager().removeSystem<ScoreSystem>();

   // 本シーン所属のエンティティを破棄
   auto& reg { registry() };
   for ( auto entt : reg.view<AffilTitleScene>() )
   {
      if ( reg.valid( entt ) ) { reg.destroy( entt ); }
   }
}

void myge::GameClearScene::start() {}    // 初期化処理（現状なし）

void myge::GameClearScene::update( [[maybe_unused]] const sdl_engine::FrameData& frame_ ) {}

void myge::GameClearScene::addSystems()
{
   // 実行順は第1引数の優先度で制御
   systemManager().addSystem( std::make_unique<HighlightSystem>( 95, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<MenuSystem>( 96, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<OutOfScreenSystem>( 97, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<ScoreSystem>( 98, registry(), eventListener() ) );
}

void myge::GameClearScene::createEntities()
{
   // JSONからUI等を生成
   auto& scene_data { sceneData() };
   if ( auto data { sdl_engine::tryGetJson( scene_data, "Entities" ) }; data )
   {
      EntityFactory factory { registry(), resourceManager() };
      factory.createEntities( *data, typeid( AffilTitleScene ) );
   }
}

void myge::GameClearScene::setupEventHandlers()
{
   // メニュー決定イベントを購読
   eventListener().connect<&GameClearScene::onContinueMenuAction, MenuButtonEvent>( this );
   // インプットSEイベントを購読
   eventListener().connect<&GameClearScene::onAppendInputSE, KeyDownEvent>( this );
}

void myge::GameClearScene::onContinueMenuAction( const MenuButtonEvent& e )
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
void myge::GameClearScene::onAppendInputSE( const KeyDownEvent& e )
{
   // 入力音を生成（再生はSoundSystemで処理、生成後は自動破棄）
   EntityFactory factory { registry(), resourceManager() };
   if ( e.down_key != KeyDownEvent::EnableKeys::Space ) { factory.createSoundEffect( "select", 0, 0.3f, 0.0f ); }
   else { factory.createSoundEffect( "decide", 0, 0.3f, 0.0f ); }
}