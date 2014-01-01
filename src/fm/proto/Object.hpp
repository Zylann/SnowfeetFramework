/*
Object.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_OBJECT_HPP_INCLUDED
#define HEADER_ZN_OBJECT_HPP_INCLUDED

#include "../types.hpp"
#include "ComponentType.hpp"

namespace zn
{

class Object
{
public:

	virtual ~Object() {}

	// TODO put serialization and meta-class stuff here?
//	virtual const std::string & className()
//	{
//		static std::string __name = "Object";
//		return __name;
//	}

private:

};

} // namespace zn

#endif // HEADER_ZN_OBJECT_HPP_INCLUDED

