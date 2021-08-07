#pragma once
#include<core/Types.hpp>

namespace disk::gfx::detail
{
	struct StateHash
	{
		StateHash() : hash(0) {}

		union
		{
			uint32 hash;
			struct
			{
				uint32 blend : 3;
				uint32 depth_test : 1;
				uint32 depth_write : 1;
				uint32 depth_compare_func : 3;
				uint32 cull_mode : 2;
				uint32 fill_mode : 2;
				uint32 depth_clip : 1;
				uint32 material_id : 8;
				uint32 shader_id : 10;
			};
		};
	};
}
