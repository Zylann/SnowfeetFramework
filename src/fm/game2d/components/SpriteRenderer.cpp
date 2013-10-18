#include <iostream>
#include "SpriteRenderer.hpp"
#include "../Entity.hpp"
#include "../../sfml/sfml2_utils.hpp"

using namespace std;

namespace zn
{

void SpriteRenderer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void SpriteRenderer::setTextureRect(const sf::IntRect& rect)
{
	if(m_sprite.getTexture() == nullptr)
		return;
#ifdef ZN_DEBUG
	if(!checkTextureRect(*m_sprite.getTexture(), rect))
		cout << "E: SpriteRenderer::setTextureRect: invalid rectangle ("
			<< rect.left << ", " << rect.top << "; "
			<< rect.width << ", " << rect.height << ")" << endl;
#endif
	m_sprite.setTextureRect(rect);
}

void SpriteRenderer::setAtlas(const TextureAtlas * atlas)
{
	if(owner().animator() != nullptr)
		owner().animator()->stop();
	r_atlas = atlas;
	m_sprite.setTexture(r_atlas->texture());
}

void SpriteRenderer::setFrame(const std::string& id)
{
	if(r_atlas != nullptr)
	{
		const TextureAtlas::Frame * f = r_atlas->frame(id);
		if(f != nullptr)
		{
			m_sprite.setTextureRect(f->rect);

			if(owner().animator() != nullptr)
			{
				owner().animator()->stop();
			}
		}
#ifdef ZN_DEBUG
		else
			cout << "E: SpriteRenderer::setFrame: not found \"" << id << "\")" << endl;
#endif
	}
#ifdef ZN_DEBUG
	else
		cout << "E: SpriteRenderer::setFrame: no TextureAtlas" << endl;
#endif
}

} // namespace zn


