#ifndef _H_CSG_NODE_FACTORY_
#define _H_CSG_NODE_FACTORY_

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <stdexcept>

#include "serializable.h"

class CSGNode;

struct NodeTypeNotFound : public std::runtime_error
{
	NodeTypeNotFound(const std::string& type) :
		std::runtime_error("Node type '" + type + "' does not exist.")
	{
	}
};

class CSGNodeFactory
{
public: 
	
	template<typename T>
	void registerType(const std::string& typeName)
	{
		m_creators[typeName] = [](const json& json, const CSGNodeFactory& fac)
		{
			return std::make_unique<T>(json, fac);
		};
	}

	std::unique_ptr<CSGNode> create(const std::string typeName, const json& json) const;

private:
	std::unordered_map<std::string, 
		std::function<std::unique_ptr<CSGNode>(const json& json, const CSGNodeFactory& fac)>> m_creators;
};


#endif 

