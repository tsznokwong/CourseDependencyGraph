#include "AVLTree.h"
#include <iostream>

#define DataPair KeyType, DataType
#define AVLTreeTemplate template<typename KeyType, typename DataType>

AVLTreeTemplate
bool AVLTree<DataPair>::AVLNode::operator<(const AVLTree<DataPair>::AVLNode& rhs){
	return this->key < rhs.key;
}

AVLTreeTemplate
AVLTree<DataPair>::AVLNode::~AVLNode(){

}

AVLTreeTemplate
AVLTree<DataPair>& AVLTree<DataPair>::leftSubtree(){
	return root->left;
}

AVLTreeTemplate
AVLTree<DataPair>& AVLTree<DataPair>::rightSubtree(){
	return root->right;
}

AVLTreeTemplate
const AVLTree<DataPair>& AVLTree<DataPair>::leftSubtree() const{
	return root->left;
}

AVLTreeTemplate
const AVLTree<DataPair>& AVLTree<DataPair>::rightSubtree() const{
	return root->right;
}

AVLTreeTemplate
int AVLTree<DataPair>::height() const{
	if (empty()) return -1;
	return root->height;
}


AVLTreeTemplate
int AVLTree<DataPair>::bfactor() const{
	return rightSubtree().height() - leftSubtree().height();
}

AVLTreeTemplate
void AVLTree<DataPair>::rotateL(){
	AVLNode* rightNode = rightSubtree().root;
	rightSubtree() = rightNode->left;
	size = 1 + leftSubtree().size + rightSubtree().size;
	rightNode->left = *this;
	updateHeight();
	this->root = rightNode;
	size = 1 + leftSubtree().size + rightSubtree().size;
	updateHeight();
}

AVLTreeTemplate
void AVLTree<DataPair>::rotateR(){
	AVLNode* leftNode = leftSubtree().root;
	leftSubtree() = leftNode->right;
	size = 1 + leftSubtree().size + rightSubtree().size;
	leftNode->right = *this;
	updateHeight();
	this->root = leftNode;
	size = 1 + leftSubtree().size + rightSubtree().size;
	updateHeight();
}

AVLTreeTemplate
void AVLTree<DataPair>::balance(){
	if (empty()) return;

	updateHeight();
	int balanceFactor = bfactor();
	if (balanceFactor == 2){
		if (rightSubtree().bfactor() > 0){
			// RR
			rotateL();
			return;
		}
		else{
			// RL
			rightSubtree().rotateR();
			rotateL();
			return;
		}
	}
	else if (balanceFactor == -2){
		if (leftSubtree().bfactor() < 0){
			// LL
			rotateR();
			return;
		}
		else{
			// LR
			leftSubtree().rotateL();
			rotateR();
			return;
		}
	}
}

AVLTreeTemplate
void AVLTree<DataPair>::updateHeight(){
	if (this->empty()){
		return;
	}
	leftSubtree().updateHeight();
	rightSubtree().updateHeight();
	root->height = std::max(leftSubtree().height(), rightSubtree().height()) + 1;
}

AVLTreeTemplate
AVLTree<DataPair>::~AVLTree(){
	delete root;
}

AVLTreeTemplate
const typename AVLTree<DataPair>::AVLNode& AVLTree<DataPair>::max() const{
	if (rightSubtree().empty()) return *root;
	else return rightSubtree().max();
}

AVLTreeTemplate

const typename AVLTree<DataPair>::AVLNode& AVLTree<DataPair>::min() const{
	if (leftSubtree().empty()) return *root;
	else return leftSubtree().min();
}

AVLTreeTemplate
bool AVLTree<DataPair>::contains(const KeyType& key) const{
	if (empty()) return false;
	if (key < root->key){
		return leftSubtree().contains(key);
	}
	if (key > root->key){
		return rightSubtree().contains(key);
	}
	return true;
}

AVLTreeTemplate
const DataType& AVLTree<DataPair>::find(const KeyType& key) const{
	if (key < root->key){
		if (leftSubtree().empty())
			return root->data;
		return leftSubtree().find(key);
	}
	if (key > root->key){
		if (rightSubtree().empty())
			return root->data;
		return rightSubtree().find(key);
	}
	return root->data;
}


AVLTreeTemplate
DataType& AVLTree<DataPair>::find(const KeyType& key) {
    if (key < root->key){
        if (leftSubtree().empty())
            return root->data;
        return leftSubtree().find(key);
    }
    if (key > root->key){
        if (rightSubtree().empty())
            return root->data;
        return rightSubtree().find(key);
    }
    return root->data;
}

AVLTreeTemplate
bool AVLTree<DataPair>::add(const KeyType& key, const DataType& data){
	bool success = false;
	if (this->empty()){
		root = new AVLNode(key, data);
		success = true;
	}
	else if (key < root->key)
		success = leftSubtree().add(key, data);
	else if (key > root->key)
		success = rightSubtree().add(key, data);
	if (success)
		++size;
	balance();
	return success;
}

AVLTreeTemplate
bool AVLTree<DataPair>::remove(const KeyType& key){
	bool success = false;
	if (empty()) return false;
	if (key < root->key) success = leftSubtree().remove(key);
	else if (key > root->key) success = rightSubtree().remove(key);
	else{
		AVLTree& left = leftSubtree();
		AVLTree& right = rightSubtree();
		if (!left.empty() && !right.empty()){
			root->key = rightSubtree().min().key;
			root->data = rightSubtree().min().data;
			success = rightSubtree().remove(root->key);
		}
		else if (left.empty()){
			AVLNode* oldroot = root;
			this->root = rightSubtree().root;
			oldroot->right.root = nullptr;
			delete oldroot;
			success = true;
		}
		else{
			AVLNode* oldroot = root;
			this->root = leftSubtree().root;
			oldroot->left.root = nullptr;
			delete oldroot;
			success = true;
		}
	}
	balance();
	this->size = this->empty()? 0: (leftSubtree().size + rightSubtree().size + 1);
	return success;
}

AVLTreeTemplate
unsigned int AVLTree<DataPair>::getSize(){
	return size;
}

AVLTreeTemplate
bool AVLTree<DataPair>::empty() const{
	return root == nullptr;
}

AVLTreeTemplate
void AVLTree<DataPair>::toKeyVector(vector<KeyType>& vector) const{
	if (!leftSubtree().empty()){
		leftSubtree().toKeyVector(vector);
	}
	vector.push_back(root->key);
	if (!rightSubtree().empty()){
		rightSubtree().toKeyVector(vector);
	}
}

AVLTreeTemplate
void AVLTree<DataPair>::toVector(vector<DataType>& vector) const{
    if (empty()) return;
    if (!leftSubtree().empty()){
            leftSubtree().toVector(vector);
    }
    vector.push_back(root->data);
    if (!rightSubtree().empty()){
            rightSubtree().toVector(vector);
    }
}

AVLTreeTemplate
void AVLTree<DataPair>::print(int depth) const{
	if (empty()) return;
	rightSubtree().print(depth+1);
	for (int i = 0; i < depth; ++i){
		cout << '\t';
	}
	cout << root->data << endl;
	leftSubtree().print(depth+1);
}

AVLTreeTemplate
void AVLTree<DataPair>::clear(){
	delete root;
	root = nullptr;
}

