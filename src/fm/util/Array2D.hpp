/*
Array2D.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_ARRAY2D_HPP_INCLUDED
#define HEADER_ZN_ARRAY2D_HPP_INCLUDED

#include <cstring> // For memcpy and memset
#include <iostream>

#include "Vector2.hpp"
#include "Exception.hpp"

namespace zn
{

/*
	Bi-dimensionnal flat array.
	Static because it is not resizeable,
	dynamic because it can be re-created without destroy its instance.
*/

template <typename T>
class Array2D
{
public :

	// creates an empty array
	Array2D()
		: m_data(nullptr),
		m_sizeX(0),
		m_sizeY(0)
	{}

	// creates an array with the specified size.
	// Note : data values are not initialized, use the fill() function if necessary.
	Array2D(u32 psizeX, u32 psizeY)
	{
		create(psizeX, psizeY);
	}

	// creates an array with the specified size and value
	Array2D(u32 psizeX, u32 psizeY, const T & value)
	{
		create(psizeX, psizeY, value);
	}

	// creates an array as copy from another
	Array2D(const Array2D & other)
	{
		copyFrom(other);
	}

	virtual ~Array2D()
	{
		clear();
	}

	Array2D<T> & operator=(const Array2D<T> & other)
	{
		copyFrom(other);
		return *this;
	}

	inline T * raw()
	{
		return m_data;
	}

	inline T & operator[](u32 i)
	{
		return m_data[i];
	}

	inline u32 index(u32 x, u32 y) const
	{
		return m_sizeX * y + x;
	}

	// creates the buffer from the specified area.
	// old data is cleared.
	// Note : data values are not initialized, use the fill() function if necessary.
	void create(u32 psizeX, u32 psizeY)
	{
		// Delete old data
		if(m_data != nullptr)
			delete[] m_data;

		// Check if the given area is correct
		if(psizeX == 0 || psizeY == 0)
		{
			std::cout << "WARNING: Array2D::create received empty area ("
					<< psizeX << ", " << psizeY << ", " << ")" << std::endl;
			return;
		}

		// Allocate memory
		m_sizeX = psizeX;
		m_sizeY = psizeY;
		m_data = new T[area()];
	}

	// Copies data from another array into this one.
	// Copy an empty array will clear this one.
	void copyFrom(const Array2D<T> & other)
	{
		if(other.empty())
			clear();
		else
		{
			create(other.sizeX(), other.sizeY());
			memcpy(m_data, other.m_data, area() * sizeof(T));
		}
	}

	// Returns the count of cells in the array
	inline u32 area() const
	{
		return m_sizeX * m_sizeY;
	}

	inline u32 sizeX() const { return m_sizeX; }
	inline u32 sizeY() const { return m_sizeY; }

	// clears the container
	void clear()
	{
		if(m_data != nullptr)
		{
			delete[] m_data;
			m_data = nullptr;
			m_sizeX = 0;
			m_sizeY = 0;
		}
	}

	// Tests if there is space allocated in the array
	inline bool empty() const
	{
		return m_data == nullptr;
	}

	// fills the buffer with the same value
	void fill(const T & val)
	{
		// TODO optimize for byte-size types (memset)
		const u32 vol = area();
		for(u32 i = 0; i < vol; ++i)
			m_data[i] = val;
	}

	// get an element without position validation (it must be valid !)
	inline T getNoEx(s32 x, s32 y) const
	{
		return m_data[index(x, y)];
	}

	// get an element
	T get(s32 x, s32 y) const throw(Exception)
	{
		if(x < 0 || y < 0 || x >= m_sizeX || y >= m_sizeY)
			throw Exception("Array2D::get " + Vector2i(x,y));
		else
			return getNoEx(x, y);
	}

	// set an element without position validation (it must be valid !)
	inline void setNoEx(s32 x, s32 y, const T & value)
	{
		m_data[index(x, y)] = value;
	}

	// Set an element
	void set(s32 x, s32 y, const T & value) throw(Exception)
	{
		if(x < 0 || y < 0 || x >= static_cast<s32>(m_sizeX) || y >= static_cast<s32>(m_sizeY))
			throw Exception("Array2D::set " + Vector2i(x,y));
		else
			return setNoEx(x, y, value);
	}

	//
	// Convenience methods
	//

	// same as create(Area3D), with an additionnal parameter for default values
	// (Convenience)
	void create(u32 psizeX, u32 psizeY, const T & defaultValue)
	{
		create(psizeX, psizeY);
		fill(defaultValue);
	}

	// Returns the data size of an element in bytes
	// (convenience)
	inline u32 elementByteCount() const
	{
		return sizeof(T);
	}

	// Returns the size of the data contained in the array in bytes
	// (convenience)
	inline u32 byteCount() const
	{
		return area() * elementByteCount();
	}

#ifdef HEADER_ZN_VECTOR2_HPP_INCLUDED

	// set an element
	// (convenience)
	inline void set(const Vector2i & pos, const T & value) throw(Exception)
	{
		set(pos.x, pos.y, value);
	}

	// set an element without position validation (it must be valid !)
	// (convenience)
	inline void setNoEx(const Vector2i & pos, const T & value)
	{
		m_data[index(pos.x, pos.y)] = value;
	}

	// get an element
	// (convenience)
	inline T get(const Vector2i & pos) const throw(Exception)
	{
		get(pos.x, pos.y);
	}

	// get an element without position validation (it must be valid !)
	// (convenience)
	inline T getNoEx(const Vector2i & pos) const
	{
		return m_data[index(pos.x, pos.y)];
	}

#endif

private :

	T * m_data;	// linear data storage (nullptr if empty)
	u32 m_sizeX;
	u32 m_sizeY;

};

} // namespace zn

#endif // HEADER_ZN_ARRAY2D_HPP_INCLUDED



