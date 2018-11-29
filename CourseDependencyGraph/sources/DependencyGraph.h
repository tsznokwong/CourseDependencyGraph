#ifndef DEPENDENCYGRAPH_H
#define DEPENDENCYGRAPH_H

#include <vector>
#include <set>
#include "AVLTree.tpp"

template <typename KeyType, typename NodeType>
class DependencyGraph {
public:
    enum Direction {
        PREVIOUS, NEXT
    };

    class Node {
        KeyType key;
        NodeType data;
        int depth;
    };

    class Edge {
        Node *targetNode;
        Direction direction;
        bool strongEdge;
        int type;
    };

private:

    AVLTree<KeyType, std::vector<Edge> > adjacencyTree;
    std::set<Node> nodes;
    Node focusNode;

public:
    DependencyGraph() = delete;
    DependencyGraph(KeyType key, NodeType node);

    // getter
    const std::set<Node>& getNodes() const;
    std::vector<Edge> getEdgesFrom(KeyType key) const;
    std::vector<Edge> getPreviousEdgesFrom(KeyType key) const;
    std::vector<Edge> getNextEdgesFrom(KeyType key) const;

    bool addNode(KeyType key, NodeType node);


};

#endif // DEPENDENCYGRAPH_H
