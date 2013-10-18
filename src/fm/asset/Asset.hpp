/*
Asset.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_ASSET_HPP_INCLUDED
#define HEADER_ZN_ASSET_HPP_INCLUDED

namespace zn
{

// Something needed by the application, that can be loaded from a file.
// Note that you don't necessarily need to implement this class, as asset
// management is mainly written using template magic.
// However, you must implement the same functions defined in it.
class IAsset
{
public:

	virtual ~IAsset() {}

	virtual bool loadFromFile(const std::string & filePath) = 0;
	//virtual std::list<std::string> getDependencies() { return std::list<std::string>(); }
};

} // namespace zn

#endif // HEADER_ZN_ASSET_HPP_INCLUDED

