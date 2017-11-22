
#include "csg_node_factory.h"

#include "csg_node.h"

std::unique_ptr<CSGNode> CSGNodeFactory::create(const std::string typeName, const json& json) const
{
	auto t = m_creators.find(typeName);
	if (t == m_creators.end())
		throw NodeTypeNotFound(typeName);

	return t->second(json, *this);
}
