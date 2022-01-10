#pragma once
#include <QPoint>

class Node
{
public:
	enum class Type : uint16_t
	{
		EDefault,
		EStart,
		EWalked
	};

	Node();
	~Node() = default;
	Node(const int& nr);
	Node(const QPointF& point, const int& nr);
	Node(const Node& other);
	Node(Node&& node);
	Node& operator=(const Node& node);
	Node& operator=(Node&& node);

public:

	QPointF GetPoint()const;
	void SetPoint(const QPointF& point);
	void SetNumber(const int& newNumber);
	int GetNumber()const;

	void SetType(Type type);
	Type GetType()const;

private:

	QPointF m_point;
	int m_number;
	Type m_type;
};

