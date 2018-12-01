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
        bool isStrong;
        int type;
    public:
        Edge(DependencyGraph* const parent, Node *targetNode, Direction direction, int type, bool isStrong = false);
        const Node*& getTargetNode() const;
        const Direction& getDirection() const;
        const bool& getIsStrong() const;
        const int& getType() const;
        void setIsStrong(bool isStrong);
    };

private:

    AVLTree<KeyType, std::vector<Edge> > adjacencyTree;
    AVLTree<KeyType, Node> nodeTree;
    Node focusNode;

    void updateIsStrongEdge(const KeyType &fromKey, const KeyType &toKey, bool isStrong);
    Node* getNode(const KeyType &key);

public:
    DependencyGraph() = delete;
    DependencyGraph(KeyType key, NodeType node);

    // getter
    const AVLTree<KeyType, Node>& getNodes() const;
    std::vector<Edge> getEdgesFrom(const KeyType &key) const;
    std::vector<Edge> getPreviousEdgesFrom(const KeyType &key) const;
    std::vector<Edge> getNextEdgesFrom(const KeyType &key) const;
    void getNodesInMap(AVLTree<int, std::vector<NodeType> > &map) const;

    bool contains(const KeyType &key) const;
    unsigned int size() const;
    bool isEmpty() const;

    void updateNodes();
	void reset(KeyType key, NodeType node);

    bool addNode(KeyType key, NodeType node);
    bool addEdge(KeyType fromKey, KeyType toKey, Direction direction, int type);



};

#endif // DEPENDENCYGRAPH_H
