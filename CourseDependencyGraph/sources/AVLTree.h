#ifndef AVLTREE_H
#define AVLTREE_H
#include <vector>
#include <functional>
using namespace std;

/***
 * An AVLTree implementation using Desmond's implementation
 * @Typename KeyType  the datatype for the Key object (must have implemented comparison operators)
 * @Typename DataType  the datatype for the data object (must have implemented assignment operator)
 */

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
	unsigned int size = 0;

	AVLTree& leftSubtree();
	AVLTree& rightSubtree();
	const AVLTree& leftSubtree() const;
	const AVLTree& rightSubtree() const;

	int height() const;
	int bfactor() const;

    // AVLTree operation
	void rotateL();
	void rotateR();
	void balance();
	void updateHeight();

public:
	AVLTree() = default;
	~AVLTree();

    // element access
    const DataType& find(const KeyType& key) const;
    DataType& find(const KeyType& key);
	const AVLNode& max() const;
	const AVLNode& min() const;

    // modifier
	bool add(const KeyType& key, const DataType& data);
	bool remove(const KeyType& key);

    // capacity
    bool contains(const KeyType& key) const;
	unsigned int getSize();
	bool empty() const;
    void clear();

    // descriptor
	void toKeyVector(vector<KeyType>& vector) const;
	void toVector(vector<DataType>& vector) const;
    void print(int depth = 0) const;

};

#endif // AVLTREE_H
