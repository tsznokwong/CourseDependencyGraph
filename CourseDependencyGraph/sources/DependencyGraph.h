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
    private:
        DependencyGraph* const parent;
        KeyType key;
        NodeType data;
        int depth;
        std::vector<Node*> strongRoots;
        Direction direction;
    public:
        Node(DependencyGraph* const parent, KeyType key, NodeType data, int depth = 0);
        void update(int depth, Node* root);
        const KeyType& getKey() const;
        const NodeType& getNode() const;
        const int& getDepth() const;
        const std::vector<Node*>& getStrongRoots() const;
        const Direction& getDirection() const;
    };

    class Edge {
    private:
        DependencyGraph* const parent;
        Node *targetNode;
        Direction direction;
        bool isStrongEdge;
        int type;
    public:
        Edge(DependencyGraph* const parent, Node *targetNode, Direction direction, bool isStrongEdge, int type);
        const Node*& getTargetNode() const;
        const Direction& getDirection() const;
        const bool& getIsStrongEdge() const;
        const int& getType() const;
        void setIsStrongEdge(bool isStrongEdge);
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
    std::vector<Edge> getEdgesFrom(KeyType &key) const;
    std::vector<Edge> getPreviousEdgesFrom(KeyType &key) const;
    std::vector<Edge> getNextEdgesFrom(KeyType &key) const;

    bool contains(const KeyType &key) const;
    unsigned int size() const;
    bool isEmpty() const;

    void updateNodes();

    bool addNode(KeyType key, NodeType node);


};

#endif // DEPENDENCYGRAPH_H
