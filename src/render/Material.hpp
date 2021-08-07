#pragma once
#include<render/MaterialInstance.hpp>
#include<render/MaterialBlueprint.hpp>
#include<render/MaterialView.hpp>

namespace tutorial::graphics::mtl
{
	
	// MaterialBlueprint pbr = matlib.find("pbr");
	// auto material = pbr.create_instance();
	// material.bind({ MaterialPbr::albedo, "a.jpg" });
	// material.bind( 
			//{ MaterialPbr::albedo, "a.jpg" }, 
			//{ MaterialPbr::roughness, 0.5f },
			//{ MaterialPbr::metallic, 1.0f }
		//);

	// MaterialView mat_view { material };
	// for(property : mat_view)  
	//	  gui(property);					// modify data here


	// MaterialSkin skin_mat;
	// skin_mat.bind({ MaterialSkin::blur_uv, tex_blur_uv });
}
