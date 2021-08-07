#include<gfx/Gfx.hpp>

#include<map>
#include<D3Dcompiler.h>
#include<wrl.h>
#include<stdexcept>

namespace tutorial::gfx
{
	using Microsoft::WRL::ComPtr;

	auto compile_shader(
		ShaderStage type, const std::string& source_code,
		const std::string& entry, const std::string& profile,
		bool save_bytecode
	) -> Shader
	{
		/*	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if _DEBUG
			flags |= D3DCOMPILE_DEBUG;
			flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif*/
		ComPtr<ID3DBlob> shader;
		ComPtr<ID3DBlob> blob;
		ComPtr<ID3DBlob> error_blob;
		if (FAILED(
			D3DCompile(
				source_code.c_str(), source_code.length(),
				NULL, NULL, NULL, entry.c_str(), profile.c_str(), 0, 0,
				&blob, &error_blob
			))) {
			std::string error_msg = "Shader compilation failed : \n";
			if (error_blob) {
				error_msg += static_cast<const char*>(error_blob->GetBufferPointer());
			}
			throw std::runtime_error(error_msg);
		}
		return create_shader(type,
			std::string{ static_cast<const char*>(blob->GetBufferPointer()), blob->GetBufferSize() }
		);
	}

	auto shader_reflection(const std::string_view& compiled_code)
		-> ShaderReflection
	{
		ShaderReflection reflection{};
		ID3D11ShaderReflection* reflector;
		HRESULT hr = D3DReflect(compiled_code.data(), compiled_code.size(), IID_ID3D11ShaderReflection, (void**)&reflector); 
		D3D11_SHADER_DESC desc;
		reflector->GetDesc(&desc);

		std::map<std::string, ShaderReflection::ConstBufferBinding> cbuffers;
		for (UINT i = 0; i < desc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC input_desc;
			reflector->GetResourceBindingDesc(i, &input_desc);
			if (input_desc.Type == D3D_SIT_CBUFFER || input_desc.Type == D3D_SIT_TBUFFER)
			{
				ShaderReflection::ConstBufferBinding cb;
				cb.name = input_desc.Name;
				cb.index = input_desc.BindPoint;
				cb.size = 0u;
				cbuffers.insert({ cb.name, cb });
			}
			else if (input_desc.Type == D3D_SIT_TEXTURE || input_desc.Type == D3D_SIT_STRUCTURED)
			{
				ShaderReflection::SrvBinding srv;
				srv.name = input_desc.Name;
				srv.index = input_desc.BindPoint;
				reflection.srv_bindings.push_back(srv);
			}
			else if (input_desc.Type == D3D_SIT_SAMPLER)
			{
			}
			else if (input_desc.Type == D3D_SIT_UAV_RWTYPED || input_desc.Type == D3D_SIT_UAV_RWSTRUCTURED
				|| input_desc.Type == D3D_SIT_BYTEADDRESS || input_desc.Type == D3D_SIT_UAV_RWBYTEADDRESS
				|| input_desc.Type == D3D_SIT_UAV_APPEND_STRUCTURED || input_desc.Type == D3D_SIT_UAV_CONSUME_STRUCTURED
				|| input_desc.Type == D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER)
			{
			}
		}

		for (UINT i = 0; i < desc.ConstantBuffers; i++)
		{
			ID3D11ShaderReflectionConstantBuffer* cbuffer = reflector->GetConstantBufferByIndex(i);
			D3D11_SHADER_BUFFER_DESC cbuffer_desc;
			cbuffer->GetDesc(&cbuffer_desc);
			if (cbuffer_desc.Type == D3D_CT_CBUFFER || cbuffer_desc.Type == D3D_CT_TBUFFER)
			{
				cbuffers.find(cbuffer_desc.Name)->second.size = cbuffer_desc.Size;
				for (UINT j = 0; j < cbuffer_desc.Variables; j++)
				{
					ID3D11ShaderReflectionVariable* var = cbuffer->GetVariableByIndex(j);
					D3D11_SHADER_VARIABLE_DESC var_desc;
					var->GetDesc(&var_desc);
					ShaderReflection::ShaderParam param;
					param.cbuffer_name = cbuffer_desc.Name;
					param.name = var_desc.Name;
					param.offset = var_desc.StartOffset;
					param.size = var_desc.Size;
					reflection.uniforms.push_back(param);
				}
			}
		}

		for (auto& [i, cbuffer] : cbuffers)
		{
			reflection.cbuffer_bindings.push_back(cbuffer);
		}

		return reflection;
	}
}