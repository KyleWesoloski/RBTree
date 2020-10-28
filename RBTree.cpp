#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

template <typename keytype, typename valuetype>
class RBTree {
	struct node {
	bool red = false;
	keytype key;
	valuetype value; 
	struct node* leftChild = nullptr; 
	struct node* rightChild = nullptr; 
	struct node* parent = nullptr; 
	int subSize = 1; 
};
public:
	struct node* root = new node;
	struct node* nil = new node; 
	 
	int nodes; 
	RBTree() {
		nodes = 0; 
		nil->subSize =0;
		root->parent = nil; 
		root->leftChild = nil; 
		root->rightChild = nil; 
	}

	RBTree(keytype k[], valuetype V[], int s) {
		nodes = 0; 
		nil->subSize =0;
		root->parent = nil; 
		root->leftChild = nil; 
		root->rightChild = nil; 
		for(int i = 0; i<s; i++){
			this->insert(k[i],V[i]);			
		}
	}

	RBTree(RBTree& obj) {
		nodes = obj.nodes; 
		nil->subSize = 0; 
		this->root = deepCopy(obj.root);
	}

	~RBTree() {
		deleteTree(root); 
	}
	
	struct node* deepCopy(struct node* n){
		if (n->subSize == 0) return nil; 
		struct node* temp = new node; 
		temp->red = n->red; 
		temp->key = n->key; 
		temp->value = n->value; 
		temp->subSize = n->subSize; 
		
		temp->leftChild = deepCopy(n->leftChild);
		temp->rightChild = deepCopy(n->rightChild); 
		temp->leftChild->parent = temp; 
		temp->rightChild->parent = temp; 
		return temp; 
	}
	void deleteTree(struct node* n){
		if (n == nil) {delete n->parent; return;}
		deleteTree(n->leftChild);
		deleteTree(n->rightChild); 
		delete n; 
	}
	RBTree& operator=(const RBTree& src) {
		
		if (this != &src) { 
			nodes = src.nodes; 
			nil->subSize = 0; 
			root = deepCopy(src.root);
			root->parent = nil; 
			root->subSize = src.root->subSize; 	
		}
		
		this->inorder();
		
		return *this; 
	}


	void insert(keytype k, valuetype v){
		struct node* z = new node; 
		z->value = v; 
		z->key = k; 
		
		z->leftChild = nil;
		z->rightChild = nil;
		z->red = true; 
		struct node* y = new node; 
		struct node* x = new node; 
		y = nil; 
		x = root; 

		while(x != nil && nodes > 0){			//find place for new node
			y = x; 
			if(z->key < x->key) x = x->leftChild; 
			else x = x->rightChild; 
		}
	 
		z->parent = y; 			//set parent of new node/ root if necessary 
		if(y == nil) root = z; 
		else if(z->key < y->key){ y->leftChild = z; }
		else {y->rightChild = z; }
						
		struct node* temp = y;		
		while(temp != nil){			//inc subsize
			temp->subSize =  size(temp);
			temp = temp->parent; 
		}
		insertFixUp(z); 		
		
		nodes++; 
	}
	
	
	void insertFixUp(struct node* z){
		struct node* y = new node; 
		while(z->parent->red == true){			//continue until parent is not red 
			if(z->parent == z->parent->parent->leftChild){ //z is to left of grandparent
				y = z->parent->parent->rightChild; 	
				if(y->red == true){			//case 1:  recolor  
					z->parent->red = false; 			
					y->red  = false; 
					z->parent->parent->red = true; 
					z = z->parent->parent; 
				}
				else{
					
					if(z == z->parent->rightChild){	//case two, left rotate z 
					z = z->parent; 
					leftRotate(z); 
					}
				z->parent->red = false; 			//case 3: recolor and right rotate 
				z->parent->parent->red = true; 
				rightRotate(z->parent->parent); 
				}
			}
			else{								//same but opposite side
				y = z->parent->parent->leftChild; 
				
				if(y->red == true){
					z->parent->red = false; 
					y->red  = false; 
					z->parent->parent->red = true; 
					z = z->parent->parent; 
				
				}
				else {
					if(z == z->parent->leftChild){
						cout<<"case 4"<<endl;
						z = z->parent; 
						rightRotate(z); 
					}
					
					z->parent->red = false; 
					z->parent->parent->red = true; 
					leftRotate(z->parent->parent); 
				}
			}
			
		}
		
		root->red = false; 
	}
	void leftRotate(struct node* x){
		struct node* y = new node; 
		y = x->rightChild; 
		
		x->rightChild = y->leftChild; 
		
		if(y->leftChild != nil) y->leftChild->parent = x; 
		y->parent = x->parent; 
		if(x->parent == nil) root = y; 
		else if(x == x->parent->leftChild) x->parent->leftChild = y; 
		else x->parent->rightChild = y; 
		y->leftChild = x; 
		x->parent = y; 
		y->subSize = x->subSize;  
		x->subSize = size(x); 
	}
	int size(struct node* x){
		if(x == nil) return 0; 
		struct node* y = x->leftChild; 
		struct node* z = x->rightChild; 
		
		if(y != nil) y->subSize = y->leftChild->subSize + y->rightChild->subSize+1;
		if(z != nil) z->subSize = z->leftChild->subSize + z->rightChild->subSize+1;
		x->subSize = x->leftChild->subSize + x->rightChild->subSize+1; 
		if(x->parent != nil) x->parent->subSize = x->parent->leftChild->subSize + x->parent->rightChild->subSize+1; 
		struct node* a = x->parent; 
		return x->subSize; 
	}
		void rightRotate(struct node* x){
		struct node* y = new node; 
		y = x->leftChild; 
		x->leftChild = y->rightChild; 
		if(y->rightChild != nil) y->rightChild->parent = x; 
		y->parent = x->parent; 
		if(x->parent == nil) root = y; 
		else if(x == x->parent->rightChild) x->parent->rightChild = y; 
		else x->parent->leftChild = y; 
		y->rightChild = x; 
		x->parent = y; 
		y->subSize = x->subSize; 
		x->subSize = size(x); 
	}
	
	valuetype * search(keytype k){
		struct node* x = new node; 
		x = root; 
		while(x != nil){
			
			if(x->key == k) return &x->value; 
			if(k < x->key) x = x->leftChild; 
			else x = x->rightChild; 
		}
		
		return NULL; 
	}
	
	int rank(keytype k){
		struct node* n = new node; 
		bool keyFound = false; 
		n = root;  
		
		while(n != nil){							//find key 
			if(n->key == k){ keyFound = true; break;}
			else if(k<n->key) n = n->leftChild; 
			else n = n->rightChild; 
		}
		if(!keyFound)return 0; 
		int r = size(n->leftChild)+1;
		struct node* y = new node; 
		y = n; 
		while(y!=root){	//if y is right child , add subtree size of parent's left side
			if(y == y->parent->rightChild) r = r + size(y->parent->leftChild)+1;
			y = y->parent; 
		}
		
		return r; 
	}
void inorder() 
{ 	
	inorderR(root);
	cout<<endl;
} 
void inorderR(struct node* n){
 if (n == nil) return; 
	
    inorderR(n->leftChild); 
	cout<<n->key<<" ";
    inorderR(n->rightChild);
}

void preorder() 
{ 	
    preOrderR(root); 
	cout<<endl;
} 
void preOrderR(struct node* n){
	 if (n == nil) return; 
    cout << n->key << " "; 
    preOrderR(n->leftChild); 
    preOrderR(n->rightChild);
}
void postorder() 
{ 	
   postorderR(root); 
   cout<<endl;
} 
void postorderR(struct node* n){
	 if (n == nil) return; 
    postorderR(n->leftChild); 
    postorderR(n->rightChild);
	cout << n->key << " "; 
}
keytype *successor(keytype k){
	struct node* n = new node; 
	n = root; 
	while(n != nil){ //locate node
		if(n->key == k) break; 
		else if(k<n->key) n = n->leftChild; 
		else n = n->rightChild; 
		}

	struct node* suc = new node; 
	if(n->rightChild != nil){
		suc = n->rightChild; 	//traverse one node to the right child. 
		n = n->rightChild;	
		while(n->leftChild != nil){	//first left node is successor (right node if none exist); 
			n = n->leftChild; 
			suc = n; 
			
		}
		return &suc->key; 
	}
	else if(n->parent != nil){	//traverse up the tree until current node becomes left child 
		suc = n->parent;
		while(suc != nil){
			if(suc->leftChild == n) return &suc->key; 
			n = n->parent; 
			suc = suc->parent; 
		}
	}
	return nullptr;  
}

keytype *predecessor(keytype k){
	struct node* n = new node; 
	n = root; 
	while(n != nil){
		if(n->key == k) break; 
		else if(k<n->key) n = n->leftChild; 
		else n = n->rightChild; 
		}

	struct node* pred = new node; 
	if(n->leftChild != nil){
		pred = n->leftChild; 		//traverse one node to the left child
		n = n->leftChild;
		while(n->rightChild != nil){	//first right node is successor (left node if none exist)
			n = n->rightChild; 
			pred = n; 
			
		}
		return &pred->key; 
	}
	else if(n->parent != nil){ //traverse up the tree until current node becomes right child 
		pred = n->parent;
		while(pred != nil){
			if(pred->rightChild == n) return &pred->key; 
			n = n->parent; 
			pred = pred->parent; 
		}
	}
	return nullptr;  
}
keytype select(int pos){
	
	if(pos <= nodes) return selectRecursion(root, pos);  
	return 0; 
}
keytype selectRecursion(struct node* n, int i){ 
	int x = 1; 		//recur until parent's left subtree size == pos
	if(n!= nil)	 x = size(n->leftChild)+ 1; 
	if(i == x) return n->key; 
	else if(i < x) return selectRecursion(n->leftChild, i); 
	else if(i>x) return selectRecursion(n->rightChild, i-x); 
}
int size(){
	return nodes; 
}

int remove(keytype k){
	struct node* y = new node; 
	struct node* z = new node; 
	struct node* x = new node; 
	bool isRed = false; 

	z = root; 
	while(z != nil){ 		//locate key 
			if(z->key == k) break; 
			else if (k<z->key) z = z->leftChild; 
			else z = z->rightChild; 
		}
	
	if (z==nil) return 0; 
	
	
	y = z; 
	
	isRed = y->red; 
	if(z->leftChild == nil){		//z has no left children, transplant rightchild 
		x = z->rightChild; 
		transplant(z, z->rightChild);	
	}
	else if(z->rightChild == nil){	//z has no right children, transplant leftchild
		x = z->leftChild; 
		transplant(z, z->leftChild);	
		}
	
	else {
		y = min(z->rightChild); 		//z has at least two children so find smallest child on right side
		isRed = y->red; 
		x = y->rightChild; 
		if(y->parent == z) x->parent = y; 	//if z only has one right child
		else{								//else move y into z's position
			transplant(y, y->rightChild);
			y->rightChild = z->rightChild; 
			y->rightChild->parent = y; 
			y->rightChild->subSize = size(y->rightChild); 
		}
			transplant(z, y);
			y->leftChild = z->leftChild; 
			z->leftChild->parent = y; 
			y->red = z->red;
			y->subSize = size(y); 
			
	}
	decrementSubtree(z);
	if(isRed == false) deleteFix(x); 
	nodes--; 
	 
	return 1; 
}
void decrementSubtree(struct node* x){
	while(x != nil){
		x->subSize = size(x)-1;
		x = x->parent;
	}
}
void deleteFix(struct node* x){
	
	while(x != root && x->red == false){	//while x is not the root and x is black 
	
	struct node* w = new node; 
	if(x == x->parent->leftChild){		//uncle is left child 
		w = x->parent->rightChild; 
		if(w->red == true){		//node is red 
			w->red = false; 	//node becomes black
			x->parent->red = true;  //gparent is red 
			leftRotate(x->parent); //rotate gparent
			w = x->parent->rightChild; 
		}
		if(w->leftChild->red == false && w->rightChild->red == false){ //children both black 
			w->red = true; 	//node is red
			x = x->parent; 
		}
		else{ 
 
			if (w->rightChild->red  == false){		//right child black 
			w->leftChild->red = false; 		//left child = black 
			w->red = true; 					//node becomes red
			rightRotate(w); 				//rotate node
			w = x->parent->rightChild; 		
			}
			w->red = x->parent->red; 		//node becomes color of gparent
			x->parent->red = false; 		//gparent = black 
			w->rightChild->red = false; 	//rightchild black 
			leftRotate(x->parent);			//rotate gparent
			x = root; 						
			}
					//same but left and right flipped
	}
	else{
		x->subSize = size(x); 
			w = x->parent->leftChild; 
		if(w->red == true){
			w->red = false; 
			x->parent->red = true; 
			rightRotate(x->parent); 
			w = x->parent->leftChild; 
			w->subSize = size(w); 
		}
		if(w->rightChild->red == false && w->leftChild->red == false){
			w->red = true; 
			x = x->parent; 
		}
		else{ 
			if (w->leftChild->red  == false){
			w->rightChild->red = false; 
			w->red = true; 
			leftRotate(w); 
			w = x->parent->leftChild;  
			}
			
			w->red = x->parent->red; 
			x->parent->red = false; 
			w->leftChild->red = false;
			rightRotate(x->parent);
			x = root; 
			}
	}
	}
	x->red = false; 

}
void transplant(struct node* x, struct node* y){
	y->parent = x->parent; 
	y->subSize = size(x)-1; 
	if(x->parent == nil) {
		 
		root = y; 
		root->subSize = size(root) - 1; 
	}
	
	else if(x == x->parent->leftChild) { x->parent->leftChild = y; if(y->parent != nil) y->parent->subSize = size(y->parent); }
	else {x->parent->rightChild = y; if(y->parent != nil) y->parent->subSize = size(y->parent);}
}

struct node* min(struct node* z){
	while(z->leftChild != nil){ 
	z->parent->subSize = size(z->parent); 
		z = z->leftChild; 
	}
	return z; 
}


};
