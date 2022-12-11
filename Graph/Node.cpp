#include "Node.h"

Node::Node(string s) {
  data = s;
}

Node::~Node() {
}

string Node::getData() {
    return data;
}

void Node::setRank(int r) {
    rank = r;
}

int Node::getRank() {
    return rank;
}

void Node::setData(string s) {
    data = s;
}

void Node::clear() {
    color = WHITE;
    discovery_time = -1;
    completion_time = -1;
    rank = -1;
    predecessor = nullptr;
}

void Node::setColor(int search_color, int time) {
    if (search_color == WHITE) {
        clear();
        return;
    }
    if (search_color == GRAY) {
        color = GRAY;
        discovery_time = time;
        return;
    }
    if (search_color == BLACK) {
        color = BLACK;
        completion_time = time;
        return;
    }
}

int Node::getColor() {
    return color;
}

int Node::getDisTime() {
    return discovery_time;
}

int Node::getFinTime() {
    return completion_time;
}

shared_ptr<Node> Node::getPre() {
    return predecessor;
}

void Node::getDiscoveryInformation(int& thecolor, int& disco_time, 
				   int& finish_time, int& bfs_rank) {
    
    thecolor = color;
    disco_time = discovery_time;
    finish_time = completion_time;
    bfs_rank = rank;
}

bool Node::isAncestor(shared_ptr<Node> other) {
    if (predecessor == nullptr or other == nullptr) {return false;}
    if (predecessor->data == other->data) {return true;}
    bool is_it = predecessor->isAncestor(other);
    return is_it;
}

void Node::setPredecessor(shared_ptr<Node> other) {
    predecessor = other;
}

ostream &operator << (std::ostream& out, Node node) {
  out << node.data;
  return out;
}
