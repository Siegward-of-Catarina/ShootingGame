#include <SDL3/SDL_scancode.h>
#include <engine/managers/input_manager.hpp>
namespace
{

   // a ~ z + 0 ~ 9 + return,escape + right,left,down,up = 41
   constexpr u32 MAX_KEYS { 42 };

   // [ SDL_SCANCODE ]が、[ 4 ]スタートなのでそれを[ 0 ]始まりにするオフセット値

   // 4 >= SDL_SCANCODE <= 41の場合
   constexpr i32 RANGE4_TO_41_OFFSET { 4 };

   // 79 >= SDL_SCANCODE <= 82の場合
   constexpr i32 RANGE79_TO_82_OFFSET { 41 };

   i32 getOffset( i32 key_code_ )
   {

      // SDL_SCANCODE_UNKNOWNは受けない前提とする
      // a ~ z + 0 ~ 9 + return,escape
      if ( key_code_ <= SDL_SCANCODE_ESCAPE ) { return RANGE4_TO_41_OFFSET; }
      // right & left & down & up keys
      if ( SDL_SCANCODE_RIGHT <= key_code_ && key_code_ <= SDL_SCANCODE_UP ) { return RANGE79_TO_82_OFFSET; }

      return 0;    // 受け付けないキー
   }
}    // namespace
namespace sdl_engine
{
   InputManager::InputManager() : _key_state { MAX_KEYS, KeyState::KeyNone }, _down_keys {}, _up_keys {} {}
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
      for ( size_t i = 0; i < _down_keys.size(); i++ )
      {
         _key_state[ _down_keys.front() ] = KeyState::KeyPress;
         _down_keys.pop();
      }
      for ( size_t i = 0; i < _up_keys.size(); i++ )
      {
         _key_state[ _up_keys.front() ] = KeyState::KeyNone;
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
//
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
//
// SDL_SCANCODE_RETURN = 40, 36
// SDL_SCANCODE_ESCAPE = 41, 37 -4

// SDL_SCANCODE_RIGHT = 79, 38 -41
// SDL_SCANCODE_LEFT = 80, 39
// SDL_SCANCODE_DOWN = 81, 40
// SDL_SCANCODE_UP = 82, 41