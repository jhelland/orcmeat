#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__


template <class T>
class RedBlackTree;

template <class T>
struct TreeNode {
	friend class RedBlackTree<T>;

	T data;
	TreeNode<T> *left;
	TreeNode<T> *right;
	TreeNode<T> *p;

	bool color;			// true == red  &&  false == black
};


template <class T>
class RedBlackTree {
public:
	TreeNode<T>* nil;
	TreeNode<T>* root;

public:
	RedBlackTree() : nil(new TreeNode<T>), root(nil) {
		nil->left = 0; 
		nil->p = 0; 
		nil->right = 0; 
		nil->color = false;
	}
	

	void insert(T key) {
		TreeNode<T>* t = new TreeNode<T>;
		t->data = key;
		TreeNode<T>* x = root;
		TreeNode<T>* y = nil;
		while (x != nil) {
			y = x;
			if (key < x->data)
				x = x->left;
			else
				x = x->right;
		}
		t->p = y;
		if (y == nil)
			root = t;
		else {
			if (t->data < y->data)
				y->left = t;
			else
				y->right = t;
		}
		t->left = nil;
		t->right = nil;
		t->color = true;
		rbInsertFixup(t);
	}


	TreeNode<T>* search(T key) {
		TreeNode<T>* x = root;
		while (x != nil && key != x->data) {
			if (key < x->data)
				x = x->left;
			else
				x = x->right;
		}
		return x;
	}


	TreeNode<T>* search_threshold(T threshold) {
		TreeNode<T>* x = root;
		while (x != nil && threshold < x->data) {
			if (threshold < x->data)
				x = x->left;
			else
				x = x->right;
		}
		return x
	}


	void remove(T key) {
		TreeNode<T>* x = search(key);
		if (x != nil)
			rbDelete(x);
	}

private:
	void leftRotate(TreeNode<T> *x) {		// x->right != nil
		TreeNode<T> * y = x->right;
		if (x->p == nil)
			root = y;
		else {
			if (x == x->p->left)
				x->p->left = y;
			else
				x->p->right = y;
		}
		y->p = x->p;
		x->right = y->left;
		y->left->p = x;
		y->left = x;
		x->p = y;
	}


	void rightRotate(TreeNode<T> *x) {
		TreeNode<T> * y = x->left;
		if (x->p == nil)
			root = y;
		else {
			if (x->p->left == x)
				x->p->left = y;
			else
				x->p->right = y;
		}
		y->p = x->p;
		x->left = y->right;
		y->right->p = x;
		y->right = x;
		x->p = y;
	}


	void rbDelete(TreeNode<T> *z) {
		TreeNode<T> * x = nil;
		TreeNode<T> * y = nil;
		if (z->left == nil || z->right == nil)
			y = z;
		else
			y = treeSuccessor(z);
		if (y->left != nil)
			x = y->left;
		else
			x = y->right;
		x->p = y->p;
		if (y->p == nil)
			root = x;
		else {
			if (y == y->p->left)
				y->p->left = x;
			else
				y->p->right = x;
		}
		if (y != z)
			z->data = y->data;
		if (y->color == false)
			rbDeleteFixup(x);
		delete y;
	}


	void rbDeleteFixup(TreeNode<T> * x) {
		while (x != root && x->color == false) {
			TreeNode<T> * w = 0;
			if (x->p->left == x) {
				w = x->p->right;
				if (w->color == true) {
					w->color = false;
					x->p->color = true;
					leftRotate(x->p);
					w = x->p->right;
				}
				if (w->left->color == false && w->right->color == false) {
					w->color = true;
					x = x->p;
				}
				else {
					if (w->right->color == false) {
						w->left->color = false;
						w->color = true;
						rightRotate(w);
						w = x->p->right;
					}
					w->color = x->p->color;
					x->p->color = false;
					w->right->color = false;
					leftRotate(x->p);
					x = root;
				}
			}
			else {
				w = x->p->left;
				if (w->color == true) {
					w->color = false;
					x->p->color = true;
					rightRotate(x->p);
					w = x->p->left;
				}
				if (w->right->color == false && w->left->color == false) {
					w->color = true;
					x = x->p;
				}
				else {
					if (w->left->color == false) {
						w->right->color = false;
						w->color = true;
						leftRotate(w);
						w = x->p->left;
					}
					w->color = x->p->color;
					x->p->color = false;
					w->left->color = false;
					rightRotate(x->p);
					x = root;
				}
			}
		}
		x->color = false;
	}


	void rbInsertFixup(TreeNode<T> *z) {
		while (z->p->color == true) {
			if (z->p == z->p->p->left) {
				TreeNode<T> * y = z->p->p->right;
				if (y->color == true) {
					z->p->color = false;
					y->color = false;
					z->p->p->color = true;
					z = z->p->p;
				}
				else {
					if (z == z->p->right) {
						z = z->p;
						leftRotate(z);
					}//
					z->p->color = false;
					z->p->p->color = true;
					z->p->right->color = false;
					rightRotate(z->p->p);
				}
			}
			else {
				TreeNode<T> * y = z->p->p->left;
				if (y->color == true) {
					z->p->color = false;
					y->color = false;
					z->p->p->color = true;
					z = z->p->p;
				}
				else {
					if (z == z->p->left) {
						z = z->p;
						rightRotate(z);
					}
					z->p->color = false;
					z->p->p->color = true;
					z->p->left->color = false;
					leftRotate(z->p->p);
				}
			}
		}
		root->color = false;
	}


	TreeNode<T>* treeSuccessor(TreeNode<T> *x) {
		if (x->right != nil) {
			while (x->left != nil)
				x = x->left;
			return x;
		}
		TreeNode<T> * y = x->p;
		while (y != nil && x == y->right) {
			x = y;
			y = y->p;
		}
		return y;
	}	

};


#endif  // RED_BLACK_TREE