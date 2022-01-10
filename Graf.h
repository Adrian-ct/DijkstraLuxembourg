#pragma once
#include "Node.h"
#include "Arc.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <QPainter>

#include <unordered_map>
#include <unordered_set>
#include <QtXml/qxml.h>
#include <QtXml/qdom.h>
#include <QFile>
#include <QtCore>

class Graf
{
public:
	using ArcPtr = std::shared_ptr<Arc>;
	Graf() = default;
	~Graf();

	const std::vector<Arc::NodePtr>& GetVectorOfNodes()const;
	std::vector<Arc::NodePtr>& GetVectorOfNodes();
	const std::vector<ArcPtr>& GetVectorOfArcs();
	struct hashNode
	{
		int operator()(Arc::NodePtr node)const
		{
			return (node->GetNumber());
		}
	};

	struct hashPair
	{
		int operator()(const std::pair<Arc::NodePtr, int>& n)const
		{
			return (n.first->GetNumber());
		}
	};

	const std::unordered_map<Arc::NodePtr, std::unordered_set<int>, hashNode>& getListaAdiacenta();
	std::unordered_map<Arc::NodePtr, std::pair<int, Arc::NodePtr>, hashNode>& getPath();
	std::unordered_map < Arc::NodePtr, std::unordered_set<std::pair<Arc::NodePtr, int>, hashPair >, hashNode>& getNeighbours();;

	void pushNode(Arc::NodePtr node);
	void pushArcs(ArcPtr arc);
	void moveArcs(Arc::NodePtr node);
	void DeleteAllArcs();
	void DeleteAllNodes();
	void Read();
	void gasesteRadacina();
	void DFS(Arc::NodePtr x, std::vector<bool>& visited);
	void DrawLineWithArrow(QPainter& painter, QPointF start, QPointF end);

	bool isTree();

	uint16_t Previous(uint16_t node);
	uint16_t getNumberOfArcs(const Node& n1, const Node& n2);

	std::pair<int, int> GetLongMaxMin()const;
	std::pair<int, int> GetLatMaxMin()const;

private:
	int longMax, longMin, latMin, latMax;

	std::unordered_map < Arc::NodePtr, std::unordered_set<std::pair<Arc::NodePtr, int>, hashPair >, hashNode> m_neighbours;
	std::unordered_map<Arc::NodePtr, std::pair<int, Arc::NodePtr>, hashNode> m_path;

	std::vector<Arc::NodePtr> m_nodes;
	std::vector<ArcPtr> m_arcs;

	std::unordered_map<Arc::NodePtr, std::unordered_set<int>, hashNode> m_listaAdiacenta;

private:
	bool isCyclic(int v, bool visited[], bool recall[]);
};

