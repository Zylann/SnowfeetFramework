#ifndef HEADER_ZN_MAPCOLLIDER_HPP_INCLUDED
#define HEADER_ZN_MAPCOLLIDER_HPP_INCLUDED

#include <vector>
#include "Collider.hpp"
#include "../../asset/TiledMap.hpp"

namespace zn
{

// Simple map collider with static square cells.
class MapCollider : public ACollider
{
public:

	ZN_COMPONENT(zn::MapCollider, CG_COLLIDER)

	MapCollider();

	bool collides(const sf::Vector2f & pos) const override;
	bool collides(const sf::FloatRect & r0) const override;
	sf::FloatRect bounds() const override;

	void build(const sf::Vector2i & size, u8 fillColliderType=0);

	void build(
		const TiledMap & map,
		const std::string collisionLayerName="collision",
		const std::string collisionTileSetName="collision"
	);

	void setCellSize(f32 cellSize);
	inline f32 cellSize() const { return m_cellSize; }

	// Setups a type of collider as a box.
	// Note: collider type 0 will always be treated as empty.
	void setColliderType(u8 index, const sf::FloatRect & box);

	// Setups the type of collider used out of map bounds.
	void setOuterColliderType(u8 index);

	const sf::FloatRect & colliderType(u8 index);
	inline u32 colliderTypesCount() const { return m_colliderTypes.size(); }

	void setCellCollider(const sf::Vector2i & cellPos, u8 colliderType);
	u8 cellCollider(const sf::Vector2i & cellPos) const;

#ifdef ZN_DEBUG
	void debug_draw(sf::RenderTarget & target) const override;
#endif

private:

	inline u32 cellIndex(const sf::Vector2i pos) const
	{
		return pos.y*m_size.x + pos.x;
	}

	f32                           m_cellSize;
	sf::Vector2i                  m_size;
	std::vector<u8>               m_collisionMap;
	std::vector<sf::FloatRect>    m_colliderTypes;
	u8                            m_outerColliderType;

};

} // namespace zn

#endif // HEADER_ZN_MAPCOLLIDER_HPP_INCLUDED


