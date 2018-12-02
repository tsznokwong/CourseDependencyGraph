#ifndef DEPENDENCYGRAPH_H
#define DEPENDENCYGRAPH_H

#include <vector>
#include <set>
#include "AVLTree.tpp"

/***
 * The DependencyGraph is a directed unweighted graph.
 * The graph has a focus node which must be specified during construction.
 * The graph is implemented with abstract adjacency list and set of nodes with AVLTree.
 * @Typename KeyType  the datatype for the Key object (must have implemented comparison operators)
 * @Typename NodeType  the datatype for the data mode object (must have implemented assignment operator)
 */
template <typename KeyType, typename NodeType>
class DependencyGraph {
public:
    /**
     * @brief The Direction enum.
     * PREVIOUS: Represent the initial node depends on the terminating node.
     * NEXT: Represent the terminating node depends on the initial node.
     */
    enum Direction {
        PREVIOUS, NEXT
    };
    /**
     * @brief The Node class is the vertex representation of the graph.
     * Each node has a key to identify and storing data.
     * Each node has a depth defined by the longest distance from focus node.
     * i.e. negative represents the focus node depends on the node.
     *      positive represents the node depends on the focus node.
     * Each node points to its initial nodes in the graph which are roots contributed to largest magnitude of its depth.
     */
    class Node {
    private:
        DependencyGraph* parent;
        KeyType key;
        NodeType data;
        int depth;
        std::vector<Node*> strongRoots;
        Direction direction;

    public:
        Node(DependencyGraph* parent, KeyType key, NodeType data, int depth = 0);

        // getter
        const KeyType& getKey() const;
        const NodeType& getNode() const;
        const int& getDepth() const;
        const std::vector<Node*>& getStrongRoots() const;
        const Direction& getDirection() const;

        // setter
        void setDepth(int depth);

        // Graph operation
        void update(int depth, Node* root);

		friend DependencyGraph;
    };

    /**
     * @brief The Edge class is the edge representation of the graph.
     * Each edge has a target node which is the terminating node of edge.
     * The edge is strong if the edge contributed to largest magnitude of depth of the target node.
     */
    class Edge {
    private:
        DependencyGraph* parent;
        Node *targetNode;
        Direction direction;
        bool isStrong;
        int type;

    public:
        Edge(DependencyGraph* parent, Node *targetNode, Direction direction, int type, bool isStrong = false);
        Edge() = default;
        Edge(const Edge& edge) = default;

        // getter
		const Node* getTargetNode() const;
		Node* getTargetNode();
        const Direction& getDirection() const;
        const bool& getIsStrong() const;
        const int& getType() const;

        // setter
        void setIsStrong(bool isStrong);

		friend DependencyGraph;

		Edge& operator=(Edge&& rhs) = default;
        Edge& operator=(const Edge &rhs) = default;
    };

private:

    // Graph elements
    AVLTree<KeyType, std::vector<Edge> > adjacencyTree;
    AVLTree<KeyType, Node> nodeTree;
    Node focusNode;

    void updateIsStrongEdge(const KeyType &fromKey, const KeyType &toKey, bool isStrong);

public:
    DependencyGraph() = delete;
    DependencyGraph(KeyType key, NodeType node);

    // getter
    Node* getNode(const KeyType &key);
    const AVLTree<KeyType, Node>& getNodes() const;
    void getEdgesFrom(const KeyType &key, std::vector<Edge> &edges) const;
    void getPreviousEdgesFrom(const KeyType &key, std::vector<Edge> &edges) const;
    void getNextEdgesFrom(const KeyType &key, std::vector<Edge> &edges) const;
    void getNodesInMap(AVLTree<int, std::vector<NodeType> > &map) const;

    // capacity
    bool contains(const KeyType &key) const;
    unsigned int size() const;
    bool isEmpty() const;
    void reset(KeyType key, NodeType node);

    // Graph operations
    void updateNodes();
    bool addNode(KeyType key, NodeType node);
    bool addEdge(KeyType fromKey, KeyType toKey, Direction direction, int type);

};

#endif // DEPENDENCYGRAPH_H
