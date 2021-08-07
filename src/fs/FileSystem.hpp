#pragma once
#include<fs/IFile.hpp>
#include<fs/IFileDevice.hpp>
#include<fs/DiskFile.hpp>
#include<fs/DiskFileDevice.hpp>

namespace tutorial::fs
{
	using namespace std::filesystem;

	class FileSystem 
	{
	public:
		FileSystem(IAllocator& allocator) : _allocator(allocator) {}

	public:
		void mount(IMountable* device);
		void set_disk_device(IFileDevice<DeviceType::Disk>* device);
		auto get_disk_device() { return _disk_device; }
		template<DeviceType device_t>
		IFile<device_t> open_sync(IFileDevice<device_t>* device, const Path& path, OpenMode mode = OpenMode::READ);
		template<DeviceType device_t>
		IFile<device_t> open_async(IFileDevice<device_t>* device, const Path& path, OpenMode mode = OpenMode::READ);


	private:

	private:
		IAllocator& _allocator;
		IFileDevice<DeviceType::Disk>* _disk_device = nullptr;
	};

	inline void tutorial::fs::FileSystem::mount(IMountable* device)
	{
	}

	inline void FileSystem::set_disk_device(IFileDevice<DeviceType::Disk>* device)
	{
		_disk_device = device;
	}

	template<DeviceType device_t>
	inline IFile<device_t> FileSystem::open_sync(IFileDevice<device_t>* device, const Path& path, OpenMode mode)
	{
		auto file = device->create_file(_allocator);
		file.open(path, mode);
		return file;
	}

	template<DeviceType device_t>
	inline IFile<device_t> FileSystem::open_async(IFileDevice<device_t>* device, const Path& path, OpenMode mode)
	{
		auto file = device->create_file(_allocator);
		file.open(path, mode);
		return file;
	}
}