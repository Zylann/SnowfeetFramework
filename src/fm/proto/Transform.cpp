#include "../util/math.hpp"
#include "Transform.hpp"
#include "Entity.hpp"
#include "Scene.hpp"

namespace zn
{

Transform::Transform(Entity& e) :
	r_entity(e),
	r_parent(nullptr),
	m_position(0,0),
	m_scale(1,1),
	m_rotation(0),
	m_localMatrixNeedUpdate(true),
	m_globalMatrixNeedUpdate(true)
{
}

//------------------------------------------------------------------------------
void Transform::notifyChildrenForParentChange()
{
	for(auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->m_globalMatrixNeedUpdate = true;
	}
}

//------------------------------------------------------------------------------
sf::Vector2f Transform::localPosition() const
{
	return m_position;
}

//------------------------------------------------------------------------------
sf::Vector2f Transform::position() const
{
	if(r_parent != nullptr)
	{
		return r_parent->matrix().transformPoint(m_position);
	}
	else
	{
		return m_position;
	}
}

//------------------------------------------------------------------------------
sf::Vector2f Transform::localScale() const
{
	return m_scale;
}

//------------------------------------------------------------------------------
sf::Vector2f Transform::scale() const
{
	if(r_parent != nullptr)
	{
		sf::Vector2f parentScale = r_parent->scale();
		return sf::Vector2f(
			parentScale.x * m_scale.x,
			parentScale.y * m_scale.y
		);
	}
	else
	{
		return m_scale;
	}
}

//------------------------------------------------------------------------------
f32 Transform::localRotation() const
{
	return m_rotation;
}

//------------------------------------------------------------------------------
f32 Transform::rotation() const
{
	if(r_parent != nullptr)
	{
		return m_rotation + r_parent->rotation();
	}
	else
	{
		return m_rotation;
	}
}

//------------------------------------------------------------------------------
void Transform::setLocalPosition(const sf::Vector2f & newPosition)
{
	m_position = newPosition;

	m_localMatrixNeedUpdate = true;
	m_globalMatrixNeedUpdate = true;
	notifyChildrenForParentChange();
}

//------------------------------------------------------------------------------
void Transform::setPosition(sf::Vector2f newPosition)
{
	// TODO cache inverted matrices?
	if(r_parent != nullptr)
	{
		newPosition = r_parent->matrix().getInverse().transformPoint(newPosition);
	}

	setLocalPosition(newPosition);
}

//------------------------------------------------------------------------------
void Transform::setLocalScale(const sf::Vector2f& newScale)
{
	m_scale = newScale;

	m_localMatrixNeedUpdate = true;
	m_globalMatrixNeedUpdate = true;
	notifyChildrenForParentChange();
}

//------------------------------------------------------------------------------
void Transform::setScale(sf::Vector2f newScale)
{
	if(r_parent != nullptr)
	{
		sf::Vector2f parentScale = r_parent->scale();
		newScale.x *= parentScale.x;
		newScale.y *= parentScale.y;
	}

	setLocalScale(newScale);
}

//------------------------------------------------------------------------------
void Transform::setLocalRotation(f32 newRotation)
{
	m_rotation = newRotation;

	m_localMatrixNeedUpdate = true;
	m_globalMatrixNeedUpdate = true;
	notifyChildrenForParentChange();
}

//------------------------------------------------------------------------------
void Transform::setRotation(f32 newRotation)
{
	if(r_parent != nullptr)
	{
		f32 parentRotation = r_parent->rotation();
		newRotation -= parentRotation;
	}

	setLocalRotation(newRotation);
}

//------------------------------------------------------------------------------
void Transform::translate(sf::Vector2f motion)
{
	setLocalPosition(m_position + motion);
}

//------------------------------------------------------------------------------
void Transform::rotate(f32 degrees)
{
	setLocalRotation(m_rotation + degrees);
}

//------------------------------------------------------------------------------
void Transform::scale(sf::Vector2f scaleFactor)
{
	setLocalScale(sf::Vector2f(
		m_scale.x * scaleFactor.x,
		m_scale.y * scaleFactor.y
	));
}

//------------------------------------------------------------------------------
const sf::Transform & Transform::localMatrix() const
{
	if(m_localMatrixNeedUpdate)
	{
		// Update local matrix
		f32 angle  = -m_rotation * math::DEG2RAD;
		f32 cosine = static_cast<f32>(cos(angle));
		f32 sine   = static_cast<f32>(sin(angle));
		f32 sxc    = m_scale.x * cosine;
		f32 syc    = m_scale.y * cosine;
		f32 sxs    = m_scale.x * sine;
		f32 sys    = m_scale.y * sine;
		f32 tx     = m_position.x;
		f32 ty     = m_position.y;
//		f32 tx     = -m_position.x * sxc - m_position.y * sys;
//		f32 ty     =  m_position.x * sxs - m_position.y * syc;
//		f32 tx     = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
//		f32 ty     =  m_origin.x * sxs - m_origin.y * syc + m_position.y;

//		m_localMatrix = sf::Transform::Identity;
//		m_localMatrix.translate(m_position);
//		m_localMatrix.rotate(m_rotation);
//		m_localMatrix.scale(m_scale);

		m_localMatrix = sf::Transform(
			sxc, sys, tx,
			-sxs, syc, ty,
			0.f, 0.f, 1.f
		);

		m_localMatrixNeedUpdate = false;
	}

	return m_localMatrix;
}

//------------------------------------------------------------------------------
const sf::Transform & Transform::matrix() const
{
	if(m_globalMatrixNeedUpdate)
	{
		// Update global matrix
		if(r_parent != nullptr)
		{
			m_globalMatrix = r_parent->matrix();
			m_globalMatrix.combine(localMatrix());
		}
		else
		{
			m_globalMatrix = localMatrix();
		}

		m_globalMatrixNeedUpdate = false;
	}

	return m_globalMatrix;
}

//------------------------------------------------------------------------------
Transform * Transform::parent() const
{
	return r_parent;
}

//------------------------------------------------------------------------------
void Transform::setParent(Transform * parent)
{
	// TODO add "auto transform" parameter to keep the same visual position

	if(parent == r_parent)
		return;

	if(r_parent != nullptr)
	{
#ifdef ZN_DEBUG
		std::cout << "D: Unparenting \"" << entity().name() << "\" from \"" << r_parent->entity().name() << '"' << std::endl;
#endif
		r_parent->onRemoveChild(this);
	}

	r_parent = parent;

	if(r_parent != nullptr)
	{
#ifdef ZN_DEBUG
		std::cout << "D: Parenting \"" << entity().name() << "\" to \"" << r_parent->entity().name() << '"' << std::endl;
#endif
		r_parent->onAddChild(this);
	}

	m_globalMatrixNeedUpdate = true;
	notifyChildrenForParentChange();
}

//------------------------------------------------------------------------------
Transform * Transform::child(u32 index) const
{
#ifdef ZN_DEBUG
	if(index >= m_children.size())
	{
		std::cout << "E: Transform::child: child index is out of bounds "
			"(index=" << index << ", "
			"size=" << m_children.size() << ", "
			"entityName=\"" << entity().name() << "\""
			")" << std::endl;
		return nullptr;
	}
#endif
	return m_children[index];
}

//------------------------------------------------------------------------------
void Transform::serialize(JsonBox::Value& o)
{
	zn::serialize(o["position"], m_position);
	zn::serialize(o["scale"], m_scale);
	o["rotation"] = m_rotation;

	JsonBox::Value & parentData = o["parent"];
	if(r_parent != nullptr)
	{
		parentData = (s32)(r_parent->entity().id());
	}
	else
	{
		parentData.setNull();
	}
}

//------------------------------------------------------------------------------
void Transform::unserialize(JsonBox::Value& o)
{
	zn::unserialize(o["position"], m_position);
	zn::unserialize(o["scale"], m_scale);
	m_rotation = o["rotation"].getDouble();

	m_localMatrixNeedUpdate = true;
	m_globalMatrixNeedUpdate = true;
}

//------------------------------------------------------------------------------
void Transform::postUnserialize(JsonBox::Value & o)
{
	JsonBox::Value & parentData = o["parent"];
	if(!parentData.isNull())
	{
		u32 parentID = parentData.getInt();
		Entity * parent = entity().scene().findEntityFromID(parentID);

		if(parent == nullptr)
		{
			std::cout << "E: Transform::postUnserialize: "
				"parentID [" << parentID << "] not found in scene "
				"(entity: \"" << entity().name() << "\")" << std::endl;
		}
		else
		{
			r_parent = &(parent->transform);
			r_parent->onAddChild(this);
		}
	}
}

//------------------------------------------------------------------------------
void Transform::onAddChild(Transform* child)
{
	m_children.push_back(child);
}

//------------------------------------------------------------------------------
void Transform::onRemoveChild(Transform * child)
{
	for(auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		if(*it == child)
		{
			m_children.erase(it);
			return;
		}
	}
}

} // namespace zn


