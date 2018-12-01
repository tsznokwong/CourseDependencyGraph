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
            for (Node *root: this->strongRoots) {
                this->parent.updateIsStrongEdge(root->key, this->key, false);
            }
            this->strongRoots.clear();

            bool isPrevious = this->direction == PREVIOUS;
            std::vector<Edge> edges = isPrevious ? this->parent.getPreviousEdgesFrom(this->key):
                                                   this->parent.getNextEdgesFrom(this->key);
            for (Edge edge: edges) {
                edge.getTargetNode()->updateNode(this->depth + (isPrevious ? -1 : 1), this);
            }
        }
        this->strongRoots.append(root);
        this->parent.updateIsStrongEdge(root->key, this->key, true);
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
DependencyGraph<NodePair>::Edge::Edge(DependencyGraph* const parent, Node *targetNode, Direction direction, int type, bool isStrong):
    parent(parent),
    targetNode(targetNode),
    direction(direction),
    isStrong(isStrong),
    type(type) {

}


DependencyGraphTemplate
const DependencyNode*& DependencyGraph<NodePair>::Edge::getTargetNode() const { return this->targetNode; }
DependencyGraphTemplate
const DependencyDirection& DependencyGraph<NodePair>::Edge::getDirection() const { return this->direction; }
DependencyGraphTemplate
const bool& DependencyGraph<NodePair>::Edge::getIsStrong() const { return this->isStrongEdge; }
DependencyGraphTemplate
const int& DependencyGraph<NodePair>::Edge::getType() const { return this->type; }

DependencyGraphTemplate
void DependencyGraph<NodePair>::Edge::setIsStrong(bool isStrongEdge) { this->isStrongEdge = isStrongEdge; }

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
std::vector<DependencyEdge> DependencyGraph<NodePair>::getEdgesFrom(const KeyType &key) const {
    if (!this->contain(key)) { return std::vector<DependencyEdge>(); }
    return this->adjacencyTree.find(key);
}

DependencyGraphTemplate
std::vector<DependencyEdge> DependencyGraph<NodePair>::getPreviousEdgesFrom(const KeyType &key) const {
    std::vector<Edge> edges = this->getEdgesFrom(key);
    for (typename std::vector<Edge>::iterator edgeIterator = edges.begin();
         edgeIterator != edges.end();
         ++edgeIterator) {
        if ((*edgeIterator).direction == PREVIOUS) {
            edgeIterator = edges.erase(edgeIterator);
            --edgeIterator;
        }
    }
    return edges;
}

DependencyGraphTemplate
std::vector<DependencyEdge> DependencyGraph<NodePair>::getNextEdgesFrom(const KeyType &key) const {
    std::vector<Edge> edges = this->getEdgesFrom(key);
    for (typename std::vector<Edge>::iterator edgeIterator = edges.begin(); edgeIterator != edges.end(); ++edgeIterator) {
        if ((*edgeIterator).direction == NEXT) {
            edgeIterator = edges.erase(edgeIterator);
            --edgeIterator;
        }
    }
    return edges;
}

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
    std::vector<Edge> edges = this->getEdgesFrom(fromKey);
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
    std::vector<Edge> edges = this->getEdgesFrom(this->focusNode.key);
    for (Edge edge: edges) {
        edge.getTargetNode()->updateNode(edge.getDirection() == PREVIOUS ? -1 : 1, this->focusNode);
    }
}

DependencyGraphTemplate
bool DependencyGraph<NodePair>::addNode(KeyType key, NodeType node) {
    if (this->contains(key)) { return false; }
    Node newNode { this, key, node };
    this->nodeTree.add(newNode.getKey(), newNode);
    this->adjacencyTree.add(newNode.getKey(), std::vector<Edge>());
    return true;
}

DependencyGraphTemplate
bool DependencyGraph<NodePair>::addEdge(KeyType fromKey, KeyType toKey, Direction direction, int type) {
    if (!this->contains(fromKey) || !this->contains(toKey)) { return false; }

    std::vector<Edge> &edges = this->adjacencyTree.find(fromKey);
    Node *toNode = this->getNode(toKey);

    edges.push_back({ this, toNode, direction, type});
    this->updateNodes();

    return true;
}

DependencyGraphTemplate
void DependencyGraph<NodePair>::reset(KeyType key, NodeType node){
	nodeTree.clear();
	adjacencyTree.clear();

	focusNode = ( this, key, node);
	this->addNode(key, node);
}

