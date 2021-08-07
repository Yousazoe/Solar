#pragma once
#include<../thirdparty/tinyxml/tinyxml.h>
#include<memory>
#include<fstream>

namespace tutorial
{

	class XMLAttribute
	{
	public:
		XMLAttribute() : _attrib(nullptr) {}
		XMLAttribute(TiXmlAttribute *attrib) : _attrib(attrib) {}

		bool is_empty() const { return _attrib == nullptr; }
		const char* name() const { return _attrib->Name(); }
		const char* value() const { return _attrib->Value(); }

		XMLAttribute next_attrib() { return XMLAttribute(_attrib->Next()); }

	protected:
		TiXmlAttribute* _attrib;
	};


	class XMLNode
	{
	public:
		XMLNode() = default;
		XMLNode(TiXmlElement* node) : _node(node) {}

	public:
		TiXmlElement* get_xml_node() { return _node; }
		bool is_empty() const { return _node == nullptr; }
		const char* name() const { return _node->Value(); }
		XMLNode first_child() const { return XMLNode(_node->FirstChildElement()); }
		XMLNode next_sibling() const { return XMLNode(_node->NextSiblingElement()); }
		XMLNode first_child(const char* name) const { return XMLNode(_node->FirstChildElement(name)); }
		XMLNode next_sibling(const char* name) const { return XMLNode(_node->NextSiblingElement(name)); }
		XMLAttribute first_attrib() { return XMLAttribute(_node->FirstAttribute()); }
		const char* attribute(const char* name, const char* defValue = "") const
		{
			const char *attrib = _node->Attribute(name);
			return attrib != nullptr ? attrib : defValue;
		}
		void attribute(const char* name, float* value, const float& default) const
		{
			double v = default;
			_node->Attribute(name, &v);
			*value = (float)v;
		}
		void attribute(const char* name, int* value, const int& default) const
		{
			_node->Attribute(name, value);
		}


		size_t child_node_count(const char *name = nullptr) const
		{
			size_t numNodes = 0u;
			TiXmlElement *node1 = _node->FirstChildElement(name);
			while (node1)
			{
				++numNodes;
				node1 = node1->NextSiblingElement(name);
			}
			return numNodes;
		}
		operator bool() const { return !is_empty(); }

	protected:
		TiXmlElement* _node = nullptr;
	};

	class XMLDoc
	{
	public:
		XMLDoc() { }
		~XMLDoc() {
			_doc.Clear();
		}

	public:
		bool has_error() const;
		XMLNode get_root_node();
		void parse_string(const char* text);
		void parse_buffer(const char* charbuf, size_t size);
		bool parse_file(const char* file_name);

	private:
		TiXmlDocument	_doc;
		std::unique_ptr<char[]> buf;
	};

	inline bool XMLDoc::has_error() const 
	{ 
		return _doc.RootElement() == nullptr; 
	}

	inline XMLNode XMLDoc::get_root_node()
	{
		return XMLNode(_doc.RootElement());
	}

	inline void XMLDoc::parse_string(const char* text)
	{
		_doc.Parse(text);
	}

	inline void XMLDoc::parse_buffer(const char* charbuf, size_t size)
	{
		buf.reset(new char[size + 1]);
		memcpy(buf.get(), charbuf, size);
		buf[size] = '\0';
		parse_string(buf.get());
	}

	inline bool XMLDoc::parse_file(const char* file_name)
	{
		std::fstream f(file_name, std::ios::in | std::ios::binary);
		if (!f.is_open())
			return false;

		f.seekg(0, std::ios::end);
		auto size = (size_t)f.tellg();
		f.seekg(0, std::ios::beg);

		buf.reset(new char[size + 1]);
		f.read(buf.get(), size);
		buf[size] = '\0';

		f.close();

		parse_string(buf.get());
		return true;
	}

}
