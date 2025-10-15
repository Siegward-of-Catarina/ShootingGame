#pragma once
namespace sdl_engine
{
	// レンダー対象を指すタグ
	struct RenderableTag
	{
	};
	// transform更新のようなロジック外の更新対象を指すタグ
	// 単純に更新をかけるかどうか
	struct UpdateableTag {};
	// ゲームロジックや判定等の更新対象を指すタグ
	struct LogicUpdateableTag
	{
	};


}    // namespace sdl_engine