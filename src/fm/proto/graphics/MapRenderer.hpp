#ifndef HEADER_ZN_MAPRENDERER_HPP_INCLUDED
#define HEADER_ZN_MAPRENDERER_HPP_INCLUDED

#include "Renderer.hpp"
#include "../../asset/TiledMap.hpp"

namespace zn
{

// 2D orthographic map renderer
class MapRenderer : public ARenderer
{
public:

	ZN_COMPONENT(zn::MapRenderer, CG_RENDERER)

	MapRenderer() : ARenderer(),
		r_tiledMap(nullptr),
		r_atlas(nullptr),
		m_needUpdate(false)
	{}

	void setAtlas(const TextureAtlas * atlas) override;

	void setMap(const TiledMap * map,
				const TextureAtlas * atlas = nullptr,
				const std::string bgLayerName="background");

	// Serialize
	void serializeData(JsonBox::Value & o) override;
	void unserializeData(JsonBox::Value & o) override;
	void postUnserialize() override;

protected:

	void update() override;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	void updateMesh();

private:

	const TiledMap *       r_tiledMap;
	const TextureAtlas *   r_atlas;
	bool                   m_needUpdate;
	std::string            m_bgLayerName;
	sf::VertexArray        m_vertices;

};

} // namespace zn

#endif // HEADER_ZN_MAPRENDERER_HPP_INCLUDED


