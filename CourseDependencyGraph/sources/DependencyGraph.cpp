#include <QDebug>
#include "DependencyGraph.h"

#define NodePair KeyType, NodeType
#define DependencyGraphTemplate template<typename KeyType, typename NodeType>

DependencyGraphTemplate
DependencyGraph<NodePair>::DependencyGraph(KeyType key, NodeType node):
    focusNode({ key, node, 0 }) {
    this->nodes.insert(focusNode);
    this->adjacencyTree.add(focusNode.key, std::vector<Edge>());
}

DependencyGraphTemplate
bool DependencyGraph<NodePair>::addNode(KeyType key, NodeType node) {
    if (this->adjacencyTree.contains(key)) { return false; }
    Node newNode { key, node };
    this->nodes.insert(newNode);
    this->adjacencyTree.add(newNode.key, std::vector<Edge>());
}
