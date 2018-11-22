#ifndef AVLTREE_H
#define AVLTREE_H
#include <vector>
using namespace std;

template<typename KeyType, typename DataType>
class AVLTree{
private:
	struct AVLNode{
		AVLNode(const KeyType& key, const DataType& data): left(), right(), key(key), data(data), height(0){}
		~AVLNode();

		AVLTree left;
		AVLTree right;
		KeyType key;
		DataType data;
		int height;

		bool operator<(const AVLNode& rhs);
	};
	AVLNode* root = nullptr;

	AVLTree& leftSubtree();
	AVLTree& rightSubtree();
	const AVLTree& leftSubtree() const;
	const AVLTree& rightSubtree() const;
	int height() const;
	int bfactor() const;

	void rotateL();
	void rotateR();
	void balance();
	void updateHeight();

public:
	AVLTree() = default;
	~AVLTree();

	const DataType& max() const;
	const DataType& min() const;
	bool contains() const;
	const DataType& find(const KeyType& key) const;
	void add(const KeyType& key, const DataType& data);
	bool remove(const KeyType& key);
	int size();
	bool empty();
	void toVector(vector<DataType>& vector) const;
};

#endif // AVLTREE_H
