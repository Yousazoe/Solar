#pragma once
#include<fs/IFile.hpp>
#include<fs/IFileDevice.hpp>

namespace tutorial::fs
{
	template<>
	class IFileDevice<DeviceType::Disk> final : public IMountable
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

	inline IFileDevice<DeviceType::Disk>::File IFileDevice<DeviceType::Disk>::create_file(IAllocator& allocator)
	{
		return File();
	}

}