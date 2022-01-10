#include "Luxembourg.h"

Luxembourg::Luxembourg(QWidget* parent)
	: QMainWindow(parent),
	m_drawByHand(false),
	luxembourg(false),
	IsTree(true)
{
	ui.setupUi(this);
	connect(ui.draw, &QPushButton::clicked, this, &Luxembourg::on_draw_clicked);
	connect(ui.luxembourg, &QPushButton::clicked, this, &Luxembourg::on_luxembourg_clicked);
	connect(ui.root, &QPushButton::clicked, this, &Luxembourg::on_root_clicked);
	connect(ui.Dijkstra, &QPushButton::clicked, this, &Luxembourg::on_Dijkstra_clicked);
}

void Luxembourg::paintEvent(QPaintEvent* event)
{
	QPainter p(this);

	if (m_drawByHand)
	{
		DrawByHand(p);
		if (!IsTree)
		{
			QPointF point = ui.root->pos();
			point.setY(point.y() + 70);
			p.drawText(point, QString("Graf incompatibil"));
		}
	}
	else
		if (luxembourg)
			drawLuxembourg(p);
}

void Luxembourg::mouseReleaseEvent(QMouseEvent* e)
{
	if (m_drawByHand)
	{
		drawNode = false;
		drawArc = false;
		if (e->button() == Qt::RightButton)
		{

			if (isValid(point))
			{
				Arc::NodePtr n = std::make_shared<Node>(std::move(Node(e->localPos(), graf.GetVectorOfNodes().size())));
				graf.pushNode(n);
				drawNode = true;
				firstNode = Node();
				secondNode = Node();

				update();
			}
		}
		else
			if (e->button() == Qt::LeftButton)
			{
				if (pressedNode)
				{
					if (!isValid(pressedNode->GetPoint()))
					{
						pressedNode->SetPoint(this->point);
						graf.moveArcs(pressedNode);
						firstNode = Node();
						secondNode = Node();
						update();
					}
				}


				QPointF p = e->localPos();

				const std::vector<Arc::NodePtr>& noduri = graf.GetVectorOfNodes();

				Node foundNode;
				foundNode.SetNumber(-1);

				for (auto n : noduri)
				{
					if (fabs(n->GetPoint().x() - p.x()) < 20 && fabs(n->GetPoint().y() - p.y()) < 20)
					{
						foundNode = *n;
						break;
					}
				}

				if (foundNode.GetNumber() == -1)
				{
					firstNode = Node();
					secondNode = Node();
					update();
					return;
				}

				if (firstNode.GetNumber() == -1)
				{
					firstNode = foundNode;
					return;
				}
				else if (firstNode.GetNumber() != foundNode.GetNumber())
				{
					uint16_t nrOfArcsBetweenTheNodes = graf.getNumberOfArcs(firstNode, foundNode);

					if (nrOfArcsBetweenTheNodes < 2)
					{
						secondNode = foundNode;
						graf.pushArcs(std::make_shared<Arc>(std::move(Arc(firstNode, secondNode))));
						firstNode = Node();
						secondNode = Node();
						drawArc = true;
						update();
					}
					else
					{
						firstNode = Node();
						secondNode = Node();
						update();
					}
				}
			}
	}
	else
		if (luxembourg)
		{
			QPointF p = e->localPos();
			std::pair<int, int> latMaxMin = graf.GetLatMaxMin();
			std::pair<int, int> longMaxMin = graf.GetLongMaxMin();

			const std::vector<Arc::NodePtr>& noduri = graf.GetVectorOfNodes();

			Node foundNode;
			foundNode.SetNumber(-1);
			if (firstNode.GetNumber() == -1 || secondNode.GetNumber() == -1)
			{

				for (auto n : noduri)
				{
					float x = width() / ((longMaxMin.first - longMaxMin.second) / divider) * 
						((n->GetPoint().y() - longMaxMin.second) / divider);
					x += xSpace;
					float y = height() / ((latMaxMin.first - latMaxMin.second) / divider) *
						((latMaxMin.first - n->GetPoint().x()) / divider);

					y += ySpace;

					if (fabs(x - p.x()) <= 5 && fabs(y - p.y()) < 5)
					{
						foundNode = *n;
						break;
					}
				}

				if (foundNode.GetNumber() == -1)
				{
					firstNode = Node();
					secondNode = Node();
					update();
					return;
				}

				if (firstNode.GetNumber() == -1)
				{
					firstNode = foundNode;
					graf.GetVectorOfNodes().at(firstNode.GetNumber())->SetType(Node::Type::EStart);
					std::cout << "Nod start ales: " << firstNode.GetNumber() << "\n";
					update();
					return;
				}
				else if (firstNode.GetNumber() != foundNode.GetNumber())
				{
					secondNode = foundNode;
					graf.GetVectorOfNodes().at(secondNode.GetNumber())->SetType(Node::Type::EStart);
					ui.Dijkstra->setEnabled(true);
					std::cout << "Nod finish ales: " << secondNode.GetNumber() << "\n";
					update();
				}
			}
		}
}

void Luxembourg::mousePressEvent(QMouseEvent* e)
{
	if (m_drawByHand)
	{
		if (e->button() == Qt::LeftButton)
		{
			for (auto node : graf.GetVectorOfNodes())
			{
				if (fabs(e->x() - node->GetPoint().x()) < 30 && fabs(e->y() -
					node->GetPoint().y()) < 30) {
					pressedNode = node;
					this->point = node->GetPoint();
					return;
				}
			}
		}
		pressedNode = nullptr;
	}
}

void Luxembourg::mouseMoveEvent(QMouseEvent* e)
{
	if (m_drawByHand)
	{
		if (!(e->buttons() & Qt::LeftButton))
			return;
		if (pressedNode)
		{
			pressedNode->SetPoint(e->pos());
			graf.moveArcs(pressedNode);
			update();
		}
	}
}

void Luxembourg::Dijkstra()
{
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
	std::chrono::time_point<std::chrono::system_clock> m_EndTime;

	auto compare = [&](std::pair < Arc::NodePtr, int>& n1, std::pair<Arc::NodePtr, int>& n2) {return (n1).second >
		n2.second; };

	std::priority_queue<std::pair<Arc::NodePtr, int>, std::vector<std::pair<Arc::NodePtr, int>>, decltype(compare)> q(compare);

	std::unordered_map<Arc::NodePtr, std::pair<int, Arc::NodePtr>, Graf::hashNode>& path = graf.getPath();
	std::vector<Arc::NodePtr>& nodes = graf.GetVectorOfNodes();

	Arc::NodePtr first = nodes[firstNode.GetNumber()];
	Arc::NodePtr second = nodes[secondNode.GetNumber()];

	std::vector<bool> seen(nodes.size(), false);
	path[first].first = 0;
	q.push({ first, 0 });

	m_StartTime = std::chrono::system_clock::now();

	while (!q.empty())
	{
		Arc::NodePtr start = q.top().first;
		q.pop();

		if (seen[start->GetNumber()])
			continue;

		seen[start->GetNumber()] = true;

		if (start == second)
			break;

		for (auto end : graf.getNeighbours()[start])
		{
			Arc::NodePtr endNode = end.first;
			int weight = end.second;

			if (!seen[endNode->GetNumber()] && path[start].first + weight < path[endNode].first)
			{
				path[endNode].first = path[start].first + weight;
				path[endNode].second = start;
				q.push({ endNode, path[start].first + weight });
			}
		}
	}


	Arc::NodePtr aux = second;
	while (path[aux].second != nullptr)
	{
		aux->SetType(Node::Type::EWalked);
		aux = path[aux].second;
	}

	m_EndTime = std::chrono::system_clock::now();
	std::cout << "Am ajuns la ultimul nod in: " << std::chrono::duration_cast<std::chrono::milliseconds>(m_EndTime - m_StartTime).count() / 60.0;

	update();
}

bool Luxembourg::isValid(const QPointF& point)
{
	if (graf.GetVectorOfNodes().size() > 0)
	{
		for (const auto n : graf.GetVectorOfNodes())
		{
			if (n->GetPoint() == point)
				continue;
			if (fabs(n->GetPoint().x() - point.x()) < 28 && fabs(n->GetPoint().y() - point.y()) < 28)
				return false;
		}
	}
	return true;
}

void Luxembourg::DrawByHand(QPainter& p)
{
	const float ellipseStroke = 2.9;
	const float rectangleSize = 35;


	if (graf.GetVectorOfNodes().size())
	{
		const std::vector<Arc::NodePtr>& noduri = graf.GetVectorOfNodes();
		QPointF sourcePoint, destPoint;
		p.setRenderHint(QPainter::Antialiasing);

		for (auto nod : noduri)
		{
			QPen pen;
			pen.setWidth(ellipseStroke);
			p.setPen(pen);
			p.setBrush(QBrush(Qt::green));
			QRectF r(nod->GetPoint().x() - 17.5, nod->GetPoint().y() - 17.5, rectangleSize, rectangleSize);
			p.drawEllipse(r);

			QPointF newPoint = r.center();
			newPoint.setX(newPoint.x() - 2);
			newPoint.setY(newPoint.y() + 2);

			p.drawText(newPoint, QString::number(nod->GetNumber()));
		}

		const std::vector<Graf::ArcPtr>& arce = graf.GetVectorOfArcs();
		for (auto arc : arce)
		{
			QPen pen;
			pen.setWidth(2);
			pen.setColor(Qt::blue);
			p.setPen(pen);

			QLineF line(arc->getFirstNode()->GetPoint(), arc->getSecondNode()->GetPoint());
			qreal length = line.length();


			if (length > qreal(10.)) {
				QPointF edgeOffset(((line.dx() * qreal(17.5)) / length),
					((line.dy() * qreal(17.5)) / length));

				sourcePoint = line.p1() + edgeOffset;
				destPoint = line.p2() - edgeOffset;
			}
			else {
				sourcePoint = destPoint = line.p1();
			}

			graf.DrawLineWithArrow(p, sourcePoint, destPoint);
		}

		if (drawNode)
		{
			Arc::NodePtr n = noduri.at(noduri.size() - 1);
			QPen pen;
			pen.setWidth(ellipseStroke);
			p.setPen(pen);
			p.setBrush(QBrush(Qt::red));
			QRectF r(n->GetPoint().x() - 17.5, n->GetPoint().y() - 17.5, rectangleSize, rectangleSize);
			p.drawEllipse(r);
			QPointF newPoint = r.center();

			newPoint.setX(newPoint.x() - 2);
			newPoint.setY(newPoint.y() + 2);
			p.drawText(newPoint, QString::number(n->GetNumber()));
		}
		else if (drawArc)
		{
			drawArc = false;
			QPen pen;
			pen.setWidth(2);
			p.setBrush(QBrush(Qt::red));
			p.setPen(pen);

			QLineF line(arce.at(arce.size() - 1)->getFirstNode()->GetPoint(),
				arce.at(arce.size() - 1)->getSecondNode()->GetPoint());
			qreal length = line.length();


			if (length > qreal(10.)) {
				QPointF edgeOffset(((line.dx() * qreal(17.5)) / length),
					((line.dy() * qreal(17.5)) / length));

				sourcePoint = line.p1() + edgeOffset;
				destPoint = line.p2() - edgeOffset;
			}
			else {
				sourcePoint = destPoint = line.p1();
			}

			graf.DrawLineWithArrow(p, sourcePoint,
				destPoint);

			firstNode = Node();
			secondNode = Node();

		}
	}
}

void Luxembourg::drawLuxembourg(QPainter& p)
{
	float nodeSize = 4;
	p.setRenderHint(QPainter::Antialiasing);
	std::pair<int, int> latMaxMin = graf.GetLatMaxMin();
	std::pair<int, int> longMaxMin = graf.GetLongMaxMin();

	for (auto arc : graf.GetVectorOfArcs())
	{
		float x = width() / ((longMaxMin.first - longMaxMin.second) / divider) * 
			((arc->getFirstNode()->GetPoint().y() - longMaxMin.second) / divider);
		x += xSpace;
		float y = height() / ((latMaxMin.first - latMaxMin.second) / divider) *
			((latMaxMin.first- arc->getFirstNode()->GetPoint().x()) / divider);
		y += ySpace;

		float x2 = width() / ((longMaxMin.first - longMaxMin.second) / divider) *
			((arc->getSecondNode()->GetPoint().y() - longMaxMin.second) / divider);
		x2 += xSpace;
		float y2 = height() / ((latMaxMin.first - latMaxMin.second) / divider) *
			((latMaxMin.first - arc->getSecondNode()->GetPoint().x()) / divider);
		y2 += ySpace;

		if (arc->getFirstNode()->GetType() == Node::Type::EWalked || arc->getSecondNode()->GetType() == Node::Type::EWalked)
		{
			QPen pen(Qt::red, 1);
			p.setPen(pen);
		}
		else
			if (arc->getFirstNode()->GetType() == Node::Type::EDefault || arc->getSecondNode()->GetType() == Node::Type::EDefault)
			{
				QPen pen(Qt::blue, 0.5);
				p.setPen(pen);
			}
		p.drawLine(x, y, x2, y2);
	}
}

void Luxembourg::on_draw_clicked()
{
	graf.DeleteAllArcs();
	graf.DeleteAllNodes();
	graf.getNeighbours().clear();
	m_drawByHand = true;
	luxembourg = false;
	ui.root->setEnabled(true);
	update();
}

void Luxembourg::on_luxembourg_clicked()
{
	graf.DeleteAllArcs();
	graf.DeleteAllNodes();
	graf.Read();
	m_drawByHand = false;
	luxembourg = true;
	ui.root->setEnabled(false);
	update();
}

void Luxembourg::on_root_clicked()
{
	if (!graf.isTree())
		graf.gasesteRadacina();
	else
	{
		IsTree = false;
		update();
	}
}

void Luxembourg::on_Dijkstra_clicked()
{
	Dijkstra();
}
