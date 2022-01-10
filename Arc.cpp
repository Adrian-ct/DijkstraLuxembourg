#include "Arc.h"

Arc::Arc(NodePtr n1, NodePtr n2)
{
	this->m_node1 = n1;
	this->m_node2 = n2;
}

Arc::Arc(NodePtr n1, NodePtr n2, Node::Type type) :
	m_node1(n1),
	m_node2(n2),
	m_type(type)
{}

Arc::Arc(const Node& n1, const Node& n2)
{
	this->m_node1 = std::make_shared<Node>(Node(n1));
	this->m_node2 = std::make_shared<Node>(Node(n2));
}

Arc::Arc(Arc&& arc)noexcept
{
	*this = arc;
}

Arc& Arc::operator=(const Arc& arc)
{
	if (this != &arc) {
		m_node1 = std::make_shared<Node>(*arc.m_node1);
		m_node2 = std::make_shared<Node>(*arc.m_node2);
		m_type = arc.m_type;
	}
	return *this;
}

Arc& Arc::operator=(Arc&& arc)noexcept
{
	if (this != &arc)
	{
		m_node1 = arc.m_node1;
		m_node2 = arc.m_node2;
		m_type = arc.m_type;
	}
	return *this;
}

Arc::NodePtr const Arc::getFirstNode() const
{
	return m_node1;
}

Arc::NodePtr const Arc::getSecondNode() const
{
	return m_node2;
}

Node::Type Arc::getType() const
{
	return m_type;
}

void Arc::setFirstPoint(NodePtr node)
{
	this->m_node1 = node;
}

void Arc::setSecondPoint(NodePtr node)
{
	m_node2 = node;
}
