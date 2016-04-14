// rand_bst.hpp

#ifndef RAND_BST_HPP
#define RAND_BST_HPP

#include <cstdlib>
#include <ctime>
#include <stack>
#include <memory>

namespace BST {

template < typename Item >
class RandBST {

	class Node;

	public:
		class PreOrderIterator;

		using pnode = std::shared_ptr<Node>;
		using iterator = PreOrderIterator;

		RandBST();
		RandBST(const RandBST&);
		~RandBST() {}

		bool contains(const Item&) const;
		bool is_empty() const;

		void insert(const Item&);
		void remove(const Item&);

		const Item& get_min() const;
		const Item& get_max() const;
		void clear();

		auto begin() const;
		auto end() const { return it_end; };
		auto traverse_pre_order() const;


		class PreOrderIterator {

			friend RandBST;

			public: 
				PreOrderIterator()
					: ended(false), level(0)
				{ }

				auto operator*();
				auto operator++();
				auto operator++(int);
				bool operator==(const iterator&) const;
				bool operator!=(const iterator&) const;
				auto get_level() { return level; }
				auto get_size() { return node->size; }

			private: 
				pnode node;
				std::stack< std::pair<pnode, int> > st;
				bool ended;
				int level;
		};

		void print(std::ostream&) const;


	private:

		struct Node {

			Item data;
			pnode left;
			pnode right;
			int size;

			Node(const Item& d, pnode l, pnode r)
				: data(d), left(l), right(r), size(1)
			{ }
			Node(const Node& other) 
				: data(other.data),
				  size(other.size)
			{ 
				left.reset(other.left ? new Node(*other.left) : nullptr);
				right.reset(other.right ? new Node(*other.right) : nullptr);
			}
		};

		// root node
		pnode root;
		// end iterator
		iterator it_end;

		void insert(pnode& , const Item&);
		void insert_to_root(pnode& , const Item &);
		void remove(pnode& , const Item&);

		void print(const pnode&, std::ostream&) const;

		void rotate_left(pnode&);
		void rotate_right(pnode&);

		pnode join(pnode&, pnode&);

		int get_size(const pnode&);
		void fix_size(pnode&);
};

template < typename Item >
RandBST<Item>::RandBST()
    : root(nullptr)
{
	std::srand(std::time(0));
	it_end.ended = true;
}

template < typename Item >
RandBST<Item>::RandBST(const RandBST& other)
{
	root.reset(new Node(*other.root));
}


template < typename Item >
bool RandBST<Item>::contains(const Item& to_find) const
{
	pnode p = root;

	while(p) {

		if(to_find < p->data) {
			p = p->left;
		}
		else if( to_find > p->data) {
			p = p->right;
		}
		else {
			return true;
		}
	}

	return false;
}

template < typename Item >
bool RandBST<Item>::is_empty() const
{
	return root == nullptr;
}

template < typename Item >
void RandBST<Item>::insert(const Item& to_insert)
{
	if( !contains(to_insert) )
		insert(root, to_insert);
}

template < typename Item >
void RandBST<Item>::insert(pnode& node, const Item& to_insert)
{
	if(node == nullptr) {
		insert_to_root(node, to_insert);
	}
	else if(std::rand() < RAND_MAX / (node->size+1)) {
		insert_to_root(node, to_insert);
	}
	else if(to_insert < node->data) {
		insert(node->left, to_insert);
		fix_size(node);
	}
	else {
		insert(node->right, to_insert);
		fix_size(node);
	}
}


template < typename Item >
void RandBST<Item>::insert_to_root(pnode& node, const Item& to_insert)
{
	if(node == nullptr) {
		node.reset(new Node(to_insert, nullptr, nullptr));
	}
	else if(to_insert < node->data) {
		insert_to_root(node->left, to_insert);
		rotate_right(node);
	}
	else if(to_insert > node->data) {
		insert_to_root(node->right, to_insert);
		rotate_left(node);
	}
}


template < typename Item >
void RandBST<Item>::remove(const Item& to_remove)
{
	remove(root, to_remove);
}


template < typename Item >
void RandBST<Item>::remove(pnode& node, const Item& to_remove)
{
	if(node == nullptr) {
		return;
	}
	if(to_remove < node->data) {
		remove(node->left, to_remove);
		fix_size(node);
	}
	else if(to_remove > node->data) {
		remove(node->right, to_remove);
		fix_size(node);
	}
	else {
		node = join(node->left, node->right);
	}
}

template < typename Item >
const Item& RandBST<Item>::get_min() const
{
	pnode min = nullptr;

	for(pnode p = root; p; min = p, p = p->left);

	return min->data;
}

template < typename Item >
const Item& RandBST<Item>::get_max() const
{
	pnode max = nullptr;

	for(pnode p = root; p; max = p, p = p->right);

	return max->data;
}

template < typename Item >
void RandBST<Item>::clear()
{
	root.reset();
}

template < typename Item >
void RandBST<Item>::print(std::ostream& os) const
{
	os << "# ";
	print(root, os);
}

template < typename Item >
void RandBST<Item>::print(const pnode& node, std::ostream& os) const
{
	if(node == nullptr) {
		os << "* ";
		return;
	}

	os << node->data;
	os << " <";
	print(node->left, os);
	os << " >";
	print(node->right, os);
}


template < typename Item >
void RandBST<Item>::rotate_left(pnode& node)
{
	pnode t = node->right;
	if(t != nullptr) {
		node->right = t->left;
		t->left = node;

		fix_size(node);
		fix_size(t);

		node = t;
	}
}


template < typename Item >
void RandBST<Item>::rotate_right(pnode& node)
{
	pnode t = node->left;
	if(t != nullptr) {
		node->left = t->right;
		t->right = node;

		fix_size(node);
		fix_size(t);

		node = t;
	}
}


template < typename Item >
typename RandBST<Item>::pnode
RandBST<Item>::join(pnode& a, pnode& b)
{
	if(a == nullptr) return b;
	if(b == nullptr) return a;

	if(rand() / (RAND_MAX / (a->size + b->size) + 1) < a->size) {

		a->right = join(a->right, b);
		fix_size(a);
		return a;
	}
	else {

		b->left = join(a, b->left);
		fix_size(b);
		return b;
	}
}


template < typename Item >
int RandBST<Item>::get_size(const pnode& node)
{
	return node ? node->size : 0;
}

template < typename Item >
void RandBST<Item>::fix_size(pnode& node)
{
	node->size = get_size(node->left) + get_size(node->right) + 1;
}

template < typename Item >
auto RandBST<Item>::begin() const
{
	return traverse_pre_order();
}

template < typename Item >
auto RandBST<Item>::traverse_pre_order() const
{
	iterator it;
	it.node = root;
	if(root == nullptr) {
		it.ended = true;
	}
	return it;
}

template < typename Item >
auto RandBST<Item>::PreOrderIterator::operator*()
{
	return node->data;
}

template < typename Item >
auto RandBST<Item>::PreOrderIterator::operator++()
{
	if(!ended) {

		if(node->left != nullptr) {
			st.push(std::make_pair(node->right, level+1));
			node = node->left;
			++level;
		}
		else if(node->right != nullptr) {
			node = node->right;
			++level;
		}
		else {

			while(!st.empty() && st.top().first == nullptr) {
				level = st.top().second;
				st.pop();
			}

			if(!st.empty()) {
				node = st.top().first;
				level = st.top().second;
				st.pop();
			}
			else {
				ended = true;
			}
		}
	}
	return *this;
}

template < typename Item >
auto RandBST<Item>::PreOrderIterator::operator++(int)
{
	return operator++();
}

template < typename Item >
bool RandBST<Item>::iterator::operator==(const iterator& it) const
{
	return (ended && it.ended) || node == it.node;
}

template < typename Item >
bool RandBST<Item>::iterator::operator!=(const iterator& it) const
{
	return !operator==(it);
}

} // namespace BST

#endif // RAND_BST_HPP
