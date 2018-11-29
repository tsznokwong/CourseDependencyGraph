#include <QDebug>
#include "DependencyGraph.h"

#define NodePair KeyType, NodeType
#define DependencyGraphTemplate template<typename KeyType, typename NodeType>
#define DependencyNode typename DependencyGraph<NodePair>::Node
#define DependencyEdge typename DependencyGraph<NodePair>::Edge
#define DependencyDirection typename DependencyGraph<NodePair>::Direction


DependencyGraphTemplate
DependencyGraph<NodePair>::Node::Node(DependencyGraph * const parent, KeyType key, NodeType data, int depth):
    parent(parent),
    key(key),
    data(data),
    depth(depth),
    strongRoots(),
    direction(PREVIOUS) {

}

DependencyGraphTemplate
void DependencyGraph<NodePair>::Node::update(int depth, DependencyNode* root) {
    if (depth <= this->depth) {
        if (depth > this->depth) {
            this->depth = depth;
            this->strongRoots.clear();

            bool isPrevious = this->direction == PREVIOUS;
            std::vector<Edge> edges = isPrevious ? this->parent.getPreviousEdgesFrom(this->key):
                                                   this->parent.getNextEdgesFrom(this->key);
            for (Edge edge: edges) {
                edge.getTargetNode()->updateNode(this->depth + (isPrevious ? -1 : 1), this);
            }
        }
        this->strongRoots.append(root);
    }
}

DependencyGraphTemplate
const KeyType& DependencyGraph<NodePair>::Node::getKey() const { return this->key; }
DependencyGraphTemplate
const NodeType& DependencyGraph<NodePair>::Node::getNode() const { return this->node; }
DependencyGraphTemplate
const int& DependencyGraph<NodePair>::Node::getDepth() const { return this->depth; }
DependencyGraphTemplate
const std::vector<DependencyNode*>& DependencyGraph<NodePair>::Node::getStrongRoots() const { return this->strongRoots; }
DependencyGraphTemplate
const DependencyDirection& DependencyGraph<NodePair>::Node::getDirection() const { return this->direction; }



DependencyGraphTemplate
const DependencyNode*& DependencyGraph<NodePair>::Edge::getTargetNode() const { return this->targetNode; }
DependencyGraphTemplate
const DependencyDirection& DependencyGraph<NodePair>::Edge::getDirection() const { return this->direction; }
DependencyGraphTemplate
const bool& DependencyGraph<NodePair>::Edge::getIsStrongEdge() const { return this->isStrongEdge; }
DependencyGraphTemplate
const int& DependencyGraph<NodePair>::Edge::getType() const { return this->type; }


DependencyGraphTemplate
DependencyGraph<NodePair>::DependencyGraph(KeyType key, NodeType node):
    focusNode({ this, key, node }) {
    this->addNode({ this, key, node });
}

DependencyGraphTemplate
const std::set<DependencyNode>& DependencyGraph<NodePair>::getNodes() const { return this->nodes; }

DependencyGraphTemplate
std::vector<DependencyEdge> DependencyGraph<NodePair>::getEdgesFrom(KeyType &key) const {
    if (!this->contain(key)) { return std::vector<DependencyEdge>(); }
    return this->adjacencyTree.find(key);
}

DependencyGraphTemplate
std::vector<DependencyEdge> DependencyGraph<NodePair>::getPreviousEdgesFrom(KeyType &key) const {
    std::vector<DependencyEdge> edges = this->getEdgesFrom(key);
    for (typename std::vector<DependencyEdge>::iterator edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator) {
        if ((*edgeIterator).direction == PREVIOUS) {
            edgeIterator = edges.erase(edgeIterator);
            --edgeIterator;
        }
    }
}

DependencyGraphTemplate
std::vector<DependencyEdge> DependencyGraph<NodePair>::getNextEdgesFrom(KeyType &key) const {
    std::vector<DependencyEdge> edges = this->getEdgesFrom(key);
    for (typename std::vector<DependencyEdge>::iterator edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator) {
        if ((*edgeIterator).direction == NEXT) {
            edgeIterator = edges.erase(edgeIterator);
            --edgeIterator;
        }
    }
}

DependencyGraphTemplate
bool DependencyGraph<NodePair>::contains(const KeyType &key) const {
    return this->adjacencyTree.contains(key);
}

DependencyGraphTemplate
unsigned int DependencyGraph<NodePair>::size() const {
    return this->adjacencyTree.size();
}

DependencyGraphTemplate
bool DependencyGraph<NodePair>::isEmpty() const {
    return this->size() == 0;
}

    std::vector<Edge> edges = this->getEdgesFrom(this->focusNode.key);
    for (Edge edge: edges) {
        edge.getTargetNode()->updateNode(edge.getDirection() == PREVIOUS ? -1 : 1, this->focusNode);
    }
}

DependencyGraphTemplate
bool DependencyGraph<NodePair>::addNode(KeyType key, NodeType node) {
    if (this->contains(key)) { return false; }
    Node newNode { this, key, node };
    this->nodes.insert(newNode);
    this->adjacencyTree.add(newNode.getKey(), std::vector<Edge>());
    return true;
}
}

