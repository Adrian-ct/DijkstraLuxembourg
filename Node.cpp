#include "Node.h"

Node::Node(const int& nr) : m_number(nr)
{
}

Node::Node() :
	m_number(-1),
	m_type(Type::EDefault)
{

}
Node::Node(const QPointF& point, const int& nr) :m_point(point), m_number(nr), 
m_type(Type::EDefault)
{}


Node::Node(const Node& other)
{
	m_number = other.m_number;
	m_point = other.m_point;
	m_type = other.m_type;
}

Node::Node(Node&& node)
{
	*this = std::move(node);
}

Node& Node::operator=(const Node& node)
{
	if (this != &node)
	{
		m_number =node.m_number;
		m_point =node.m_point;
		m_type = node.m_type;
	}
	return *this;
}

Node& Node::operator=(Node&& node)
{
	if (this != &node)
	{
		m_number = node.m_number;
		m_point = std::move(node.m_point);
		m_type = node.m_type;
	}
	return *this;
}

QPointF Node::GetPoint() const
{
	return m_point;
}

void Node::SetPoint(const QPointF& point)
{
	m_point = point;
}

void Node::SetNumber(const int& newNumber)
{
	m_number = newNumber;
}

int Node::GetNumber() const
{
	return m_number;
}

void Node::SetType(Type type)
{
	m_type = type;
}

Node::Type Node::GetType() const
{
	return m_type;
}