#pragma once
#include <engine/core/forward_declarations.hpp>
#include <queue>
#include <vector>
namespace sdl_engine
{
   enum class KeyState : u8
   {
      KeyDown,
      KeyPress,
      KeyUp,
      KeyNone,
   };
   class InputManager
   {
   public:
      InputManager();
      ~InputManager();
      void handleEvent( const SDL_Event& event_ );
      /// <summary>
      /// KeyState���X�V����
      /// �Q�[�����[�v�̍Ō�ŌĂԂ��ƁI
      /// </summary>
      void update();
      bool isKeyDown( i32 scancode_ );
      bool isKeyPress( i32 scancode_ );
      bool isKeyUp( i32 scancode_ );

   private:
      std::vector<KeyState> _key_state;
      // �t���[���P�ʂ̓��̓L���[
      std::queue<i32> _down_keys;
      std::queue<i32> _up_keys;
   };
}    // namespace sdl_engine
