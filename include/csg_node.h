#ifndef _H_CSG_NODE_
#define _H_CSG_NODE_

#include "serializable.h"

#include <vector>

#include "vtkImplicitFunction.h"
#include "vtkSmartPointer.h"

class CSGNodeFactory;

class CSGNode : public ISerializable
{
public: 
	CSGNode() {}
	CSGNode(const json& json, const CSGNodeFactory& factory) {}
	virtual vtkSmartPointer<vtkImplicitFunction> createImplicitFunction() const = 0;
protected: 
	std::vector<std::unique_ptr<CSGNode>> m_childs;
};


#endif 

