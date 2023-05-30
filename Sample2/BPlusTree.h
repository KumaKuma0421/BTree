// ---------------------------------------------------------------------------------------
// BPlusTree.h
//
// @sa https://github.com/solangii/b-plus-tree
// ---------------------------------------------------------------------------------------

#pragma once 

#include <iostream>
#include <iomanip>

static int indent = 0;

template <typename T>
struct Node
{
public:
	Node (std::size_t degree)
	{
		_isLeaf = false;
		_degree = degree;
		_size = 0;

		_item = new T[_degree - 1];
		for (int i = 0; i < _degree - 1; i++)
		{
			_item[i] = 0;
		}

		_childs = new Node<T>*[_degree];
		for (int i = 0; i < _degree; i++)
		{
			_childs[i] = nullptr;
		}

		_parent = nullptr;
	}

	bool        _isLeaf;
	std::size_t _degree;
	std::size_t _size;
	T*          _item;
	Node<T>**   _childs;
	Node<T>*    _parent;
};

template <class T>
class BPlusTree
{
public:
	BPlusTree (std::size_t degree)
	{
		_root = nullptr;
		_degree = degree;
	}

	~BPlusTree ()
	{
		clear (_root);
	}

	Node<T>* getroot ()
	{
		return _root;
	}

	Node<T>* BPlusTreeSearch (Node<T>* node, T key)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		else
		{
			Node<T>* cursor = node;

			while (!cursor->_isLeaf)
			{
				for (int i = 0; i < cursor->_size; i++)
				{
					if (key < cursor->_item[i])
					{
						cursor = cursor->_childs[i];
						break;
					}
					if (i == (cursor->_size) - 1)
					{
						cursor = cursor->_childs[i + 1];
						break;
					}
				}
			}

			for (int i = 0; i < cursor->_size; i++)
			{
				if (cursor->_item[i] == key)
				{
					return cursor;
				}
			}

			return nullptr;
		}
	}

	Node<T>* BPlusTreeRangeSearch (Node<T>* node, T key)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		else
		{
			Node<T>* cursor = node;

			while (!cursor->_isLeaf)
			{
				for (int i = 0; i < cursor->_size; i++)
				{
					if (key < cursor->_item[i])
					{
						cursor = cursor->_childs[i];
						break;
					}
					if (i == (cursor->_size) - 1)
					{
						cursor = cursor->_childs[i + 1];
						break;
					}
				}
			}

			return cursor;
		}
	}

	int range_search (T start, T end, T* result_data, int arr_length)
	{
		int index = 0;

		Node<T>* start_node = BPlusTreeRangeSearch (this->_root, start);
		Node<T>* cursor = start_node;
		T temp = cursor->_item[0];

		while (temp <= end)
		{
			if (cursor == nullptr)
			{
				break;
			}

			for (int i = 0; i < cursor->_size; i++)
			{
				temp = cursor->_item[i];
				if ((temp >= start) && (temp <= end))
				{
					result_data[index] = temp;
					index++;
				}
			}
			cursor = cursor->_childs[cursor->_size];
		}

		return index;
	}

	bool search (T data)
	{
		return BPlusTreeSearch (this->_root, data) != nullptr;
	}

	int find_index (T* arr, T data, int len)
	{
		int index = 0;
		for (int i = 0; i < len; i++)
		{
			if (data < arr[i])
			{
				index = i;
				break;
			}
			if (i == len - 1)
			{
				index = len;
				break;
			}
		}

		return index;
	}

	T* item_insert (T* arr, T data, int len)
	{
		int index = 0;

		for (int i = 0; i < len; i++)
		{
			if (data < arr[i])
			{
				index = i;
				break;
			}
			if (i == len - 1)
			{
				index = len;
				break;
			}
		}

		for (int i = len; i > index; i--)
		{
			arr[i] = arr[i - 1];
		}

		arr[index] = data;

		return arr;
	}

	Node<T>** child_insert (Node<T>** child_arr, Node<T>* child, int len, int index)
	{
		for (int i = len; i > index; i--)
		{
			child_arr[i] = child_arr[i - 1];
		}

		child_arr[index] = child;

		return child_arr;
	}

	Node<T>* child_item_insert (Node<T>* node, T data, Node<T>* child)
	{
		int item_index = 0;
		int child_index = 0;

		for (int i = 0; i < node->_size; i++)
		{
			if (data < node->_item[i])
			{
				item_index = i;
				child_index = i + 1;
				break;
			}

			if (i == node->_size - 1)
			{
				item_index = node->_size;
				child_index = node->_size + 1;
				break;
			}
		}

		for (int i = node->_size; i > item_index; i--)
		{
			node->_item[i] = node->_item[i - 1];
		}

		for (int i = node->_size + 1; i > child_index; i--)
		{
			node->_childs[i] = node->_childs[i - 1];
		}

		node->_item[item_index] = data;
		node->_childs[child_index] = child;

		return node;
	}

	void InsertPar (Node<T>* par, Node<T>* child, T data)
	{
		Node<T>* cursor = par;

		if (cursor->_size < this->_degree - 1)
		{
			cursor = child_item_insert (cursor, data, child);
			cursor->_size++;
		}
		else
		{
			auto* Newnode = new Node<T> (this->_degree);
			Newnode->_parent = cursor->_parent;

			T* item_copy = new T[cursor->_size + 1];
			for (int i = 0; i < cursor->_size; i++)
			{
				item_copy[i] = cursor->_item[i];
			}

			item_copy = item_insert (item_copy, data, cursor->_size);

			auto** child_copy = new Node<T>*[cursor->_size + 2];
			for (int i = 0; i < cursor->_size + 1; i++)
			{
				child_copy[i] = cursor->_childs[i];
			}

			child_copy[cursor->_size + 1] = nullptr;
			child_copy = child_insert (child_copy, child, cursor->_size + 1, find_index (item_copy, data, cursor->_size + 1));

			cursor->_size = (this->_degree) / 2;
			if ((this->_degree) % 2 == 0)
			{
				Newnode->_size = (this->_degree) / 2 - 1;
			}
			else
			{
				Newnode->_size = (this->_degree) / 2;
			}

			for (int i = 0; i < cursor->_size; i++)
			{
				cursor->_item[i] = item_copy[i];
				cursor->_childs[i] = child_copy[i];
			}
			cursor->_childs[cursor->_size] = child_copy[cursor->_size];

			for (int i = 0; i < Newnode->_size; i++)
			{
				Newnode->_item[i] = item_copy[cursor->_size + i + 1];
				Newnode->_childs[i] = child_copy[cursor->_size + i + 1];
				Newnode->_childs[i]->_parent = Newnode;
			}
			Newnode->_childs[Newnode->_size] = child_copy[cursor->_size + Newnode->_size + 1];
			Newnode->_childs[Newnode->_size]->_parent = Newnode;

			T paritem = item_copy[this->_degree / 2];

			delete[] item_copy;
			delete[] child_copy;

			if (cursor->_parent == nullptr)
			{
				auto* Newparent = new Node<T> (this->_degree);
				cursor->_parent = Newparent;
				Newnode->_parent = Newparent;

				Newparent->_item[0] = paritem;
				Newparent->_size++;

				Newparent->_childs[0] = cursor;
				Newparent->_childs[1] = Newnode;

				this->_root = Newparent;
			}
			else
			{
				InsertPar (cursor->_parent, Newnode, paritem);
			}
		}
	}

	void insert (T data)
	{
		if (this->_root == nullptr)
		{
			this->_root = new Node<T> (this->_degree);
			this->_root->_isLeaf = true;
			this->_root->_item[0] = data;
			this->_root->_size = 1;
		}
		else
		{
			Node<T>* cursor = this->_root;

			cursor = BPlusTreeRangeSearch (cursor, data);

			if (cursor->_size < (this->_degree - 1))
			{
				cursor->_item = item_insert (cursor->_item, data, cursor->_size);
				cursor->_size++;
				cursor->_childs[cursor->_size] = cursor->_childs[cursor->_size - 1];
				cursor->_childs[cursor->_size - 1] = nullptr;
			}
			else
			{
				auto* Newnode = new Node<T> (this->_degree);
				Newnode->_isLeaf = true;
				Newnode->_parent = cursor->_parent;

				T* item_copy = new T[cursor->_size + 1];
				for (int i = 0; i < cursor->_size; i++)
				{
					item_copy[i] = cursor->_item[i];
				}

				item_copy = item_insert (item_copy, data, cursor->_size);

				cursor->_size = (this->_degree) / 2;
				if ((this->_degree) % 2 == 0)
				{
					Newnode->_size = (this->_degree) / 2;
				}
				else
				{
					Newnode->_size = (this->_degree) / 2 + 1;
				}

				for (int i = 0; i < cursor->_size; i++)
				{
					cursor->_item[i] = item_copy[i];
				}

				for (int i = 0; i < Newnode->_size; i++)
				{
					Newnode->_item[i] = item_copy[cursor->_size + i];
				}

				cursor->_childs[cursor->_size] = Newnode;
				Newnode->_childs[Newnode->_size] = cursor->_childs[this->_degree - 1];
				cursor->_childs[this->_degree - 1] = nullptr;

				delete[] item_copy;

				T paritem = Newnode->_item[0];

				if (cursor->_parent == nullptr)
				{
					auto* Newparent = new Node<T> (this->_degree);
					cursor->_parent = Newparent;
					Newnode->_parent = Newparent;

					Newparent->_item[0] = paritem;
					Newparent->_size++;

					Newparent->_childs[0] = cursor;
					Newparent->_childs[1] = Newnode;

					this->_root = Newparent;
				}
				else
				{
					InsertPar (cursor->_parent, Newnode, paritem);
				}
			}
		}
	}

	void remove (T data)
	{
		Node<T>* cursor = this->_root;

		cursor = BPlusTreeRangeSearch (cursor, data);

		int sib_index = -1;
		for (int i = 0; i < cursor->_parent->_size + 1; i++)
		{
			if (cursor == cursor->_parent->_childs[i])
			{
				sib_index = i;
			}
		}

		int left = sib_index - 1;
		int right = sib_index + 1;

		int del_index = -1;
		for (int i = 0; i < cursor->_size; i++)
		{
			if (cursor->_item[i] == data)
			{
				del_index = i;
				break;
			}
		}

		if (del_index == -1)
		{
			return;
		}

		for (int i = del_index; i < cursor->_size - 1; i++)
		{
			cursor->_item[i] = cursor->_item[i + 1];
		}
		cursor->_item[cursor->_size - 1] = 0;
		cursor->_size--;

		if (cursor == this->_root && cursor->_size == 0)
		{
			clear (this->_root);
			this->_root = nullptr;
			return;
		}
		cursor->_childs[cursor->_size] = cursor->_childs[cursor->_size + 1];
		cursor->_childs[cursor->_size + 1] = nullptr;

		if (cursor == this->_root)
		{
			return;
		}

		if (cursor->_size < _degree / 2)
		{
			if (left >= 0)
			{
				Node<T>* leftsibling = cursor->_parent->_childs[left];

				if (leftsibling->_size > _degree / 2)
				{
					T* temp = new T[cursor->_size + 1];

					for (int i = 0; i < cursor->_size; i++)
					{
						temp[i] = cursor->_item[i];
					}

					item_insert (temp, leftsibling->_item[leftsibling->_size - 1], cursor->_size);
					for (int i = 0; i < cursor->_size + 1; i++)
					{
						cursor->_item[i] = temp[i];
					}
					cursor->_size++;
					delete[] temp;

					cursor->_childs[cursor->_size] = cursor->_childs[cursor->_size - 1];
					cursor->_childs[cursor->_size - 1] = nullptr;

					leftsibling->_item[leftsibling->_size - 1] = 0;
					leftsibling->_size--;
					leftsibling->_childs[leftsibling->_size] = leftsibling->_childs[leftsibling->_size + 1]; //cursor
					leftsibling->_childs[leftsibling->_size + 1] = nullptr;

					cursor->_parent->_item[left] = cursor->_item[0];

					return;
				}
			}

			if (right <= cursor->_parent->_size)
			{
				Node<T>* rightsibling = cursor->_parent->_childs[right];

				if (rightsibling->_size > _degree / 2)
				{
					T* temp = new T[cursor->_size + 1];

					for (int i = 0; i < cursor->_size; i++)
					{
						temp[i] = cursor->_item[i];
					}

					item_insert (temp, rightsibling->_item[0], cursor->_size);
					for (int i = 0; i < cursor->_size + 1; i++)
					{
						cursor->_item[i] = temp[i];
					}
					cursor->_size++;
					delete[] temp;

					cursor->_childs[cursor->_size] = cursor->_childs[cursor->_size - 1];
					cursor->_childs[cursor->_size - 1] = nullptr;

					for (int i = 0; i < rightsibling->_size - 1; i++)
					{
						rightsibling->_item[i] = rightsibling->_item[i + 1];
					}
					rightsibling->_item[rightsibling->_size - 1] = 0;
					rightsibling->_size--;
					rightsibling->_childs[rightsibling->_size] = rightsibling->_childs[rightsibling->_size + 1]; //cursor
					rightsibling->_childs[rightsibling->_size + 1] = nullptr;

					cursor->_parent->_item[right - 1] = rightsibling->_item[0];

					return;
				}
			}

			if (left >= 0)
			{
				Node<T>* leftsibling = cursor->_parent->_childs[left];

				for (int i = 0; i < cursor->_size; i++)
				{
					leftsibling->_item[leftsibling->_size + i] = cursor->_item[i];
				}

				leftsibling->_childs[leftsibling->_size] = nullptr;
				leftsibling->_size = leftsibling->_size + cursor->_size;
				leftsibling->_childs[leftsibling->_size] = cursor->_childs[cursor->_size];

				Removepar (cursor, left, cursor->_parent);
				for (int i = 0; i < cursor->_size; i++)
				{
					cursor->_item[i] = 0;
					cursor->_childs[i] = nullptr;
				}
				cursor->_childs[cursor->_size] = nullptr;

				delete[] cursor->_item;
				delete[] cursor->_childs;
				delete cursor;

				return;
			}

			if (right <= cursor->_parent->_size)
			{
				Node<T>* rightsibling = cursor->_parent->_childs[right];

				for (int i = 0; i < rightsibling->_size; i++)
				{
					cursor->_item[i + cursor->_size] = rightsibling->_item[i];
				}

				cursor->_childs[cursor->_size] = nullptr;
				cursor->_size = rightsibling->_size + cursor->_size;
				cursor->_childs[cursor->_size] = rightsibling->_childs[rightsibling->_size];

				Removepar (rightsibling, right - 1, cursor->_parent);

				for (int i = 0; i < rightsibling->_size; i++)
				{
					rightsibling->_item[i] = 0;
					rightsibling->_childs[i] = nullptr;
				}

				rightsibling->_childs[rightsibling->_size] = nullptr;

				delete[] rightsibling->_item;
				delete[] rightsibling->_childs;
				delete rightsibling;
				return;
			}
		}
		else
		{
			return;
		}
	}

	void Removepar (Node<T>* node, int index, Node<T>* par)
	{
		Node<T>* remover = node;
		Node<T>* cursor = par;
		T target = cursor->_item[index];

		if (cursor == this->_root && cursor->_size == 1)
		{
			if (remover == cursor->_childs[0])
			{
				delete[] remover->_item;
				delete[] remover->_childs;
				delete remover;
				this->_root = cursor->_childs[1];
				delete[] cursor->_item;
				delete[] cursor->_childs;
				delete cursor;
				return;
			}

			if (remover == cursor->_childs[1])
			{
				delete[] remover->_item;
				delete[] remover->_childs;
				delete remover;
				this->_root = cursor->_childs[0];
				delete[] cursor->_item;
				delete[] cursor->_childs;
				delete cursor;
				return;
			}
		}

		for (int i = index; i < cursor->_size - 1; i++)
		{
			cursor->_item[i] = cursor->_item[i + 1];
		}
		cursor->_item[cursor->_size - 1] = 0;

		int rem_index = -1;

		for (int i = 0; i < cursor->_size + 1; i++)
		{
			if (cursor->_childs[i] == remover)
			{
				rem_index = i;
			}
		}

		if (rem_index == -1)
		{
			return;
		}

		for (int i = rem_index; i < cursor->_size; i++)
		{
			cursor->_childs[i] = cursor->_childs[i + 1];
		}
		cursor->_childs[cursor->_size] = nullptr;
		cursor->_size--;

		if (cursor == this->_root)
		{
			return;
		}
		if (cursor->_size < _degree / 2)
		{
			int sib_index = -1;
			for (int i = 0; i < cursor->_parent->_size + 1; i++)
			{
				if (cursor == cursor->_parent->_childs[i])
				{
					sib_index = i;
				}
			}
			int left = sib_index - 1;
			int right = sib_index + 1;

			if (left >= 0)
			{
				Node<T>* leftsibling = cursor->_parent->_childs[left];

				if (leftsibling->_size > _degree / 2)
				{
					T* temp = new T[cursor->_size + 1];

					for (int i = 0; i < cursor->_size; i++)
					{
						temp[i] = cursor->_item[i];
					}

					item_insert (temp, cursor->_parent->_item[left], cursor->_size);
					for (int i = 0; i < cursor->_size + 1; i++)
					{
						cursor->_item[i] = temp[i];
					}
					cursor->_parent->_item[left] = leftsibling->_item[leftsibling->_size - 1];
					delete[] temp;

					Node<T>** child_temp = new Node<T>*[cursor->_size + 2];

					for (int i = 0; i < cursor->_size + 1; i++)
					{
						child_temp[i] = cursor->_childs[i];
					}

					child_insert (child_temp, leftsibling->_childs[leftsibling->_size], cursor->_size, 0);

					for (int i = 0; i < cursor->_size + 2; i++)
					{
						cursor->_childs[i] = child_temp[i];
					}
					delete[] child_temp;

					cursor->_size++;
					leftsibling->_size--;
					return;
				}
			}

			if (right <= cursor->_parent->_size)
			{
				Node<T>* rightsibling = cursor->_parent->_childs[right];

				if (rightsibling->_size > _degree / 2)
				{
					T* temp = new T[cursor->_size + 1];

					for (int i = 0; i < cursor->_size; i++)
					{
						temp[i] = cursor->_item[i];
					}

					item_insert (temp, cursor->_parent->_item[sib_index], cursor->_size);
					for (int i = 0; i < cursor->_size + 1; i++)
					{
						cursor->_item[i] = temp[i];
					}
					cursor->_parent->_item[sib_index] = rightsibling->_item[0];
					delete[] temp;

					cursor->_childs[cursor->_size + 1] = rightsibling->_childs[0];
					for (int i = 0; i < rightsibling->_size; i++)
					{
						rightsibling->_childs[i] = rightsibling->_childs[i + 1];
					}
					rightsibling->_childs[rightsibling->_size] = nullptr;

					cursor->_size++;
					rightsibling->_size--;
					return;
				}
			}

			if (left >= 0)
			{
				Node<T>* leftsibling = cursor->_parent->_childs[left];

				leftsibling->_item[leftsibling->_size] = cursor->_parent->_item[left];

				for (int i = 0; i < cursor->_size; i++)
				{
					leftsibling->_item[leftsibling->_size + i + 1] = cursor->_item[i];
				}

				for (int i = 0; i < cursor->_size + 1; i++)
				{
					leftsibling->_childs[leftsibling->_size + i + 1] = cursor->_childs[i];
					cursor->_childs[i]->_parent = leftsibling;
				}

				for (int i = 0; i < cursor->_size + 1; i++)
				{
					cursor->_childs[i] = nullptr;
				}
				leftsibling->_size = leftsibling->_size + cursor->_size + 1;

				Removepar (cursor, left, cursor->_parent);
				return;
			}

			if (right <= cursor->_parent->_size)
			{
				Node<T>* rightsibling = cursor->_parent->_childs[right];

				cursor->_item[cursor->_size] = cursor->_parent->_item[right - 1];

				for (int i = 0; i < rightsibling->_size; i++)
				{
					cursor->_item[cursor->_size + 1 + i] = rightsibling->_item[i];
				}
				for (int i = 0; i < rightsibling->_size + 1; i++)
				{
					cursor->_childs[cursor->_size + i + 1] = rightsibling->_childs[i];
					rightsibling->_childs[i]->_parent = rightsibling;
				}
				for (int i = 0; i < rightsibling->_size + 1; i++)
				{
					rightsibling->_childs[i] = nullptr;
				}

				rightsibling->_size = rightsibling->_size + cursor->_size + 1;

				Removepar (rightsibling, right - 1, cursor->_parent);
				return;
			}
		}
		else
		{
			return;
		}
	}

	void clear (Node<T>* cursor)
	{
		if (cursor != nullptr)
		{
			if (!cursor->_isLeaf)
			{
				for (int i = 0; i <= cursor->_size; i++)
				{
					clear (cursor->_childs[i]);
				}
			}
			delete[] cursor->_item;
			delete[] cursor->_childs;
			delete cursor;
		}
	}

	void Traverse ()
	{
		print2 (this->_root);
	}

	void print (Node<T>* cursor)
	{
		if (cursor != NULL)
		{
			for (int i = 0; i < cursor->_size; ++i)
			{
				std::cout << std::setw(3) << cursor->_item[i] << " ";
			}
			std::cout << std::endl;

			if (!cursor->_isLeaf)
			{
				for (int i = 0; i < cursor->_size + 1; ++i)
				{
					print (cursor->_childs[i]);
				}
			}
		}
	}

	void print2 (Node<T>* cursor)
	{
		if (cursor != NULL)
		{
			int i = 0;
			for (i = 0; i < cursor->_size; i++)
			{
				if (cursor->_isLeaf == false)
				{
					std::cout << std::endl;
					indent += 2;
					print2 (cursor->_childs[i]);
					indent -= 2;
				}
				for (int j = 0; j < indent; j++) std::cout << " ";

				if (cursor->_isLeaf)
					std::cout << "    " << std::setw (3) << cursor->_item[i];
				else
					std::cout << "N[" << std::setw(3) << cursor->_item[i] << "] ";
			}
			std::cout << std::endl;

			if (!cursor->_isLeaf)
			{
				indent += 2;
				print2 (cursor->_childs[i]);
				indent -= 2;
			}
		}
	}

private:
	Node<T>* _root;
	std::size_t _degree;
};