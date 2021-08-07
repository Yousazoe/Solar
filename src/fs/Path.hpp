#pragma once
#include<filesystem>

namespace tutorial::fs
{
	using Path = std::filesystem::path;

	// aaa/b.txt
	// path::parent_path   ->  aaa
	// path::filename -> b.txt
	// path::stem  -> b

}

