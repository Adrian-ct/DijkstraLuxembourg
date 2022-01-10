#pragma once
#include "Node.h"
#include <memory>

class Arc
{
public:
	using NodePtr = std::shared_ptr<Node>;

	Arc() = default;
	Arc(NodePtr n1, NodePtr n2);
	Arc(NodePtr n1, NodePtr n2, Node::Type type);
	Arc(const Node& n1, const Node& n2);
	~Arc() = default;
	Arc(Arc&& arc)noexcept;
	Arc& operator=(const Arc& arc);
	Arc& operator=(Arc&& arc)noexcept;

	NodePtr const getFirstNode()const;
	NodePtr const getSecondNode()const;
	Node::Type getType()const;

	void setFirstPoint(NodePtr node);
	void setSecondPoint(NodePtr node);
private:
	Node::Type m_type;
	
	NodePtr m_node1,  m_node2;
};

