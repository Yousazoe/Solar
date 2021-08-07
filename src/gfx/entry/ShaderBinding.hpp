#pragma once
#include<tuple>
#include<vector>
#include<string>
#include<gfx/entry/Define.hpp>
#include<gfx/entry/UniformHash.hpp>

namespace tutorial::gfx
{
	struct ShaderReflection
	{
		struct ShaderParam
		{
			std::string name;
			std::string cbuffer_name;
			uint16 offset;
			uint16 size;
		};

		struct ConstBufferBinding
		{
			uint8 index;
			uint16 size;
			std::string name;
		};

		struct SrvBinding
		{
			uint8 index;
			std::string name;
		};

		ShaderReflection() = default;
		ShaderReflection(const ShaderReflection&) = delete;
		void operator=(const ShaderReflection&) = delete;
		ShaderReflection(ShaderReflection&& rhs) noexcept : 
			uniforms{ std::move(rhs.uniforms) },
			cbuffer_bindings{ std::move(rhs.cbuffer_bindings) },
			srv_bindings{ std::move(rhs.srv_bindings) }
		{
		}
		void operator=(ShaderReflection&&) = delete;

		std::vector<ShaderParam> uniforms;
		std::vector<ConstBufferBinding> cbuffer_bindings;
		std::vector<SrvBinding> srv_bindings;
	};

	struct ShaderBinding
	{
		struct Binding
		{
			ShaderStage stage;
			uint8 index;
			UniformHandle handle;
		};

		struct Location
		{
			UniformHandle buffer_handle;
			uint16 offset;
		};

		struct Uniform
		{
			UniformHandle handle;
			Location loc;
		};

		ShaderBinding() = default;
		ShaderBinding(ShaderStage stage, const ShaderReflection& reflection);
		ShaderBinding(const ShaderBinding&) = delete;
		void operator=(const ShaderBinding&) = delete;
		ShaderBinding(ShaderBinding&& rhs) noexcept { (*this) = std::forward< ShaderBinding&&>(rhs); }
		void operator=(ShaderBinding&& rhs) noexcept
		{
			cbuffer_bindings = std::move(rhs.cbuffer_bindings);
			srv_bindings = std::move(rhs.srv_bindings);
			uniform_bindings = std::move(rhs.uniform_bindings);
		}
		auto map_uniform_address(const UniformHandle& uniform_handle) const -> Location;
		auto merge(const ShaderBinding& binding) -> void;

		std::vector<Binding> cbuffer_bindings;
		std::vector<Binding> srv_bindings;
		std::vector<Uniform> uniform_bindings;
	};

	struct ConstBufferMapping
	{
		using Address = struct
		{
			UniformHandle cbuffer;
			int offset;
		};
		auto map_uniform_address(const UniformHandle& uniform_handle)->Address;
	};

}
