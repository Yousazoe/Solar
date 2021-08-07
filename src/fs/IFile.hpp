#pragma once
#include<fs/Define.h>
#include<fs/Path.hpp>

namespace tutorial::fs
{
	// only support binary now, no text format
	enum class OpenMode
	{
		READ = 0,
		WRITE,
		READ_WRITE,
	};

	enum class SeekDir
	{
		BEGIN = 0,
		END,
		CURRENT,
	};

	template<DeviceType type>
	class IFile final
	{
	public:
		IFile() {}
		~IFile() {}
		IFile(const IFile&) = delete;
		void operator=(const IFile&) = delete;
		IFile(IFile&&);
		void operator=(IFile&&);

	public:
		bool open(const Path& path, OpenMode mode);
		void close();
		bool read(void* buffer, size_t size);
		bool write(const void* buffer, size_t size);
		bool seek(SeekDir base, size_t pos);
		size_t pos() const;
		size_t size() const;
	};
}