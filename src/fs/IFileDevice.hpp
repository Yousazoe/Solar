#pragma once
#include<fs/IFile.hpp>
#include<memory/IAllocator.hpp>
#include<memory>

namespace tutorial::fs
{
	class IMountable { };

	template<DeviceType type>
	class IFileDevice final : public IMountable
	{
	private:
		using File = IFile<DeviceType::Disk>;

	public:
		IFileDevice() {}
		~IFileDevice() {}
		IFileDevice(const IFileDevice&) = delete;
		void operator=(const IFileDevice&) = delete;
		IFileDevice(IFileDevice&&) = delete;
		void operator=(IFileDevice&&) = delete;

	public:
		File create_file(IAllocator& allocator);
	};

}