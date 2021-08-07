#pragma once
#include<fs/IFile.hpp>
#include<fstream>

namespace tutorial::fs
{
	template<>
	class IFile<DeviceType::Disk> 
	{
	public:
		IFile() = default;
		~IFile() {
			close();
		}
		IFile(const IFile&) = delete;
		void operator=(const IFile&) = delete;
		IFile(IFile&&) = default;
		void operator=(IFile&& rhs) {
			_mode = rhs._mode;
			_file = std::move(rhs._file);
			_position = rhs._position;
			_size = rhs._size;
		}

	public:
		bool open(const Path& path, OpenMode mode);
		void close();
		bool read(void* buffer, size_t size);
		bool write(const void* buffer, size_t size);
		bool seek(SeekDir base, size_t pos);
		size_t pos() const;
		size_t size() const;

	private:
		OpenMode _mode;
		std::fstream _file;
		size_t _position = 0u;
		size_t _size = 0u;
	};

	inline bool IFile<DeviceType::Disk>::open(const Path& path, OpenMode mode)
	{
		constexpr std::ios_base::openmode open_mode[] =
		{
			std::ios::in,
			std::ios::out,
		};
		auto mode_impl = open_mode[(size_t)mode];
		_file.open(path, mode_impl | std::ios::binary);
		_position = 0u;
		_size = 0u;
		_mode = mode;

		if (_mode == OpenMode::READ)
		{
			_file.seekg(0, std::ios::end);
			_size = (size_t)_file.tellg();
			_file.seekg(0, std::ios::beg);
		}

		return _file.is_open();
	}

	inline void IFile<DeviceType::Disk>::close()
	{
		_file.close();
		_position = 0u;
		_size = 0u;
	}

	inline bool IFile<DeviceType::Disk>::read(void* buffer, size_t size)
	{
		_file.read((char*)buffer, size);
		_position += size;
		return true;
	}

	inline bool IFile<DeviceType::Disk>::write(const void* buffer, size_t size)
	{
		_file.write((char*)buffer, size);
		_position += size;
		_size += size;
		return true;
	}

	inline bool IFile<DeviceType::Disk>::seek(SeekDir dir, size_t pos)
	{
		if (_mode == OpenMode::READ && pos > _size)
			pos = _size;

		constexpr std::ios_base::seekdir dirs[] = 
		{
			std::ios_base::beg,
			std::ios_base::end,
			std::ios_base::cur,
		};
		auto dir_impl = dirs[(int)dir];
		_position = pos;

		if (_mode == OpenMode::READ)
			_file.seekg(_position, dir_impl);
		else if (_mode == OpenMode::WRITE)
			_file.seekp(_position, dir_impl);

		return true;
	}

	inline size_t IFile<DeviceType::Disk>::pos() const
	{
		return _position;
	}

	inline size_t IFile<DeviceType::Disk>::size() const
	{
		return _size;
	}

}