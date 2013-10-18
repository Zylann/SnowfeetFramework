/*
LayerMap.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_LAYERMAP_HPP_INCLUDED
#define HEADER_ZN_LAYERMAP_HPP_INCLUDED

#include <iostream>
#include <unordered_map>

namespace zn
{

// A set of 32 named layers.
class LayerMap
{
public:

	static const u32 COUNT = 32;

	inline void setLayer(u8 index, std::string name)
	{
		m_layers[index] = name;
	}

	inline void unsetLayer(u8 index)
	{
		m_layers[index].clear();
	}

	inline bool isSet(u8 index) const
	{
		return !m_layers[index].empty();
	}

	inline u32 count() const
	{
		return COUNT;
	}

	u32 maskFromName(const std::string & name) const
	{
		return 1 << indexFromName(name);
	}

	u8 indexFromName(const std::string & name) const
	{
		for(u32 i = 0; i < COUNT; ++i)
		{
			if(m_layers[i] == name)
				return i;
		}
#ifdef ZN_DEBUG
		std::cout << "E: LayerMap::indexFromName: undefined layer " << name << std::endl;
#endif
		return 0;
	}

	std::string nameFromIndex(u8 index) const
	{
		return m_layers[index];
	}

	void print(std::ostream & os) const
	{
		for(u32 i = 0; i < COUNT; ++i)
		{
			std::cout << "[" << i << "]: " << m_layers[i] << std::endl;
		}
	}

private:

	std::string m_layers[32];

};

} // namespace zn

#endif // HEADER_ZN_LAYERMAP_HPP_INCLUDED

