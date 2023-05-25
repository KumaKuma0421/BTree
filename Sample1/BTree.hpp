// ---------------------------------------------------------------------------------------
// BTree.h
//
// @sa https://www.geeksforgeeks.org/delete-operation-in-b-tree/
// ---------------------------------------------------------------------------------------

#pragma once

#include <iostream>

template<class T>
class BTree;

template<class T>
class BTreeNode
{
public:
	//! @brief コンストラクタ
	//! @param degree 
	//! @param leaf 葉の場合(true) 枝の場合(false)
	BTreeNode (int degree, bool leaf)
	{
		_minDegree = degree;
		_isLeaf = leaf;

		// 事前に_keysと_childの領域を確保します。
		_keys = new T[2 * _minDegree - 1];
		_childs = new BTreeNode<T> *[2 * _minDegree];

		// keysの格納数を初期化します。
		_number = 0;
	}

	//! @brief デストラクタ
	~BTreeNode ()
	{
		delete[] _keys;
		delete[] _childs;
	}

	//! @brief B-Treeの内容をstd::coutに列挙します。
	void traverse ()
	{
		// There are n keys and n+1 children, traverse through n keys
		// and first n children
		int i;
		for (i = 0; i < _number; i++)
		{
			// If this is not leaf, then before printing key[i],
			// traverse the subtree rooted with _childs[i].
			if (_isLeaf == false)
				_childs[i]->traverse ();
			std::cout << " " << _keys[i];
		}

		// Print the subtree rooted with last child
		if (_isLeaf == false)
			_childs[i]->traverse ();
	}

	//! @brief B-Treeから指定のキーを検索します。
	//! @param key 検索キー
	//! @return 検索結果のT*
	T* search (T key) // returns nullptr if key is not present.
	{
		// Find the first key greater than or equal to k
		int i = 0;
		while (i < _number && key > _keys[i])
			i++;

		// If the found key is equal to k, return this node
		if (_keys[i] == key)
			return &_keys[i];

		// If key is not found here and this is a leaf node
		if (_isLeaf == true)
			return nullptr;

		// Go to the appropriate child
		return _childs[i]->search (key);
	}

	//! @brief keyが格納されているインデックスを検索します。
	//! @param key 検索キー
	//! @return B-Treeの格納位置
	int findKey (T key)
	{
		int idx = 0;
		while (idx < _number && _keys[idx] < key)
			++idx;

		return idx;
	}

	//! @brief まだ満杯でないBTreeNodeにkeyを格納します。
	//! @param key 格納キー
	void insertNonFull (T key)
	{
		// Initialize index as index of rightmost element
		int i = _number - 1;

		// If this is a leaf node
		if (_isLeaf == true)
		{
			// The following loop does two things
			// a) Finds the location of new key to be inserted
			// b) Moves all greater keys to one place ahead
			while (i >= 0 && _keys[i] > key)
			{
				_keys[i + 1] = _keys[i];
				i--;
			}

			// Insert the new key at found location
			_keys[i + 1] = key;
			_number = _number + 1;
		}
		else // If this node is not leaf
		{
			// Find the child which is going to have the new key
			while (i >= 0 && _keys[i] > key)
				i--;

			// See if the found child is full
			if (_childs[i + 1]->_number == 2 * _minDegree - 1)
			{
				// If the child is full, then split it
				splitChild (i + 1, _childs[i + 1]);

				// After split, the middle key of _childs[i] goes up and
				// _childs[i] is splitted into two. See which of the two
				// is going to have the new key
				if (_keys[i + 1] < key)
					i++;
			}
			_childs[i + 1]->insertNonFull (key);
		}
	}

	//! @brief B-TreeNodeを分割します。
	//! @param idx インデックス
	//! @param node 分割対象のBTreeNode
	void splitChild (int idx, BTreeNode<T>* node)
	{
		// Create a new node which is going to store (t-1) keys
		// of y
		BTreeNode<T>* z = new BTreeNode<T> (node->_minDegree, node->_isLeaf);
		z->_number = _minDegree - 1;

		// Copy the last (t-1) keys of y to z
		for (int j = 0; j < _minDegree - 1; j++)
			z->_keys[j] = node->_keys[j + _minDegree];

		// Copy the last t children of y to z
		if (node->_isLeaf == false)
		{
			for (int j = 0; j < _minDegree; j++)
			{
				z->_childs[j] = node->_childs[j + _minDegree];
			}
		}

		// Reduce the number of keys in y
		node->_number = _minDegree - 1;

		// Since this node is going to have a new child,
		// create space of new child
		for (int j = _number; j >= idx + 1; j--)
		{
			_childs[j + 1] = _childs[j];
		}

		// Link the new child to this node
		_childs[idx + 1] = z;

		// A key of y will move to this node. Find location of
		// new key and move all greater keys one space ahead
		for (int j = _number - 1; j >= idx; j--)
			_keys[j + 1] = _keys[j];

		// Copy the middle key of y to this node
		_keys[idx] = node->_keys[_minDegree - 1];

		// Increment count of keys in this node
		_number = _number + 1;
	}

	//! @brief B-Treeから指定のキーを削除します。
	//! @param key 削除するキー
	//! @return 削除結果
	//! @retval true(削除を行った)
	//! @retval false(削除対象がなかった)
	bool remove (T key)
	{
		bool ret = true;
		int idx = findKey (key);

		// The key to be removed is present in this node
		if (idx < _number && _keys[idx] == key)
		{
			// If the node is a leaf node - removeFromLeaf is called
			// Otherwise, removeFromNonLeaf function is called
			if (_isLeaf)
				removeFromLeaf (idx);
			else
				ret = removeFromNonLeaf (idx);
		}
		else
		{
			// If this node is a leaf node, then the key is not present in tree
			if (_isLeaf)
			{
				std::cout << "The key " << key << " is does not exist in the tree" << std::endl;
				return false;
			}

			// The key to be removed is present in the sub-tree rooted with this node
			// The flag indicates whether the key is present in the sub-tree rooted
			// with the last child of this node
			bool flag = ((idx == _number) ? true : false);

			// If the child where the key is supposed to exist has less that t keys,
			// we fill that child
			if (_childs[idx]->_number < _minDegree)
				fill (idx);

			// If the last child has been merged, it must have merged with the previous
			// child and so we recurse on the (idx-1)th child. Else, we recurse on the
			// (idx)th child which now has atleast t keys
			if (flag && idx > _number)
				ret = _childs[idx - 1]->remove (key);
			else
				ret = _childs[idx]->remove (key);
		}

		return ret;
	}

	//! @brief 葉からデータを削除(前詰め)します。
	//! @param idx 削除対象の位置
	void removeFromLeaf (int idx)
	{
		// Move all the keys after the idx-th pos one place backward
		for (int i = idx + 1; i < _number; ++i)
			_keys[i - 1] = _keys[i];

		// Reduce the count of keys
		_number--;
	}

	//! @brief 枝からデータを削除します。
	//! @param idx 削除対象の位置
	bool removeFromNonLeaf (int idx)
	{
		T k = _keys[idx];
		bool ret;

		// If the child that precedes k (_childs[idx]) has atleast t keys,
		// find the predecessor 'pred' of k in the subtree rooted at
		// _childs[idx]. Replace k by pred. Recursively delete pred
		// in _childs[idx]
		if (_childs[idx]->_number >= _minDegree)
		{
			T pred = getPred (idx);
			_keys[idx] = pred;
			ret = _childs[idx]->remove (pred);
		}

		// If the _childs[idx] has less that t keys, examine _childs[idx+1].
		// If _childs[idx+1] has atleast t keys, find the successor 'succ' of k in
		// the subtree rooted at _childs[idx+1]
		// Replace k by succ
		// Recursively delete succ in _childs[idx+1]
		else if (_childs[idx + 1]->_number >= _minDegree)
		{
			T succ = getSucc (idx);
			_keys[idx] = succ;
			ret = _childs[idx + 1]->remove (succ);
		}

		// If both _childs[idx] and _childs[idx+1] has less that t keys,merge k
		// and all of _childs[idx+1] into _childs[idx]
		// Now _childs[idx] contains 2t-1 keys
		// Free _childs[idx+1] and recursively delete k from _childs[idx]
		else
		{
			merge (idx);
			ret = _childs[idx]->remove (k);
		}

		return ret;
	}

	// A function to get predecessor of keys[idx]
	T getPred (int idx)
	{
		// Keep moving to the right most node until we reach a leaf
		BTreeNode<T>* cur = _childs[idx];
		while (!cur->_isLeaf)
			cur = cur->_childs[cur->_number];

		// Return the last key of the leaf
		return cur->_keys[cur->_number - 1];
	}

	// A function to get the successor of the key- where the key
	// is present in the idx-th position in the node
	T getSucc (int idx)
	{
		// Keep moving the left most node starting from _childs[idx+1] until we reach a leaf
		BTreeNode<T>* cur = _childs[idx + 1];
		while (!cur->_isLeaf)
			cur = cur->_childs[0];

		// Return the first key of the leaf
		return cur->_keys[0];
	}

	// A function to fill _childs[idx] which has less than t-1 keys
	void fill (int idx)
	{
		// If the previous _childs[idx-1] has more than t-1 keys, borrow a key
		// from that child
		if (idx != 0 && _childs[idx - 1]->_number >= _minDegree)
			borrowFromPrev (idx);

		// If the next _childs[idx+1] has more than t-1 keys, borrow a key
		// from that child
		else if (idx != _number && _childs[idx + 1]->_number >= _minDegree)
			borrowFromNext (idx);

		// Merge _childs[idx] with its sibling
		// If _childs[idx] is the last child, merge it with its previous sibling
		// Otherwise merge it with its next sibling
		else
		{
			if (idx != _number)
				merge (idx);
			else
				merge (idx - 1);
		}
	}

	// A function to borrow a key from _childs[idx-1] and insert it into _childs[idx]
	void borrowFromPrev (int idx)
	{
		BTreeNode<T>* child = _childs[idx];
		BTreeNode<T>* sibling = _childs[idx - 1];

		// The last key from _childs[idx-1] goes up to the parent and key[idx-1]
		// from parent is inserted as the first key in _childs[idx]. Thus, the loses
		// sibling one key and child gains one key

		// Moving all key in _childs[idx] one step ahead
		for (int i = child->_number - 1; i >= 0; --i)
			child->_keys[i + 1] = child->_keys[i];

		// If _childs[idx] is not a leaf, move all its child pointers one step ahead
		if (!child->_isLeaf)
		{
			for (int i = child->_number; i >= 0; --i)
				child->_childs[i + 1] = child->_childs[i];
		}

		// Setting child's first key equal to keys[idx-1] from the current node
		child->_keys[0] = _keys[idx - 1];

		// Moving sibling's last child as _childs[idx]'s first child
		if (!child->_isLeaf)
			child->_childs[0] = sibling->_childs[sibling->_number];

		// Moving the key from the sibling to the parent
		// This reduces the number of keys in the sibling
		_keys[idx - 1] = sibling->_keys[sibling->_number - 1];

		child->_number += 1;
		sibling->_number -= 1;
	}

	// A function to borrow a key from the _childs[idx+1] and place it in _childs[idx]
	void borrowFromNext (int idx)
	{
		BTreeNode<T>* child = _childs[idx];
		BTreeNode<T>* sibling = _childs[idx + 1];

		// keys[idx] is inserted as the last key in _childs[idx]
		child->_keys[(child->_number)] = _keys[idx];

		// Sibling's first child is inserted as the last child
		// into _childs[idx]
		if (!(child->_isLeaf))
			child->_childs[(child->_number) + 1] = sibling->_childs[0];

		//The first key from sibling is inserted into keys[idx]
		_keys[idx] = sibling->_keys[0];

		// Moving all keys in sibling one step behind
		for (int i = 1; i < sibling->_number; ++i)
			sibling->_keys[i - 1] = sibling->_keys[i];

		// Moving the child pointers one step behind
		if (!sibling->_isLeaf)
		{
			for (int i = 1; i <= sibling->_number; ++i)
				sibling->_childs[i - 1] = sibling->_childs[i];
		}

		// Increasing and decreasing the key count of _childs[idx] and _childs[idx+1]
		// respectively
		child->_number += 1;
		sibling->_number -= 1;
	}

	// A function to merge _childs[idx] with _childs[idx+1]
	// _childs[idx+1] is freed after merging
	void merge (int idx)
	{
		BTreeNode<T>* child = _childs[idx];
		BTreeNode<T>* sibling = _childs[idx + 1];

		// Pulling a key from the current node and inserting it into (t-1)th
		// position of _childs[idx]
		child->_keys[_minDegree - 1] = _keys[idx];

		// Copying the keys from _childs[idx+1] to _childs[idx] at the end
		for (int i = 0; i < sibling->_number; ++i)
			child->_keys[i + _minDegree] = sibling->_keys[i];

		// Copying the child pointers from _childs[idx+1] to _childs[idx]
		if (!child->_isLeaf)
		{
			for (int i = 0; i <= sibling->_number; ++i)
				child->_childs[i + _minDegree] = sibling->_childs[i];
		}

		// Moving all keys after idx in the current node one step before -
		// to fill the gap created by moving keys[idx] to _childs[idx]
		for (int i = idx + 1; i < _number; ++i)
			_keys[i - 1] = _keys[i];

		// Moving the child pointers after (idx+1) in the current node one
		// step before
		for (int i = idx + 2; i <= _number; ++i)
			_childs[i - 1] = _childs[i];

		// Updating the key count of child and the current node
		child->_number += sibling->_number + 1;
		_number--;

		// Freeing the memory occupied by sibling
		delete(sibling);
	}

	// Make BTree friend of this so that we can access private members of
	// this class in BTree functions
	friend class BTree<T>;

private:
	BTreeNode () = delete;

	T* _keys;               //! キーの格納領域
	int _minDegree;	        //! このノードに格納できるサイズ
	BTreeNode<T>** _childs; //! 子ノードの格納領域
	int _number;            //! 現在のキーの格納数
	bool _isLeaf;           //! このノードが枝か葉か
};

template<class T>
class BTree
{
public:
	//! @brief コンストラクタ
	//! @param degree １つのノードに格納するキーの数
	BTree (int degree)
	{
		_root = nullptr;
		_minDegree = degree;
	}

	//! @brief デストラクタ
	~BTree ()
	{
		RemoveAll ();
		delete _root;
	}

	//! @brief B-Treeの内容をstd::coutに列挙します。
	void Traverse ()
	{
		if (_root != nullptr) _root->traverse ();
	}

	//! @brief B-Treeから指定のキーを検索します。
	//! @param key 検索キー
	//! @return 検索結果のT*
	T* Search (T key)
	{
		return (_root == nullptr) ? nullptr : _root->search (key);
	}

	//! @brief キーをB-Treeに挿入します。
	//! @param key 挿入するキー
	void Insert (T key)
	{
		// If tree is empty
		if (_root == nullptr)
		{
			// Allocate memory for root
			_root = new BTreeNode<T> (_minDegree, true);
			_root->_keys[0] = key; // Insert key
			_root->_number = 1; // Update number of keys in root
		}
		// If tree is not empty
		else
		{
			// If root is full, then tree grows in height
			if (_root->_number == 2 * _minDegree - 1)
			{
				// Allocate memory for new root
				BTreeNode<T>* s = new BTreeNode<T> (_minDegree, false);
				// Make old root as child of new root
				s->_childs[0] = _root;

				// Split the old root and move 1 key to the new root
				s->splitChild (0, _root);

				// New root has two children now. Decide which of the
				// two children is going to have new key
				int i = 0;
				if (s->_keys[0] < key)
					i++;
				s->_childs[i]->insertNonFull (key);

				// Change root
				_root = s;
			}
			else // If root is not full, call insertNonFull for root
				_root->insertNonFull (key);
		}
	}

	//! @brief キーをB-Treeから削除します。
	//! @param key 削除するキー
	//! @return 削除結果
	//! @retval true(キーを削除した)
	//! @retval false(キーが存在しなかった)
	bool Remove (T key)
	{
		if (!_root)
		{
			std::cout << "The tree is empty" << std::endl;
			return false;
		}

		// Call the remove function for root
		bool ret = _root->remove (key);

		// If the root node has 0 keys, make its first child as the new root
		// if it has a child, otherwise set root as nullptr
		if (_root->_number == 0)
		{
			BTreeNode<T>* tmp = _root;
			if (_root->_isLeaf)
				_root = nullptr;
			else
				_root = _root->_childs[0];

			// Free the old root
			delete tmp;
		}

		return ret;
	}

	//! @brief 格納されたキーをすべて削除します。
	void RemoveAll ()
	{
		while (_root != nullptr)
		{
			T key;

			if (_root->_isLeaf)
			{
				key = _root->_keys[0];
			}
			else
			{
				key = _root->_childs[0]->_keys[0];
			}

			if (!Remove (key)) break;
		}
	}

private:
	BTree () = delete;

	BTreeNode<T>* _root; // ルートノード
	int _minDegree;      // 各ノードに格納できるサイズ
};