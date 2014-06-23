#ifndef HEADER_ZN_MAPCOLLIDER_HPP_INCLUDED
#define HEADER_ZN_MAPCOLLIDER_HPP_INCLUDED

#include <vector>
#include "Collider.hpp"
#include "../../asset/TiledMap.hpp"

namespace zn
{

/// \brief Simple map collider with static square cells.
/// Each cell is associated to a simple collider primitive (square, triangle...),
/// which is identified by integer IDs.
/// Currently, only rectangular shapes are supported (more will come with the integration of a physics engine).
class ZN_API MapCollider : public Collider
{
public:

	ZN_OBJECT(zn::MapCollider, zn::Collider)

	MapCollider();

    /// \brief Tests if a point is inside a blocking cell of the map
    /// \param pos: Position in world space
    /// \return true if it collides, false otherwise
	bool collides(const sf::Vector2f & pos) const override;

    /// \brief Tests if a rectangle intersects a blocking cell of the map
    /// \param r0: Axis-aligned rectangle in world space
    /// \return true if it collides, false otherwise
	bool collides(const sf::FloatRect & r0) const override;

	sf::FloatRect bounds() const override;

    /// \brief rebuilds the collider
    /// \param size: size of the collision grid
    /// \param fillColliderType: ID of the collision type to fill in
	void build(const sf::Vector2i & size, u8 fillColliderType=0);

    /// \brief rebuilds the collider from a TiledMap (Tiles will be interpreted as collisions or not).
    /// \param map: Tiled map to use
    /// \param collisionLayerName: name of the collision layer within the Tiled map
    /// \param collisionTileSetName: name of the collision tileset within the Tiled map
	void build(
		const TiledMap & map,
		const std::string collisionLayerName="collision",
		const std::string collisionTileSetName="collision"
	);

	void build(
		const std::string & mapName,
		const std::string collisionLayerName="collision",
		const std::string collisionTileSetName="collision"
	);

	void setCellSize(f32 cellSize);
	inline f32 cellSize() const { return m_cellSize; }

	/// \brief Setups a type of collider as a box.
	/// Types are internally stored in an array, index will be used to access its value.
	/// If the array is too small, it will be resized.
	/// \param index: ID of the collision type
	/// \note collider ID 0 will always be treated as empty.
	/// \param box: geometry of the collision type in a size range of 0 to 1
	void setColliderType(u8 index, const sf::FloatRect & box);

	/// \brief Setups the type of collider used out of map bounds.
	/// \param index: ID of the collision to use. 0 means no collision.
	void setVoidColliderType(u8 index);

	/// \brief returns the hitbox of a collision ID within this map.
	/// \param index: existing ID of the collision type
	/// \return axis-aligned hitbox of the collider in size range of 0 to 1.
	const sf::FloatRect & colliderType(u8 index);

	/// \brief returns the hitbox of a collision ID within this map.
	inline u32 colliderTypesCount() const { return m_colliderTypes.size(); }

	void setCell(const sf::Vector2i & cellPos, u8 colliderType);

	u8 cellAt(const sf::Vector2i & cellPos) const;

#ifdef ZN_DEBUG
	void debug_draw(sf::RenderTarget & target) const override;
#endif

	// Serialization
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;

private:

	inline u32 cellIndex(const sf::Vector2i pos) const
	{
		return pos.y*m_size.x + pos.x;
	}

	f32                           m_cellSize;
	sf::Vector2i                  m_size;
	std::vector<u8>               m_collisionMap;
	std::vector<sf::FloatRect>    m_colliderTypes;
	u8                            m_voidColliderType;

	// If the MapCollider is built from a TiledMap:
	const TiledMap *              r_tiledMap;
	std::string                   m_tiledLayerName;
	std::string                   m_tiledTilesetName;

};

} // namespace zn

#endif // HEADER_ZN_MAPCOLLIDER_HPP_INCLUDED


