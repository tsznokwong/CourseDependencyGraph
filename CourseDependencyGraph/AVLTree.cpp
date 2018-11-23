#include "AVLTree.h"

#define DataPair KeyType, DataType
#define AVLTreeTemplate template<typename KeyType, typename DataType>


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
	return leftSubtree();
}

AVLTreeTemplate
const AVLTree<DataPair>& AVLTree<DataPair>::rightSubtree() const{
	return rightSubtree();
}

AVLTreeTemplate
int AVLTree<DataPair>::height() const{
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
	rightNode->left = *this;
	updateHeight();
	this->root = rightNode;
	updateHeight();
}

AVLTreeTemplate
void AVLTree<DataPair>::rotateR(){
	AVLNode* leftNode = leftSubtree().root;
	leftSubtree() = leftNode->right;
	leftNode->left = *this;
	updateHeight();
	this->root = leftNode;
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
	updateHeight(leftSubtree());
	updateHeight(rightSubtree());
	this->height() = std::max(leftSubtree().height(), rightSubtree().height()) + 1;
}

AVLTreeTemplate
AVLTree<DataPair>::~AVLTree<DataPair>(){
	delete root;
}

AVLTreeTemplate
const DataType& AVLTree<DataPair>::max() const{
	if (rightSubtree().empty()) return root->data;
	else return rightSubtree().max();
}

AVLTreeTemplate
const DataType& AVLTree<DataPair>::min() const{
	if (leftSubtree().empty()) return root->data;
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
const typename AVLTree<DataPair>::AVLTreeFindType& AVLTree<DataPair>::find(const KeyType& key) const{
	if (key < root->key){
		return leftSubtree().find(key);
	}
	if (key > root->key){
		return rightSubtree().find(key);
	}
	return root->data;
}

AVLTreeTemplate
void AVLTree<DataPair>::add(const KeyType& key, const DataType& data){

}

AVLTreeTemplate
bool AVLTree<DataPair>::remove(const KeyType& key){

}

AVLTreeTemplate
unsigned int AVLTree<DataPair>::getSize(){
	return size;
}

AVLTreeTemplate
bool AVLTree<DataPair>::empty(){
	return size == 0;
}

AVLTreeTemplate
void AVLTree<DataPair>::toVector(vector<DataType>& vector) const{
	if (!leftSubtree().empty()){
		leftSubtree().toVector(vector);
	}
	vector.push_back(root->data);
	if (!rightSubtree().empty()){
		rightSubtree().toVector(vector);
	}
}
