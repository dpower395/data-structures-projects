#include "Graph.h"

#include <iostream>
#include <sstream>

using namespace std;

// fwd declarations
string make_dot(Graph* g);
string what(int& v);

Graph::Graph() {
  directed = false;
}

Graph::~Graph() {
}

vector<shared_ptr<Node>> Graph::getNodes() {
  return nodes;
}

vector<shared_ptr<Edge>> Graph::getEdges() {
  return edges;
}

int Graph::getClock() {
  return clock;
}

void Graph::addNode(shared_ptr<Node> n) {
  nodes.push_back(n);
}

void Graph::addEdge(shared_ptr<Edge> e) {
  edges.push_back(e);
}
 
void Graph::removeNode(shared_ptr<Node> n) {
  for (vector<shared_ptr<Node>>::iterator it = nodes.begin(); it != nodes.end(); it++) {
    if (n == *it) {
      nodes.erase(it);
      break;
    }
  }
}
 
void Graph::removeEdge(shared_ptr<Edge> e) {
  for (vector<shared_ptr<Edge>>::iterator it = edges.begin();
       it != edges.end(); 
       it++) {
    if (e == *it) {
      edges.erase(it);
      break;
    }
  }
}

void Graph::setDirected(bool val) {
  directed = val;
}

bool Graph::isDirected() {
  return directed;
}

set<shared_ptr<Edge>> Graph::getAdjacentEdges(shared_ptr<Node> n) {
  set<shared_ptr<Edge>> ret;
  for (vector<shared_ptr<Edge>>::iterator it = edges.begin(); it != edges.end(); it++) {
    shared_ptr<Edge> edge = *it;
    if (edge->getStart() == n) {
      ret.insert(edge);
    }
    if (!directed && (edge->getEnd() == n)) {
      ret.insert(edge);
    }
  }
  return ret;
}

void Graph::clear() {
    clock = 0;
    for (vector<shared_ptr<Edge>>::iterator it = edges.begin(); it != edges.end(); it++) {
        shared_ptr<Edge> edge = *it;
        edge->setType(UNDISCOVERED_EDGE);
    }
    for (vector<shared_ptr<Node>>::iterator it = nodes.begin(); it != nodes.end(); it++) {
        shared_ptr<Node> n = *it;
        n->clear();
    }
}

void Graph::tick(string message) {
  // optional debugging function, disabled with if(false) to prevent
  // unused var warning. Change this however you want.
  // recommended use: call it just after incrementing the clock.
  if (false) {
    cout << "// " << message << endl << make_dot(this) << endl;
  }
}

void Graph::dfs_recursive(shared_ptr<Node> start) {
    // ensure node is not nullptr
    if (start == nullptr) {return;}
    // mark node as gray, set discovery time, increment clock
    start->setColor(GRAY, getClock());
    clock++;
    // get adjacent edges to node
    set<shared_ptr<Edge>> adj = getAdjacentEdges(start);
    set<shared_ptr<Edge>>::iterator curEdge = adj.begin();
    // for each edge in set:
    for (int i = 0; i < int(adj.size()); i++) {
        // if the node at end of edge is white:
        shared_ptr<Node> new_node = (*curEdge)->getEnd();
        if (new_node->getColor() == WHITE) {
            // set node predecessor
            new_node->setPredecessor(start);
            // dfs(that node)
            dfs_recursive(new_node);
        }
        // increment curEdge
        curEdge++;
    }
    // once for-loop is done, mark node black, completion time, increment clock
    start->setColor(BLACK, getClock());
    clock++;
}

void Graph::dfs(shared_ptr<Node> start) {
    clear();
    dfs_recursive(start);
    // iterate through all edges in graph
    vector<shared_ptr<Edge>> edge_list = getEdges();
    for (int i = 0; i < int(edge_list.size()); i++) {
        shared_ptr<Edge> curEdge = edge_list[i];
        shared_ptr<Node> a = curEdge->getStart();
        int a_d = a->getDisTime();
        int a_f = a->getFinTime();
        shared_ptr<Node> b = curEdge->getEnd();
        int b_d = b->getDisTime();
        int b_f = b->getFinTime();
        // if start node is pred of end node, tree
        if (b->getPre() != nullptr and b->getPre()->getData() == a->getData()) {
            curEdge->setType(TREE_EDGE);
        }
        // else if end node has >d and <f, forward
        else if (b_d > a_d and b_f < a_f) {
            curEdge->setType(FORWARD_EDGE);
        }
        // else if end node has >d >f or <d <f, cross
        else if ((b_d > a_d and b_f > a_f) or (b_d < a_d and b_f < a_f)) {
            curEdge->setType(CROSS_EDGE);
        }
        // else if end node has <d and >f, back
        else if (b_d < a_d and b_f > a_f) {
            curEdge->setType(BACK_EDGE);
        }
    }
}

void Graph::bfs(shared_ptr<Node> start) {
    shared_ptr<Node> fakeNode(new Node("I hope no one thought of this"));
    bfs(start, fakeNode);
}

void Graph::bfs(shared_ptr<Node> start, shared_ptr<Node> finish) {
    // clear the graph
    clear();
    // mark start as gray, update rank, add start to Q
    start->setColor(GRAY, 0);
    start->setRank(0);
    queue<shared_ptr<Node>> Q;
    Q.push(start);
    // while Q is not empty
    while (Q.size() > 0) {
        // u = top, pop Q, u.color = black
        shared_ptr<Node> u = Q.front();
        Q.pop();
        u->setColor(BLACK, 0);
        // for all v_nodes adjacent to u:
        set<shared_ptr<Edge>> adj = getAdjacentEdges(u);
        set<shared_ptr<Edge>>::iterator curEdge = adj.begin();
        for (int i = 0; i < int(adj.size()); i++) {
            shared_ptr<Node> v = (*curEdge)->getEnd();
            if (v->getData() == u->getData()) {
                v = (*curEdge)->getStart();
            }
            // if v is white:
            if (v->getColor() == WHITE) {
                // v.color = gray, v.pre = u, v.rank = u.rank+1, add to Q
                v->setColor(GRAY, 0);
                v->setPredecessor(u);
                v->setRank(u->getRank() + 1);
                Q.push(v);
                // if v == finish, set it to black and exit
                if (v->getData() == finish->getData()) {
                    v->setColor(BLACK, 0);
                    return;
                }
            }
            curEdge++;
        }
    }
}


// overloading operator << lets you put a Graph object into an output
// stream.
ostream &operator << (ostream& out, Graph graph) {
  out << graph.nodes.size() << " Nodes:" << endl;
  out << "[";
  for (vector<shared_ptr<Node>>::iterator it = graph.nodes.begin(); it != graph.nodes.end(); it++) {
    shared_ptr<Node> n = *it;
    out << *n << ", ";
  }
  out << "]" << endl;
  out << graph.edges.size() << " Edges:" << endl;
  out << "[";
  for (vector<shared_ptr<Edge>>::iterator it = graph.edges.begin(); it != graph.edges.end(); it++) {
    shared_ptr<Edge> e = *it;
    out << *e << ", ";
  }
  out << "]";
  return out;
}

// what is a helper function that turns a constant (e.g. WHITE or
// FORWARD_EDGE) into a string for debugging goodness. 
string what(int& v) {
  string ret = "Unknown";
  if (v == WHITE) {
    ret = "white";
  } else if (v == GRAY) {
    ret = "gray";
  } else if (v == BLACK) {
    ret = "black";
  } else if (v == UNDISCOVERED_EDGE) {
    ret = "undiscovered";
  } else if (v == TREE_EDGE) {
    ret = "tree";
  } else if (v == BACK_EDGE) {
    ret = "back";
  } else if (v == FORWARD_EDGE) {
    ret = "forward";
  } else if (v == CROSS_EDGE) {
    ret = "cross";
  }
  return ret;
}

// make_dot generates a dotfile string for viewing your graph. Have
// tick() print out the results of this function, and then use
// http://www.webgraphviz.com/ (or get a copy of GraphViz) to view the
// result. If you're stuck, this can help you get un-stuck.
string make_dot(Graph* g) {
  stringstream ss;
  vector<shared_ptr<Node>> nodes = g->getNodes();
  vector<shared_ptr<Edge>> edges = g->getEdges();
  string gt = "graph";
  string con = "--";
  if (g->isDirected()) {
    con = "->";
    gt = "digraph";
  }
  ss << gt << " homework {" << endl;
  int c, dt, ft, r;
  string textColor = "black";
  for (auto it=nodes.begin(); it != nodes.end(); ++it) {
    shared_ptr<Node> n = *it;
    n->getDiscoveryInformation(c, dt, ft, r);
    if (c == BLACK) {
      textColor = "white";
    } else {
      textColor = "black";
    }
    ss << "  " << n->getData() << " [ style=\"filled\" fontcolor=\"" << textColor << "\" fillcolor=\"" << what(c) << "\"" << " ]" << endl;
  }
  
  string edgeColor = "black";
  for (auto it=edges.begin(); it != edges.end(); ++it) {
    shared_ptr<Edge> e = *it;
    if (e->getType() == TREE_EDGE) {
      edgeColor = "black";
    } else if (e->getType() == FORWARD_EDGE) {
      edgeColor = "purple";
    } else if (e->getType() == BACK_EDGE) {
      edgeColor = "blue";
    } else if (e->getType() == CROSS_EDGE) {
      edgeColor = "green";
    } else {
      edgeColor = "gray";
    }
    ss << "  " << e->getStart()->getData() << con << e->getEnd()->getData() << " [color=\"" << edgeColor << "\"]" << endl;
  }
  ss << "}" << endl;
  return ss.str();
}