#include <SDL3/SDL_scancode.h>
#include <engine/managers/input_manager.hpp>
namespace
{

   // a ~ z + 0 ~ 9 + return,escape,backspace,tab,space + right,left,down,up = 45
   constexpr u32 MAX_KEYS { 45 };

   // [ SDL_SCANCODE ]が、[ 4 ]スタートなのでそれを[ 0 ]始まりにするオフセット値

   // 4 >= SDL_SCANCODE <= 44の場合
   constexpr i32 RANGE4_TO_44_OFFSET { 4 };

   // 79 >= SDL_SCANCODE <= 82の場合
   constexpr i32 RANGE79_TO_82_OFFSET { 38 };

   i32 getOffset( i32 key_code_ )
   {

      // SDL_SCANCODE_UNKNOWNは受けない前提とする
      // a ~ z + 0 ~ 9 + return,escape,backspace,tab,space
      if ( key_code_ <= SDL_SCANCODE_SPACE ) { return RANGE4_TO_44_OFFSET; }
      // right & left & down & up keys
      if ( SDL_SCANCODE_RIGHT <= key_code_ && key_code_ <= SDL_SCANCODE_UP ) { return RANGE79_TO_82_OFFSET; }

      return 0;    // 受け付けないキー
   }
}    // namespace
namespace sdl_engine
{
   InputManager::InputManager()
     : _key_state { MAX_KEYS, KeyState::KeyNone }, _down_keys {}, _up_keys {}, _any_key_inputs { false }
   {
   }
   InputManager::~InputManager() {}
   void InputManager::handleEvent( const SDL_Event& event_ )
   {
      i32 scancode = static_cast<i32>( event_.key.scancode );

      if ( event_.type == SDL_EVENT_KEY_DOWN )
      {
         if ( auto offset = getOffset( scancode ); offset )
         {
            auto idx = scancode - offset;

            if ( _key_state[ idx ] == KeyState::KeyNone )
            {
               _key_state[ idx ] = KeyState::KeyDown;
               // 押されたキーのインデックスを記憶
               _down_keys.emplace( idx );
               // キーが押されたとだけのフラグを上げる
               _any_key_inputs = true;
            }
         }
      }
      if ( event_.type == SDL_EVENT_KEY_UP )
      {
         if ( auto offset = getOffset( scancode ); offset )
         {
            auto idx          = scancode - offset;
            _key_state[ idx ] = KeyState::KeyUp;
            // 押されたキーのインデックスを記憶
            _up_keys.emplace( idx );
         }
      }
   }
   void InputManager::update()
   {
      while ( !_down_keys.empty() )
      {
         auto idx { _down_keys.front() };
         _key_state[ idx ] = KeyState::KeyPress;
         _down_keys.pop();
      }
      while ( !_up_keys.empty() )
      {
         auto idx { _up_keys.front() };
         _key_state[ idx ] = KeyState::KeyNone;
         _up_keys.pop();
      }
   }
   bool InputManager::isKeyDown( i32 scancode_ )
   {
      if ( auto offset = getOffset( scancode_ ); offset )
      {
         auto idx = scancode_ - offset;
         return _key_state[ idx ] == KeyState::KeyDown;
      }
      return false;
   }
   bool InputManager::isKeyPress( i32 scancode_ )
   {
      if ( auto offset = getOffset( scancode_ ); offset )
      {
         auto idx = scancode_ - offset;
         return ( _key_state[ idx ] == KeyState::KeyPress || _key_state[ idx ] == KeyState::KeyDown );
      }
      return false;
   }
   bool InputManager::isKeyUp( i32 scancode_ )
   {
      if ( auto offset = getOffset( scancode_ ); offset )
      {
         auto idx = scancode_ - offset;
         return _key_state[ idx ] == KeyState::KeyUp;
      }
      return false;
   }
   bool InputManager::isKeyDown( std::string_view key_name_ )
   {
      auto scancode { SDL_GetScancodeFromName( key_name_.data() ) };
      if ( scancode == SDL_SCANCODE_UNKNOWN ) { return false; }
      return isKeyDown( scancode );
   }
   bool InputManager::isKeyPress( std::string_view key_name_ )
   {
      auto scancode { SDL_GetScancodeFromName( key_name_.data() ) };
      if ( scancode == SDL_SCANCODE_UNKNOWN ) { return false; }
      return isKeyPress( scancode );
   }
   bool InputManager::isKeyUp( std::string_view key_name_ )
   {
      auto scancode { SDL_GetScancodeFromName( key_name_.data() ) };
      if ( scancode == SDL_SCANCODE_UNKNOWN ) { return false; }
      return isKeyUp( scancode );
   }
   bool InputManager::isAnyKeydown() { return !_down_keys.empty(); }
}    // namespace sdl_engine
// SDL_SCANCODE_A = 4, 0
// SDL_SCANCODE_B = 5, 1
// SDL_SCANCODE_C = 6, 2
// SDL_SCANCODE_D = 7, 3
// SDL_SCANCODE_E = 8, 4
// SDL_SCANCODE_F = 9, 5
// SDL_SCANCODE_G = 10, 6
// SDL_SCANCODE_H = 11, 7
// SDL_SCANCODE_I = 12, 8
// SDL_SCANCODE_J = 13, 9
// SDL_SCANCODE_K = 14, 10
// SDL_SCANCODE_L = 15, 11
// SDL_SCANCODE_M = 16, 12
// SDL_SCANCODE_N = 17, 13
// SDL_SCANCODE_O = 18, 14
// SDL_SCANCODE_P = 19, 15
// SDL_SCANCODE_Q = 20, 16
// SDL_SCANCODE_R = 21, 17
// SDL_SCANCODE_S = 22, 18
// SDL_SCANCODE_T = 23, 19
// SDL_SCANCODE_U = 24, 20
// SDL_SCANCODE_V = 25, 21
// SDL_SCANCODE_W = 26, 22
// SDL_SCANCODE_X = 27, 23
// SDL_SCANCODE_Y = 28, 24
// SDL_SCANCODE_Z = 29, 25
// SDL_SCANCODE_1 = 30, 26
// SDL_SCANCODE_2 = 31, 27
// SDL_SCANCODE_3 = 32, 28
// SDL_SCANCODE_4 = 33, 29
// SDL_SCANCODE_5 = 34, 30
// SDL_SCANCODE_6 = 35, 31
// SDL_SCANCODE_7 = 36, 32
// SDL_SCANCODE_8 = 37, 33
// SDL_SCANCODE_9 = 38, 34
// SDL_SCANCODE_0 = 39, 35
// SDL_SCANCODE_RETURN = 40, 36
// SDL_SCANCODE_ESCAPE = 41, 37
// SDL_SCANCODE_BACKSPACE = 42, 38
// SDL_SCANCODE_TAB = 43, 39
// SDL_SCANCODE_SPACE = 44, 40 -4
//
// SDL_SCANCODE_RIGHT = 79,41 -38
// SDL_SCANCODE_LEFT = 80, 42
// SDL_SCANCODE_DOWN = 81, 43
// SDL_SCANCODE_UP = 82, 44