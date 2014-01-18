/*
PerformanceGraph.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#ifndef HEADER_ZN_PERFORMANCEGRAPH_HPP_INCLUDED
#define HEADER_ZN_PERFORMANCEGRAPH_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <fm/util/Range.hpp>

namespace zn
{

// A simple histogram showing the evolution of a value over time.
class PerformanceGraph : public sf::Drawable
{
public:

	PerformanceGraph();

	void setPosition(sf::Vector2f pos);
	void setSize(sf::Vector2i size);
	void setValueRange(f32 min, f32 max);
	void setColor(sf::Color color);

	void pushValue(f32 y);

	void update();
	void updateMesh();

protected:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:

	sf::Vector2f      m_pos;
	sf::Vector2i      m_size;
	Range<f32>        m_valueRange;
	sf::Color         m_color;
	std::vector<f32>  m_values;
	u32               m_currentIndex;
	sf::VertexArray   m_vertices;
	sf::VertexArray   m_background;

};

} // namespace zn

#endif // HEADER_ZN_PERFORMANCEGRAPH_HPP_INCLUDED

