#include <QDebug>
#include "DependencyGraph.h"

#define NodePair KeyType, NodeType
#define DependencyGraphTemplate template<typename KeyType, typename NodeType>
#define DependencyNode typename DependencyGraph<NodePair>::Node
#define DependencyEdge typename DependencyGraph<NodePair>::Edge
#define DependencyDirection typename DependencyGraph<NodePair>::Direction


DependencyGraphTemplate
DependencyGraph<NodePair>::Node::Node(DependencyGraph *parent, KeyType key, NodeType data, int depth):
    parent(parent),
    key(key),
    data(data),
    depth(depth),
    strongRoots(),
    direction(PREVIOUS) {

}

DependencyGraphTemplate
void DependencyGraph<NodePair>::Node::update(int depth, DependencyNode* root) {

    // If current depth of the node is larger than the new value in terms of magnitude, ignore.
    if (abs(depth) < abs(this->depth)) { return; }

    // If current depth of the node is smaller than the new value in terms of magnitude, update new depth.
    if (abs(depth) > abs(this->depth)) {
        this->depth = depth;
        for (Node *root: this->strongRoots) {
            this->parent->updateIsStrongEdge(root->key, this->key, false);
        }
        this->strongRoots.clear();

        bool isPrevious = this->direction == PREVIOUS;
        std::vector<Edge> edges;
        if (isPrevious) {
            this->parent->getPreviousEdgesFrom(this->key, edges);
        } else {
            this->parent->getNextEdgesFrom(this->key, edges);
        }
        // Recursive call to update directed nodes from current node.
        for (Edge edge: edges) {
            edge.getTargetNode()->update(this->depth + (isPrevious ? -1 : 1), this);
        }
    }
    this->strongRoots.push_back(root);
    this->parent->updateIsStrongEdge(root->key, this->key, true);

}

DependencyGraphTemplate
const KeyType& DependencyGraph<NodePair>::Node::getKey() const { return this->key; }
DependencyGraphTemplate
const NodeType& DependencyGraph<NodePair>::Node::getNode() const { return this->data; }
DependencyGraphTemplate
const int& DependencyGraph<NodePair>::Node::getDepth() const { return this->depth; }
DependencyGraphTemplate
const std::vector<DependencyNode*>& DependencyGraph<NodePair>::Node::getStrongRoots() const { return this->strongRoots; }
DependencyGraphTemplate
const DependencyDirection& DependencyGraph<NodePair>::Node::getDirection() const { return this->direction; }

DependencyGraphTemplate
void DependencyGraph<NodePair>::Node::setDepth(int depth) {
    this->depth = depth;
}


DependencyGraphTemplate
DependencyGraph<NodePair>::Edge::Edge(DependencyGraph *parent, Node *targetNode, Direction direction, int type, bool isStrong):
    parent(parent),
    targetNode(targetNode),
    direction(direction),
    isStrong(isStrong),
    type(type) {

}


DependencyGraphTemplate
const DependencyNode* DependencyGraph<NodePair>::Edge::getTargetNode() const { return this->targetNode; }
DependencyGraphTemplate
DependencyNode* DependencyGraph<NodePair>::Edge::getTargetNode(){ return this->targetNode; }
DependencyGraphTemplate
const DependencyDirection& DependencyGraph<NodePair>::Edge::getDirection() const { return this->direction; }
DependencyGraphTemplate
const bool& DependencyGraph<NodePair>::Edge::getIsStrong() const { return this->isStrongEdge; }
DependencyGraphTemplate
const int& DependencyGraph<NodePair>::Edge::getType() const { return this->type; }

DependencyGraphTemplate
void DependencyGraph<NodePair>::Edge::setIsStrong(bool isStrongEdge) { this->isStrong = isStrongEdge; }

DependencyGraphTemplate
DependencyGraph<NodePair>::DependencyGraph(KeyType key, NodeType node):
    focusNode({ this, key, node }) {
    this->addNode(key, node);
}

DependencyGraphTemplate
DependencyNode* DependencyGraph<NodePair>::getNode(const KeyType& key) {
    if (!this->nodeTree.contains(key)) { return nullptr; }
    return &(this->nodeTree.find(key));
}

DependencyGraphTemplate
const AVLTree<KeyType, DependencyNode>& DependencyGraph<NodePair>::getNodes() const { return this->nodes; }

DependencyGraphTemplate
void DependencyGraph<NodePair>::getEdgesFrom(const KeyType &key, std::vector<Edge> &edges) const {
    if (!this->contains(key)) { return; }
    edges = this->adjacencyTree.find(key);
}

DependencyGraphTemplate
void DependencyGraph<NodePair>::getPreviousEdgesFrom(const KeyType &key, std::vector<Edge> &edges) const {
    this->getEdgesFrom(key, edges);
    for (typename std::vector<Edge>::iterator edgeIterator = edges.begin();
         edgeIterator != edges.end();
         ++edgeIterator) {
		if ((*edgeIterator).direction == PREVIOUS) {
            edgeIterator = edges.erase(edgeIterator);
            --edgeIterator;
        }
    }
}

DependencyGraphTemplate
void DependencyGraph<NodePair>::getNextEdgesFrom(const KeyType &key, std::vector<Edge> &edges) const {
        this->getEdgesFrom(key, edges);
    for (typename std::vector<Edge>::iterator edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator) {
        if ((*edgeIterator).direction == NEXT) {
            edgeIterator = edges.erase(edgeIterator);
            --edgeIterator;
        }
    }
}

/**
 * Returning an abstract map of AVLTree of the graph.
 * Key value is the depth in the map.
 * Data value is a vector of nodes in the depth identified in key value.
 */
DependencyGraphTemplate
void DependencyGraph<NodePair>::getNodesInMap(AVLTree<int, std::vector<NodeType> > &map) const {
    std::vector<Node> nodes;
    this->nodeTree.toVector(nodes);

    for (const Node& node: nodes) {
        if (!map.contains(node.getDepth())) {
            map.add(node.getDepth(), std::vector<NodeType>());
        }
        std::vector<NodeType> &nodesInDepth = map.find(node.getDepth());
        nodesInDepth.push_back(node.getNode());
    }

}


DependencyGraphTemplate
void DependencyGraph<NodePair>::updateIsStrongEdge(const KeyType &fromKey, const KeyType &toKey, bool isStrong) {
    std::vector<Edge> edges;
    this->getEdgesFrom(fromKey, edges);
    for (Edge edge: edges) {
        if (edge.getTargetNode()->getKey() == toKey) {
            edge.setIsStrong(isStrong);
            break;
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

DependencyGraphTemplate
void DependencyGraph<NodePair>::updateNodes() {
    std::vector<Edge> edges;
    this->getEdgesFrom(this->focusNode.key, edges);
    for (Edge edge: edges) {
		edge.getTargetNode()->update(edge.getDirection() == PREVIOUS ? -1 : 1, &this->focusNode);
    }
}

DependencyGraphTemplate
bool DependencyGraph<NodePair>::addNode(KeyType key, NodeType node) {
    // If node already exists, add node failed.
    if (this->contains(key)) { return false; }
    Node newNode { this, key, node };
    this->nodeTree.add(newNode.getKey(), newNode);
    this->adjacencyTree.add(newNode.getKey(), std::vector<Edge>());
    return true;
}

DependencyGraphTemplate
bool DependencyGraph<NodePair>::addEdge(KeyType fromKey, KeyType toKey, Direction direction, int type) {
    // If the nodes are not exists, add edge failed.
    if (!this->contains(fromKey) || !this->contains(toKey)) { return false; }

    std::vector<Edge> &edges = this->adjacencyTree.find(fromKey);
    Node *fromNode = this->getNode(fromKey);
    Node *toNode = this->getNode(toKey);
    Edge edge(this, toNode, direction, type);
    edges.push_back(edge);
    toNode->update(fromNode->getDepth() + (direction == PREVIOUS ? -1 : 1), fromNode);
    return true;
}

DependencyGraphTemplate
void DependencyGraph<NodePair>::reset(KeyType key, NodeType node){
	nodeTree.clear();
	adjacencyTree.clear();

	focusNode.key = key;
	focusNode.data = node;
	this->addNode(key, node);
}

