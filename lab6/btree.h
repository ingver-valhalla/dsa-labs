// btree.h

typedef int Key;

// Cormen version of B-tree

struct BTreeNode {
	int n;
	int min_degree;
	Key * keys;
	BTreeNode ** children;
	bool leaf;

	BTreeNode( int d )
		: n(0),
		  min_degree(d),
		  keys(new Key[2*d-1]),
		  children(new BTreeNode*[2*d]),
		  leaf(true)
	{ }

	~BTreeNode()
	{
		delete[] keys;
		if(!leaf)
			for( int i = 0; i < n+1; ++i )
				delete children[i];
		delete[] children;
	}
};


BTreeNode * btree_search( BTreeNode * node, Key k, int * index );
void btree_split_child( BTreeNode * node, int index );
void btree_insert_nonfull( BTreeNode * node, Key k );
void btree_insert( BTreeNode *& root, Key k );
int btree_height( BTreeNode * root );
