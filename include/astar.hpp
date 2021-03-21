#pragma once

struct Node {
  int id;
  double x, y;
  double g, h, f;
  std::vector<std::pair<std::shared_ptr<Node>, double>> child;
  std::shared_ptr<Node> parent;

  double estimate_cost(std::shared_ptr<Node> trg) {
    double dx = x - trg->x;
    double dy = y - trg->y;

    return sqrt(dx * dx + dy * dy);
  }

  Node() {
    x = y = 0.0;
    g = h = f = 0.0;
  }

  bool operator<(const Node& rhs) {
    return this->f < rhs.f;
  }
};

using NODE_PTR = std::shared_ptr<Node>;

class Astar
{
public:
  Astar();
  ~Astar();

  void setStartGoal(NODE_PTR start, NODE_PTR goal);
  int findPath(std::list<NODE_PTR> &route);
  void step();
  bool findGoal();
  int makeRoute(std::list<NODE_PTR> &route);

private:
  NODE_PTR start;
  NODE_PTR goal;

  std::list<NODE_PTR> opList;
  std::list<NODE_PTR> clList;
};