/*
PerformanceGraph.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraft-Framework project.
*/

#include "PerformanceGraph.hpp"
#include "../util/math.hpp"

namespace zn
{

PerformanceGraph::PerformanceGraph() : sf::Drawable(),
	m_valueRange(0, 100),
	m_color(255,128,0),
	m_currentIndex(0)
{
	m_vertices.setPrimitiveType(sf::Lines);

	m_background.setPrimitiveType(sf::Quads);
	m_background.resize(4);

	sf::Color bgColor(0,0,0,128);
	m_background[0].color = bgColor;
	m_background[1].color = bgColor;
	m_background[2].color = bgColor;
	m_background[3].color = bgColor;

	setSize(sf::Vector2i(200,100));
}

void PerformanceGraph::setPosition(sf::Vector2f pos)
{
	m_pos = pos;
}

void PerformanceGraph::setSize(sf::Vector2i size)
{
	if(size.x <= 2)
	{
		std::cout << "E: PerformanceGraph::setSize: invalid width (" << size.x << ")" << std::endl;
		return;
	}

	m_size = size;
	m_values.resize(size.x);
	std::cout << "Size set to " << m_values.size() << std::endl;

	updateMesh();
}

void PerformanceGraph::updateMesh()
{
	if(static_cast<s32>(m_vertices.getVertexCount()) != m_size.x*2)
	{
		m_vertices.resize(m_size.x * 2);
		for(u32 i = 0; i < m_vertices.getVertexCount(); i += 2)
		{
			m_vertices[i].position = sf::Vector2f(i/2, m_size.y);
			m_vertices[i+1].position = sf::Vector2f(i/2, m_size.y);

//			_vertices[i].color = sf::Color(_color.r/2, _color.g/2, _color.b/2, _color.a);
//			_vertices[i+1].color = _color;
		}

		m_background[0].position = sf::Vector2f(-1, -1);
		m_background[1].position = sf::Vector2f(m_size.x+2, -1);
		m_background[2].position = sf::Vector2f(m_size.x+2, m_size.y+2);
		m_background[3].position = sf::Vector2f(-1, m_size.y+2);
	}
}

void PerformanceGraph::setValueRange(f32 min, f32 max)
{
	m_valueRange.set(min, max);
}

void PerformanceGraph::setColor(sf::Color color)
{
	m_color = color;
}

void PerformanceGraph::pushValue(f32 y)
{
	//std::cout << m_currentIndex << ", " << _values.size() << std::endl;
	m_values[m_currentIndex] = y;

	sf::Color color1 = m_color;
	sf::Color color2 = sf::Color(m_color.r/3, m_color.g/3, m_color.b/3, m_color.a);

	s32 i0 = m_currentIndex*2;

	f32 v = math::clamp(m_valueRange.getT(y), 0.f, 1.f);
	m_vertices[i0+1].position.y = m_size.y - v * static_cast<f32>(m_size.y);

	m_vertices[i0+1].color = color1;
	m_vertices[i0].color = color2;

	// Color fade
	for(u32 i = 0; i < m_vertices.getVertexCount(); ++i)
	{
		sf::Color color = m_vertices[i].color;

		if(color.r > color2.r)
		{
			if(color.r == 1)
				color.r = 0;
			else
				color.r -= 2;
		}
		if(color.g > color2.g)
		{
			if(color1.g == 1)
				color1.g = 0;
			else
				color.g -= 2;
		}
		if(color.b > color2.b)
		{
			if(color.b == 1)
				color.b = 0;
			else
				color.b -= 2;
		}

		m_vertices[i].color = color;
	}

	++m_currentIndex;
	if(m_currentIndex == m_values.size())
	{
		m_currentIndex = 0;
	}
}

void PerformanceGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.translate(m_pos);
	target.draw(m_background, states);
	target.draw(m_vertices, states);
}

} // namespace zn


