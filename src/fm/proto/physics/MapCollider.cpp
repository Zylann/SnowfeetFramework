#include <iostream>

#include "../../util/math.hpp"
#include "../../sfml/sfml2_utils.hpp"
#include "../Entity.hpp"
#include "MapCollider.hpp"
#include "../../json/json_utils.hpp"
#include "../../asset/AssetBank.hpp"

#ifdef ZN_DEBUG
	#include <map> // for debug
#endif

namespace zn
{

MapCollider::MapCollider() : ACollider(),
	m_cellSize(1), // Default
	m_outerColliderType(0), // Default
	r_tiledMap(nullptr)
{
	m_colliderTypes.push_back(sf::FloatRect(0,0,1,1)); // The empty collider
	m_colliderTypes.push_back(sf::FloatRect(0,0,1,1)); // Block default collider
}

//------------------------------------------------------------------------------
sf::FloatRect MapCollider::bounds() const
{
	return sf::FloatRect(0, 0, m_size.x*m_cellSize, m_size.y*m_cellSize);
}

//------------------------------------------------------------------------------
void MapCollider::build(const sf::Vector2i & size, u8 fillColliderType)
{
	if(size.x <= 0 || size.x <= 0)
	{
		std::cout << "E: MapCollider::build: invalid size (" << size.x << ", " << size.y << ")" << std::endl;
		return;
	}

	m_size = size;
	m_collisionMap.clear();
	m_collisionMap.resize(m_size.x*m_size.y, fillColliderType);
}

//------------------------------------------------------------------------------
void MapCollider::build(const TiledMap & map,
						const std::string collisionLayerName,
						const std::string collisionTileSetName)
{
#ifdef ZN_DEBUG
	std::cout << "D: Building MapCollider from TiledMap..." << std::endl;
#endif

	if(map.tileSize.x != map.tileSize.y)
	{
		std::cout << "E: MapCollider::build: "
			"non-square tile size is not supported ! ("
			<< map.tileSize.x << ", " << map.tileSize.y << ")" << std::endl;
		return;
	}
	m_cellSize = map.tileSize.x;

	const TiledMap::Layer * collisionLayer = map.layerFromName(collisionLayerName);
	if(collisionLayer == nullptr)
	{
		std::cout << "E: MapCollider::build: "
			"the given TiledMap doesn't have any collision layer named \""
			<< collisionLayerName << "\"." << std::endl;
		return;
	}

	m_size = map.size;
	m_collisionMap.clear();
	m_collisionMap.resize(m_size.x*m_size.y, 0);

	if(collisionLayer->position.x != 0 || collisionLayer->position.y != 0
		|| collisionLayer->size.x != m_size.x || collisionLayer->size.y != m_size.y)
	{
		std::cout << "E: MapCollider::build: "
			"TiledMap collision layer with position or different size is not supported !" << std::endl;
		return;
	}

	const TiledMap::TileSet * collisionTileSet = map.tileSetFromName(collisionTileSetName);
	if(collisionTileSet == nullptr)
	{
		std::cout << "E: MapCollider::build: "
			"the given TiledMap doesn't uses any tileset named \""
			<< collisionTileSetName << "\"" << std::endl;
		return;
	}

#ifdef ZN_DEBUG
	std::map<s32,u32> colliderCounts;
#endif

	for(u32 i = 0; i < m_collisionMap.size(); ++i)
	{
		s32 ct = collisionLayer->grid[i] - collisionTileSet->firstgid;
#ifdef ZN_DEBUG
		if(ct < 0 || ct >= static_cast<s32>(m_colliderTypes.size()))
		{
			++colliderCounts[-1];
		}
		++colliderCounts[ct];
#endif
		if(ct < 0 || ct >= static_cast<s32>(m_colliderTypes.size()))
		{
			ct = 0;
		}
		m_collisionMap[i] = ct;
	}

#ifdef ZN_DEBUG
	std::cout << "D: MapCollider build " << m_size.x << "x" << m_size.y << ". Collider counts:" << std::endl;
	for(auto it = colliderCounts.begin(); it != colliderCounts.end(); ++it)
	{
		if(it->first < 0)
			std::cout << "D: [unknown]: " << it->second << std::endl;
		else
			std::cout << "D: [" << (s32)it->first << "]: " << it->second << std::endl;
	}
#endif

	r_tiledMap = &map;
	m_tiledLayerName = collisionLayerName;
	m_tiledTilesetName = collisionTileSetName;
}

//------------------------------------------------------------------------------
bool MapCollider::collides(const sf::Vector2f & pos) const
{
	sf::Vector2f rpos = pos - entity().position();
	u8 ct = cellCollider(sf::Vector2i(rpos.x/m_cellSize, pos.y/m_cellSize));
	if(ct == 0 || ct >= m_colliderTypes.size())
		return false;
	rpos.x = math::repeat(rpos.x, m_cellSize);
	rpos.y = math::repeat(rpos.y, m_cellSize);
	const sf::FloatRect & r = m_colliderTypes[ct];
	return r.contains(rpos);
}

//------------------------------------------------------------------------------
bool MapCollider::collides(const sf::FloatRect & r0) const
{
	sf::FloatRect r;
	r.left = r0.left - entity().position().x;
	r.top = r0.top - entity().position().y;
	r.width = r0.width;
	r.height = r0.height;

	s32 minX = math::clamp(static_cast<s32>(r.left / m_cellSize), 0, m_size.x-1);
	s32 minY = math::clamp(static_cast<s32>(r.top / m_cellSize), 0, m_size.y-1);
	s32 maxX = math::clamp(static_cast<s32>((r.left+r.width-1) / m_cellSize), 0, m_size.x-1);
	s32 maxY = math::clamp(static_cast<s32>((r.top+r.height-1) / m_cellSize), 0, m_size.y-1);

	sf::Vector2i pos;
	sf::FloatRect mapRect;

	for(pos.y = minY; pos.y <= maxY; ++pos.y)
	{
		for(pos.x = minX; pos.x <= maxX; ++pos.x)
		{
			u8 ct = cellCollider(pos);
			if(ct != 0 && ct < m_colliderTypes.size())
			{
				mapRect.left = m_colliderTypes[ct].left + pos.x * m_cellSize;
				mapRect.top = m_colliderTypes[ct].top + pos.y * m_cellSize;
				mapRect.width = m_colliderTypes[ct].width;
				mapRect.height = m_colliderTypes[ct].height;

				if(intersects(mapRect, r))
					return true;
			}
		}
	}

	return false;
}

//------------------------------------------------------------------------------
void MapCollider::setCellSize(f32 cellSize)
{
	if(cellSize < 0)
	{
		std::cout << "E: MapCollider::setCellSize: invalid value " << cellSize << std::endl;
		cellSize = 0;
	}
	m_cellSize = cellSize;
}

//------------------------------------------------------------------------------
void MapCollider::setColliderType(u8 index, const sf::FloatRect & box)
{
	if(index >= m_colliderTypes.size())
		m_colliderTypes.resize(index+1);
	m_colliderTypes[index] = box;
}

//------------------------------------------------------------------------------
const sf::FloatRect & MapCollider::colliderType(u8 index)
{
	return m_colliderTypes[index];
}

//------------------------------------------------------------------------------
void MapCollider::setCellCollider(const sf::Vector2i & cellPos, u8 colliderType)
{
#ifdef ZN_DEBUG
	if(colliderType >= m_colliderTypes.size())
	{
		std::cout << "E: MapCollider::setCellCollider: "
			"invalid collider ID " << (u32)colliderType
			<< ", max is " << m_colliderTypes.size()-1 << std::endl;
	}
#endif

	s32 i = cellIndex(cellPos);
	if(i >= 0 && i < static_cast<s32>(m_collisionMap.size()))
		m_collisionMap[i] = colliderType;
#ifdef ZN_DEBUG
	else
	{
		std::cout << "E: MapCollider::setCellCollider: "
			"invalid position (" << cellPos.x << ", " << cellPos.y << ")" << std::endl;
	}
#endif
}

//------------------------------------------------------------------------------
u8 MapCollider::cellCollider(const sf::Vector2i & cellPos) const
{
	s32 i = cellIndex(cellPos);
	if(i >= 0 && i < static_cast<s32>(m_collisionMap.size()))
		return m_collisionMap[i];
	else
		return m_outerColliderType;
}

//------------------------------------------------------------------------------
void MapCollider::serializeData(JsonBox::Value & o)
{
	ACollider::serializeData(o);

	o["cellSize"] = m_cellSize;
	zn::serialize(o["size"], m_size);
	o["voidColliderShape"] = m_outerColliderType;

	// Save collision shapes
	JsonBox::Array shapesData;
	shapesData.resize(m_colliderTypes.size());
	for(u32 i = 0; i < shapesData.size(); ++i)
	{
		zn::serialize(shapesData[i], m_colliderTypes[i]);
	}
	o["collisionShapes"] = shapesData;

	// If the collider is derived from a TiledMap
	if(r_tiledMap != nullptr)
	{
		// Save TiledMap reference
		std::string tiledMapName = AssetBank::current()->maps.findName(r_tiledMap);
		o["tiledMap"] = tiledMapName;
		o["tiledMapLayer"] = m_tiledLayerName;
		o["tiledMapTileset"] = m_tiledTilesetName;
	}
	else
	{
		// Save map data directly
		JsonBox::Array mapData;
		mapData.resize(m_collisionMap.size());
		for(u32 i = 0; i < m_collisionMap.size(); ++i)
		{
			mapData[i] = m_collisionMap[i];
		}
		o["mapData"] = mapData;
	}
}

//------------------------------------------------------------------------------
void MapCollider::unserializeData(JsonBox::Value & o)
{
	ACollider::unserializeData(o);

	m_cellSize = o["cellSize"].getDouble();
	zn::unserialize(o["size"], m_size);
	m_outerColliderType = o["voidColliderShape"].getInt();

	// Load collision shapes
	JsonBox::Value & shapesData = o["collisionShapes"];
	u32 n = shapesData.getArray().size();
	m_colliderTypes.resize(n);
	for(u32 i = 0; i < n; ++i)
	{
		sf::FloatRect rect;
		zn::unserialize(shapesData[i], rect);
		m_colliderTypes[i] = rect;
	}

	// Get TiledMap reference name
	std::string tiledMapName = o["tiledMap"].getString();

	// If the name is not empty
	if(!tiledMapName.empty())
	{
		// Build the map from TiledMap
		const TiledMap * tiledMap = AssetBank::current()->maps.get(tiledMapName);
		build(
			*tiledMap,
			o["tiledMapLayer"].getString(),
			o["tiledMapTileset"].getString()
		);
	}
	else
	{
		// Build the map from direct data
		const JsonBox::Array & mapData = o["mapData"].getArray();
		m_collisionMap.resize(mapData.size());
		for(u32 i = 0; i < mapData.size(); ++i)
		{
			m_collisionMap[i] = mapData[i].getInt();
		}
	}
}

//------------------------------------------------------------------------------
void MapCollider::postUnserialize()
{

}

//------------------------------------------------------------------------------
#ifdef ZN_DEBUG
void MapCollider::debug_draw(sf::RenderTarget & target) const
{
	sf::RectangleShape rect;
	sf::Vector2i pos;

	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1);
	rect.setFillColor(sf::Color::Transparent);

	for(pos.y = 0; pos.y < m_size.y; ++pos.y)
	{
		for(pos.x = 0; pos.x < m_size.x; ++pos.x)
		{
			u8 ct = cellCollider(pos);
			if(ct != 0)
			{
				const sf::FloatRect & r = m_colliderTypes[ct];

				rect.setSize(sf::Vector2f(r.width, r.height));
				rect.setOrigin(-r.left, -r.top);
				rect.setPosition(m_cellSize*sf::Vector2f(pos.x, pos.y) + entity().position());
				target.draw(rect);
			}
		}
	}
}
#endif

} // namespace zn


