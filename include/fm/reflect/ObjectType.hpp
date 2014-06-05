#ifndef HEADER_ZN_OBJECTTYPE_HPP_INCLUDED
#define HEADER_ZN_OBJECTTYPE_HPP_INCLUDED

#include <string>
#include <sstream>
#include <fm/types.hpp>

namespace zn
{

typedef u16 ObjectTypeID;


/// \brief You can derive this class to store additional information when registering an ObjectType.
/// \see ObjectType, ObjectTypeDatabase
class IObjectTypeUserData
{
private:
	IObjectTypeUserData();
public:
	virtual ~IObjectTypeUserData();
};


/// \brief Meta-class of an object.
struct ObjectType
{
	/// \brief Unique numeric ID of the component.
	/// DO NOT modify (only done on type registration).
	/// The value of IDs may be generated sequentially, starting from 1.
	/// 0 means null type.
	ObjectTypeID ID;

	/// \brief Unique name of the type.
	std::string name;

	/// \brief Name of the base class.
	std::string baseName;

	/// \brief you can store additional type information here when you register a type with ObjectTypeDatabase.
	/// It will be destroyed automatically when the singleton lifetime ends,
	/// unless you destroy it yourself and set it to null before.
	IObjectTypeUserData * userData;

	ObjectType(
		const std::string & p_name,
		const std::string & p_baseName
	) :
		ID(0), // null ID, until the type gets registered
		name(p_name),
		baseName(p_baseName),
		userData(nullptr)
	{}

	~ObjectType()
	{
		if(userData)
			delete userData;
	}

	void print(std::ostream & os) const
	{
		os << "{[" << ID << "]" << name << " : " << baseName << "}";
	}

	std::string toString() const
	{
		std::stringstream ss;
		print(ss);
		return ss.str();
	}

	bool is(const ObjectType & other) const;

};

} // namespace zn

#endif // HEADER_ZN_OBJECTTYPE_HPP_INCLUDED

