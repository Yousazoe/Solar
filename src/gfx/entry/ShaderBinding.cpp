#include<gfx/entry/ShaderBinding.hpp>
#include<gfx/entry/ConstBuffers.hpp>

namespace tutorial::gfx
{
	ShaderBinding::ShaderBinding(ShaderStage stage, const ShaderReflection& reflection)
	{
		for (auto& cbuffer : reflection.cbuffer_bindings)
		{
			//auto handle = UniformHash::get(cbuffer.name, UniformType::ConstBuffer);
			auto handle = detail::ConstBuffers::get_or_create(cbuffer.name, cbuffer.size);
			Binding binding{ stage, cbuffer.index, handle };
			cbuffer_bindings.emplace_back(binding);
		}

		for (auto& srv : reflection.srv_bindings)
		{
			auto handle = UniformHash::get(srv.name, UniformType::Srv);
			Binding binding{ stage, srv.index, handle };
			srv_bindings.push_back(binding);
		}

		for (auto& uniform : reflection.uniforms)
		{
			auto uniform_handle = UniformHash::get(uniform.name, UniformType::Var);
			auto cbuffer_handle = UniformHash::get(uniform.cbuffer_name, UniformType::ConstBuffer);
			Location location{ cbuffer_handle,  uniform.offset };
			uniform_bindings.push_back({ uniform_handle, location });
		}
	}

	auto ShaderBinding::map_uniform_address(const UniformHandle& uniform_handle) const
		-> Location
	{
		static Location loc = { UniformHandle::Invalid, 0 };
		auto it = std::find_if(uniform_bindings.begin(), uniform_bindings.end(),
			[&](const decltype(uniform_bindings)::value_type& uniform) {
				return uniform_handle == uniform.handle;
		});
		return it == uniform_bindings.end() ? loc : it->loc;
	}

	auto ShaderBinding::merge(const ShaderBinding& rhs)
		-> void
	{
		cbuffer_bindings.insert(
			cbuffer_bindings.end(), 
			rhs.cbuffer_bindings.begin(), rhs.cbuffer_bindings.end()
		);
		srv_bindings.insert(
			srv_bindings.end(),
			rhs.srv_bindings.begin(), rhs.srv_bindings.end()
		);

		for (auto& uniform : rhs.uniform_bindings)
		{
			auto it = std::find_if(uniform_bindings.begin(), uniform_bindings.end(),
				[&](const decltype(uniform_bindings)::value_type& ele) {
				return uniform.handle == ele.handle;
			});

			if (it == uniform_bindings.end())
			{
				uniform_bindings.push_back(uniform);
			}
		}
	}
}