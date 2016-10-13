#include "BezierSquareCurve.hpp"

namespace sfc
{
	/*
		Private
	*/
	void BezierSquareCurve::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		if(this->m_mode == DEBUG)
			target.draw(this->m_debugLines, 4, sf::LinesStrip);
		target.draw(this->m_vertices, states);
	}

	/*
		Protected
	*/
	void BezierSquareCurve::calculateSquare(
	std::vector<sf::Vector2f> &vector,
	const size_t numSegments,
	const float limitCurve,
	sf::Vertex *debugLines)
	{
		vector.clear();

		if (!numSegments) // Any points at all?
			return;

		for(float progress = 0.f; progress <= (numSegments + 1) * limitCurve; progress += limitCurve)
			vector.push_back(this->getPoint(progress / (this->m_pointCount + 1)));

		if(debugLines)
		{
			debugLines[0] = {this->m_begin, sf::Color(0, 255, 0)};
			debugLines[1] = {this->m_controlPoint, sf::Color(0, 255, 0)};
			debugLines[2] = {this->m_controlPoint, sf::Color(0, 255, 0)};
			debugLines[3] = {this->m_end, sf::Color(0, 255, 0)};
		}
	}

	/*
		Public
	*/
	BezierSquareCurve::BezierSquareCurve()
	{
		this->m_color = {255, 255, 255};
		this->m_limitCurve = 1.f;
		this->m_pointCount = 32;
	}

	BezierSquareCurve::BezierSquareCurve(const sf::Vector2f &start,
	const sf::Vector2f &end,
	const sf::Vector2f &controlPoint,
	DrawMode mode)
	{
		this->m_color = {255, 255, 255};
		this->m_limitCurve = 1.f;
		this->m_pointCount = 32;

		this->m_mode = mode;

		this->m_controlPoint = controlPoint;
		this->m_begin = start;
		this->m_end = end;
	}

	Line2<float> BezierSquareCurve::getHelperLine(const float normalized)
	{
		auto tmpLinePoint1 = this->m_begin + (this->m_controlPoint - this->m_begin) * normalized;
		auto tmpLinePoint2 = this->m_controlPoint + (this->m_end - this->m_controlPoint) * normalized;
		auto tmpLineVector = tmpLinePoint2 - tmpLinePoint1;

		return Line2<float>(tmpLinePoint1, tmpLineVector);
	}
	sf::Vector2f BezierSquareCurve::getPoint(const float normalized)
	{
		auto line = getHelperLine(normalized);
		return (line.point + line.vector * normalized);
	}

	void BezierSquareCurve::setPointPosition(BezierSquareCurve::PointType point, sf::Vector2f pos)
	{
		switch (point)
		{
		case CONTROL:
			this->m_controlPoint = pos;
			break;
		case BEGIN:
			this->m_begin = pos;
			break;
		case END:
			this->m_end = pos;
			break;
		}
	}
	sf::Vector2f BezierSquareCurve::getPointPosition(BezierSquareCurve::PointType point)
	{
		switch (point)
		{
		case CONTROL:
			return this->m_controlPoint;
		case BEGIN:
			return this->m_begin;
		case END:
			return this->m_end;
		default:
			return {0.f, 0.f};
		}
	}

	void BezierSquareCurve::update()
	{
		std::vector<sf::Vector2f> points;
		calculateSquare(
		points,
		this->m_pointCount,
		this->m_limitCurve,
		this->m_mode == NORMAL ? nullptr : this->m_debugLines);

		this->m_vertices.clear();
		for (auto i = 0u; i < points.size(); ++i)
			this->m_vertices.push_back({points[i], this->m_color});
	}
}