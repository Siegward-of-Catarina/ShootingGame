#pragma once
#include <engine/forward.hpp>
namespace myge
{
	//タグコンポーネントだが、
	// entering以降のためのintervalを持たせる。
	struct WaitTag {
		f32 wait_time;
		f32 elapsed;
	};
	struct EnteringTag
	{
	};
	struct ActiveTag
	{
	};
	struct DyingTag
	{
	};
	struct DeadTag
	{
	};
}    // namespace myge