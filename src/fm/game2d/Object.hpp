/*
Object.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_OBJECT_HPP_INCLUDED
#define HEADER_ZN_OBJECT_HPP_INCLUDED

#include "../types.hpp"
#include "components/ComponentType.hpp"

namespace zn
{

class Object
{
public:

	virtual ~Object() {}

	inline const std::string & name() const { return m_name; }
	inline void setName(const std::string & newName) { m_name = newName; }

private:

	std::string m_name;

};

} // namespace zn

#endif // HEADER_ZN_OBJECT_HPP_INCLUDED

