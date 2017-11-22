#ifndef _H_SERIALIZABLE_
#define _H_SERIALIZABLE_

#include "json.hpp"

#include <vector>

#include "vtkImplicitFunction.h"
#include "vtkSmartPointer.h"

using json = nlohmann::json;

class ISerializable
{
public: 
	ISerializable() {}
	ISerializable(const json& json) {}
};

#endif 

