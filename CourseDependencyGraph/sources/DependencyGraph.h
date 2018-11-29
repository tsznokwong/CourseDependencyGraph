#ifndef DEPENDENCYGRAPH_H
#define DEPENDENCYGRAPH_H

#include <vector>
#include "AVLTree.tpp"

template <typename KeyType, typename NodeType>
class DependencyGraph {
public:
    enum Direction {
        PREVIOUS, NEXT
    };

private:

    class Node {
        NodeType data;
        int depth;
    };

    class Edge {
        Node targetNode;
        Direction direction;
    };

    AVLTree<KeyType, std::vector<Edge> > adjacencyTree;
    Node focusNode;

public:
    DependencyGraph() = delete;
    DependencyGraph(Node focusNode);

};

#endif // DEPENDENCYGRAPH_H
