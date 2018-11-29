#include <QDebug>
#include "DependencyGraph.h"

#define NodePair KeyType, NodeType
#define DependencyGraphTemplate template<typename KeyType, typename NodeType>
#define DependencyNode typename DependencyGraph<NodePair>::Node
#define DependencyEdge typename DependencyGraph<NodePair>::Edge
#define DependencyDirection DependencyGraph<NodePair>::Direction

DependencyGraphTemplate
DependencyGraph<NodePair>::DependencyGraph(KeyType key, NodeType node):
    focusNode({ key, node, 0 }) {
    this->nodes.insert(focusNode);
    this->adjacencyTree.add(focusNode.key, std::vector<Edge>());
}

DependencyGraphTemplate
const std::set<DependencyNode>& DependencyGraph<NodePair>::getNodes() const {
    return this->nodes;
}

DependencyGraphTemplate
std::vector<DependencyEdge> DependencyGraph<NodePair>::getEdgesFrom(KeyType key) const {
    if (!this->adjacencyTree.contain(key)) { return std::vector<DependencyEdge>(); }
    return this->adjacencyTree.find(key);
}

DependencyGraphTemplate
std::vector<DependencyEdge> DependencyGraph<NodePair>::getPreviousEdgesFrom(KeyType key) const {
    std::vector<DependencyEdge> edges = this->getEdgesFrom(key);
    for (typename std::vector<DependencyEdge>::iterator edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator) {
        if ((*edgeIterator).direction == DependencyDirection::PREVIOUS) {
            edgeIterator = edges.erase(edgeIterator);
            --edgeIterator;
        }
    }
}

DependencyGraphTemplate
std::vector<DependencyEdge> DependencyGraph<NodePair>::getNextEdgesFrom(KeyType key) const {
    std::vector<DependencyEdge> edges = this->getEdgesFrom(key);
    for (typename std::vector<DependencyEdge>::iterator edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator) {
        if ((*edgeIterator).direction == DependencyDirection::NEXT) {
            edgeIterator = edges.erase(edgeIterator);
            --edgeIterator;
        }
    }
}


DependencyGraphTemplate
bool DependencyGraph<NodePair>::addNode(KeyType key, NodeType node) {
    if (this->adjacencyTree.contains(key)) { return false; }
    Node newNode { key, node };
    this->nodes.insert(newNode);
    this->adjacencyTree.add(newNode.key, std::vector<Edge>());
    return true;
}
}

