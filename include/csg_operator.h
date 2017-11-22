#ifndef _H_CSG_OPERATOR_
#define _H_CSG_OPERATOR_

#include "csg_node.h"

enum class CSGOperatorType
{
	INTERSECTION = 0,
	UNION,
	DIFFERENCE
};

class CSGOperator : public CSGNode 
{
public:
	CSGOperator(const json& json, const CSGNodeFactory& factory);
	CSGOperator(CSGOperatorType type);

	virtual vtkSmartPointer<vtkImplicitFunction> createImplicitFunction() const override;
private:
	CSGOperatorType m_type;	
};

#endif 

