#include<core/Blob.hpp>
#include<memory/IAllocator.hpp>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<cassert>


namespace tutorial
{

	template <typename T, int count> int lengthOf(const T(&)[count])
	{
		return count;
	};

	OutputBlob::OutputBlob(IAllocator& allocator)
		: m_allocator(&allocator)
		, m_data(nullptr)
		, m_size(0)
		, m_pos(0)
	{}


	OutputBlob::OutputBlob(void* data, int size)
		: m_data(data)
		, m_size(size)
		, m_pos(0)
	{
	}


	OutputBlob::OutputBlob(const OutputBlob& blob, IAllocator& allocator)
		: m_allocator(&allocator)
		, m_pos(blob.m_pos)
	{
		if (blob.m_size > 0)
		{
			m_data = allocator.allocate(blob.m_size);
			memcpy(m_data, blob.m_data, blob.m_size);
			m_size = blob.m_size;
		}
		else
		{
			m_data = nullptr;
			m_size = 0;
		}
	}


	OutputBlob::OutputBlob(const InputBlob& blob, IAllocator& allocator)
		: m_allocator(&allocator)
		, m_pos(blob.getSize())
	{
		if (blob.getSize() > 0)
		{
			m_data = allocator.allocate(blob.getSize());
			memcpy(m_data, blob.getData(), blob.getSize());
			m_size = blob.getSize();
		}
		else
		{
			m_data = nullptr;
			m_size = 0;
		}
	}


	OutputBlob::~OutputBlob()
	{
		if (m_allocator) m_allocator->deallocate(m_data);
	}


	OutputBlob& OutputBlob::operator << (const char* str)
	{
		write(str, strlen(str));
		return *this;
	}


	OutputBlob& OutputBlob::operator << (int value)
	{
		char tmp[20];
		//itoa(value, tmp, lengthOf(tmp));
		sprintf_s(tmp, "%d", value);
		write(tmp, strlen(tmp));
		return *this;
	}


	OutputBlob& OutputBlob::operator << (uint64 value)
	{
		char tmp[40];
		//itoa(value, tmp, lengthOf(tmp));
		sprintf_s(tmp, "%llu", value);
		write(tmp, strlen(tmp));
		return *this;
	}


	OutputBlob& OutputBlob::operator << (int64 value)
	{
		char tmp[40];
		//itoa(value, tmp, lengthOf(tmp));
		sprintf_s(tmp, "%lld", value);
		write(tmp, strlen(tmp));
		return *this;
	}


	OutputBlob& OutputBlob::operator << (uint32 value)
	{
		char tmp[20];
		//itoa(value, tmp, lengthOf(tmp));
		sprintf_s(tmp, "%u", value);
		write(tmp, strlen(tmp));
		return *this;
	}


	OutputBlob& OutputBlob::operator << (float value)
	{
		char tmp[30];
		//itoa(value, tmp, lengthOf(tmp), 6);
		sprintf_s(tmp, "%.6f", value);
		write(tmp, strlen(tmp));
		return *this;
	}


	OutputBlob::OutputBlob(const OutputBlob& rhs)
	{
		assert(rhs.m_allocator);
		m_allocator = rhs.m_allocator;
		m_pos = rhs.m_pos;
		if (rhs.m_size > 0)
		{
			m_data = m_allocator->allocate(rhs.m_size);
			memcpy(m_data, rhs.m_data, rhs.m_size);
			m_size = rhs.m_size;
		}
		else
		{
			m_data = nullptr;
			m_size = 0;
		}
	}


	void OutputBlob::operator =(const OutputBlob& rhs)
	{
		assert(rhs.m_allocator);
		if (m_allocator) m_allocator->deallocate(m_data);

		//_allocator = rhs._allocator;
		m_pos = rhs.m_pos;
		if (rhs.m_size > 0)
		{
			m_data = m_allocator->allocate(rhs.m_size);
			memcpy(m_data, rhs.m_data, rhs.m_size);
			m_size = rhs.m_size;
		}
		else
		{
			m_data = nullptr;
			m_size = 0;
		}
	}


	void OutputBlob::write(const void* data, int size)
	{
		if (!size) return;

		if (m_pos + size > m_size)
		{
			reserve((m_pos + size) << 1);
		}
		memcpy((uint8*)m_data + m_pos, data, size);
		m_pos += size;
	}


	void OutputBlob::writeString(const char* string)
	{
		if (string)
		{
			int size = strlen(string) + 1;
			write(size);
			write(string, size);
		}
		else
		{
			write((int)0);
		}
	}


	void OutputBlob::clear()
	{
		m_pos = 0;
	}


	void OutputBlob::reserve(int size)
	{
		if (size <= m_size) return;

		//assert(_allocator);
		uint8* tmp = (uint8*)m_allocator->allocate(size);
		memcpy(tmp, m_data, m_size);
		m_allocator->deallocate(m_data);
		m_data = tmp;
		m_size = size;
	}


	void OutputBlob::resize(int size)
	{
		m_pos = size;
		if (size <= m_size) return;

		//assert(_allocator);
		uint8* tmp = (uint8*)m_allocator->allocate(size);
		memcpy(tmp, m_data, m_size);
		m_allocator->deallocate(m_data);
		m_data = tmp;
		m_size = size;
	}



	InputBlob::InputBlob(const void* data, int size)
		: m_data((const uint8*)data)
		, m_size(size)
		, m_pos(0)
	{}


	InputBlob::InputBlob(const OutputBlob& blob)
		: m_data((const uint8*)blob.getData())
		, m_size(blob.getPos())
		, m_pos(0)
	{}


	const void* InputBlob::skip(int size)
	{
		auto* pos = m_data + m_pos;
		m_pos += size;
		if (m_pos > m_size)
		{
			m_pos = m_size;
		}

		return (const void*)pos;
	}


	bool InputBlob::read(void* data, int size)
	{
		if (m_pos + (int)size > m_size)
		{
			for (int i = 0; i < size; ++i)
				((unsigned char*)data)[i] = 0;
			return false;
		}
		if (size)
		{
			memcpy(data, ((char*)m_data) + m_pos, size);
		}
		m_pos += size;
		return true;
	}


	bool InputBlob::readString(char* data, int max_size)
	{
		int size;
		read(size);
		assert(size <= max_size);
		return read(data, size < max_size ? size : max_size);
	}


}
