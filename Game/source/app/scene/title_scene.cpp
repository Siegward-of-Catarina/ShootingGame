// タイトルシーン
// - メニューのハイライト/決定入力を受けてシーン遷移
// - 開始時にBGMを生成して再生（SoundSystemで処理）
// my header
#include <app/scene/title_scene.hpp>
// scene
#include <app/scene/game_scene.hpp>
// factory
#include <app/entity_factory.hpp>
// component
#include <app/components/affiliation.hpp>
// system
#include <app/systems/highlight_system.hpp>
#include <app/systems/menu_system.hpp>
#include <app/systems/out_of_screen_system.hpp>
#include <app/systems/screen_bounds_system.hpp>
#include <app/systems/sprite_brink_system.hpp>
// event
#include <app/event/key_down_event.hpp>
#include <app/event/menu_button_event.hpp>
#include <engine/events/sound_events.hpp>
// core
#include <engine/core.hpp>

myge::TitleScene::TitleScene( const sdl_engine::SceneDependencies& dependencies_ )
  : Scene { dependencies_, "game_data/scene_data/title_scene_data.json" }, _scene_elapsed_time { 0 }
{
}

myge::TitleScene::~TitleScene()
{
   // 追加したシステムの登録解除
   systemManager().removeSystem<SpriteBrinkSystem>();
   systemManager().removeSystem<ScreenBoundsSystem>();
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

void myge::TitleScene::start() {}    // 初期化処理（現状なし）

void myge::TitleScene::update( [[maybe_unused]] const sdl_engine::FrameData& frame_ ) {}

void myge::TitleScene::addSystems()
{
   // 実行順は第1引数の優先度で制御（小さいほど先）
   systemManager().addSystem( std::make_unique<SpriteBrinkSystem>( 94, registry() ) );
   systemManager().addSystem( std::make_unique<ScreenBoundsSystem>( 95, registry() ) );
   systemManager().addSystem( std::make_unique<HighlightSystem>( 95, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<MenuSystem>( 96, registry(), eventListener() ) );
   systemManager().addSystem( std::make_unique<OutOfScreenSystem>( 97, registry(), eventListener() ) );
}

void myge::TitleScene::onTitleMenuAction( const MenuButtonEvent& e )
{
   // メニュー決定時のアクション
   switch ( e.button_type )
   {
      case ButtonUI::Type::Start :
         // タイトルBGMをフェードアウトしてゲームへ
         eventListener().trigger<sdl_engine::StopBGMEvent>( { 1000 } );
         sceneManager().setNextScene( std::make_unique<GameScene>( sceneDependencies() ) );
         break;
      case ButtonUI::Type::Exit : sceneManager().quitGame(); break;
   }
}

void myge::TitleScene::onAppendInputSE( const KeyDownEvent& e )
{
   // 入力音を生成（再生はSoundSystemで処理、生成後は自動破棄）
   EntityFactory factory { registry(), resourceManager() };
   if ( e.down_key != KeyDownEvent::EnableKeys::Space ) { factory.createSoundEffect( "select", 0, 0.5f ); }
   else { factory.createSoundEffect( "decide", 0, 0.5f ); }
}

void myge::TitleScene::createEntities()
{
   // JSONからタイトル用エンティティを生成し、BGMも生成
   auto& scene_data { sceneData() };
   if ( auto data { sdl_engine::tryGetJson( scene_data, "Entities" ) }; data )
   {
      EntityFactory factory { registry(), resourceManager() };
      factory.createEntities( *data, typeid( AffilTitleScene ) );
      factory.createBGM( "bgm_title", -1, 0, 0.5f );    // ループ再生
   }
}

void myge::TitleScene::setupEventHandlers()
{
   // メニューと入力SEイベントを購読
   eventListener().connect<&TitleScene::onTitleMenuAction, MenuButtonEvent>( this );
   eventListener().connect<&TitleScene::onAppendInputSE, KeyDownEvent>( this );
}