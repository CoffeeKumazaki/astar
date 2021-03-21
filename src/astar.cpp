#include <stdafx.hpp>
#include <astar.hpp>

using namespace std;

Astar::Astar() {

}

Astar::~Astar() {

}

void Astar::setStartGoal(NODE_PTR _start, NODE_PTR _goal){

  start = _start;
  goal = _goal;

  start->g = 0;
  start->h = start->estimate_cost(goal);
  start->f = start->g + start->h;
  start->parent = NULL;

  opList.push_back(start);
}

int Astar::findPath(std::list<NODE_PTR>& route) {

  while (!opList.empty()) {

    if (findGoal()) {
      break;
    }
    step();
  }

  return makeRoute(route);
}

void Astar::step() {

  NODE_PTR trg = opList.front();
  opList.pop_front();

  for (size_t i = 0; i < trg->child.size(); i++) {
    NODE_PTR chld = trg->child[i].first;
    double cost = trg->child[i].second;

    chld->g = trg->g + cost;
    chld->h = chld->estimate_cost(goal);
    chld->f = chld->g + chld->h;

    auto opit = find(opList.begin(), opList.end(), chld);
    auto clit = find(clList.begin(), clList.end(), chld);

    if (clit != clList.end() && (*clit)->f > chld->f) {
      chld->parent = trg;
      clList.erase(clit);
      opList.push_back(chld);
    }

    if (opit == opList.end()) {
      opList.push_back(chld);
      chld->parent = trg;
    }

  }

  opList.sort();
}


bool Astar::findGoal() {

  return goal == opList.front();
}

int Astar::makeRoute(std::list<NODE_PTR> &route) {

  route.clear();

  if (findGoal()) {
    NODE_PTR p = goal;
    while (p != start) {
      route.push_front(p);
      p = p->parent;
    }
  }

  return route.size();
}
