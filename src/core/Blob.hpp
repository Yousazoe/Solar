#pragma once
#include<core/Types.hpp>

namespace tutorial
{

	class InputBlob;
	class IAllocator;

	class OutputBlob
	{
	public:
		explicit OutputBlob(IAllocator& allocator);
		OutputBlob(void* data, int size);
		OutputBlob(const OutputBlob& rhs);
		OutputBlob(const OutputBlob& blob, IAllocator& allocator);
		OutputBlob(const InputBlob& blob, IAllocator& allocator);
		void operator =(const OutputBlob& rhs);
		~OutputBlob();

		void resize(int size);
		void reserve(int size);
		const void* getData() const { return m_data; }
		void* getMutableData() { return m_data; }
		int getPos() const { return m_pos; }
		void write(const void* data, int size);
		void writeString(const char* string);
		template <class T> inline void write(const T& value);
		void clear();

		OutputBlob& operator << (const char* str);
		OutputBlob& operator << (uint64 value);
		OutputBlob& operator << (int64 value);
		OutputBlob& operator << (int value);
		OutputBlob& operator << (uint32 value);
		OutputBlob& operator << (float value);

	private:
		void* m_data;
		int m_size;
		int m_pos;
		IAllocator* m_allocator;
	};

	template <class T> inline void OutputBlob::write(const T& value)
	{
		write(&value, sizeof(T));
	}

	template <> inline void OutputBlob::write<bool>(const bool& value)
	{
		uint8 v = value;
		write(&v, sizeof(v));
	}

	class InputBlob
	{
	public:
		InputBlob(const void* data, int size);
		explicit InputBlob(const OutputBlob& blob);

		bool read(void* data, int size);
		bool readString(char* data, int max_size);
		template <class T> void read(T& value) { read(&value, sizeof(T)); }
		template <class T> inline T read();
		const void* skip(int size);
		const void* getData() const { return (const void*)m_data; }
		int getSize() const { return m_size; }
		int getPosition() { return m_pos; }
		void setPosition(int pos) { m_pos = pos; }
		void rewind() { m_pos = 0; }
		uint8 readChar() { ++m_pos; return m_data[m_pos - 1]; }

	private:
		const uint8* m_data;
		int m_size;
		int m_pos;
	};

	template <class T> inline T InputBlob::read()
	{
		T v;
		read(&v, sizeof(v));
		return v;
	}

	template <> inline bool InputBlob::read<bool>()
	{
		uint8 v;
		read(&v, sizeof(v));
		return v != 0;
	}
}
