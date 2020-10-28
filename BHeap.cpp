#include "CDA.cpp"

#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;


//==========BHeap==========//


template <typename keytype, typename valuetype>
class BHeap {
	

public:
	int nodes = 0;
	BHeap(); 
	BHeap(keytype k[], valuetype V[], int s); 
	BHeap(BHeap& obj);
	~BHeap();
	keytype peekKey(); 
	valuetype peekValue(); 
	keytype extractMin(); 
	void insert(keytype k, valuetype v); 
	void merge(BHeap<keytype,valuetype> &H2);
	void printKey();
	
	BHeap& operator=(const BHeap& src) {
		if (this != &src) {
			deepCopy(src);
		}
		return *this;
	}
	
private:
	struct node {
	keytype key;
	valuetype value; 
	struct node* next = nullptr;
	struct node* prev = nullptr; 
	//struct node** child = new node*; 
	CDA<struct node*> child;
	int subtrees = 0; 
	int degree = 0; 
	};
	int trees =0; 
	struct node* min;// = new node; 
	void traverse(struct node* c); 
	void deepCopy(const BHeap& src);
	
	
};

template <typename keytype, typename valuetype>
BHeap<keytype,valuetype>::BHeap(){
		min = new node; 
	}
template <typename keytype, typename valuetype>
BHeap<keytype,valuetype>::~BHeap(){
		struct node* c = min; 
		for(int i = 0; i < trees; i++) {
			node* n = c->next; 
			delete c; 
			c = n; 
		}			
	}
template <typename keytype, typename valuetype>
BHeap<keytype,valuetype>::BHeap(keytype k[], valuetype V[], int s){ 
		min = new node; 
		for(int i = 0; i < s; i++) insert(k[i],V[i]);  
	}
template <typename keytype, typename valuetype>	
BHeap<keytype,valuetype>::BHeap(BHeap& obj) {
		if(this !=&obj) deepCopy(obj);
	}
	

template <typename keytype, typename valuetype>
void BHeap<keytype,valuetype>::deepCopy(const BHeap& src) {
		min = new node; 
		struct node* temp = min; 
		struct node* p = temp; 
		struct node* source= src.min;
		trees = src.trees; 
		
		for(int i= 0; i<trees; i++){
			temp->key = source->key; 
			temp->value = source->value;  
			temp->child = source->child; 
			temp->subtrees = source->subtrees; 
			temp->degree = source->degree; 
			temp->prev = new node; 
			temp->next = new node; 
			if(i>0) {	
				temp->prev = p; 
				p->next = temp; 
			}
			p = temp; 
			temp = temp->next; 
			source = source->next; 
		}
	}

template <typename keytype, typename valuetype>
void BHeap<keytype,valuetype>::insert(keytype k, valuetype v){
	nodes++; 
	if(trees == 0) {
		trees++;
		min->key = k; 
		min->value = v; 
		min->degree = 0; 
	}
	else {
	trees++; 
	min->prev = new node; 
	min->prev->key = k; 
	min->prev->value = v; 
	min->prev->degree = 0; 
	min->prev->next = min; 
	min = min->prev; 
	struct node* temp = min;
	int total = trees; 
	for(int i =0; i < total-1; i++){
		if(temp->degree == temp->next->degree){			//case 1 x and next x are same 
			if(i == total-2 || temp->next->degree != temp->next->next->degree){  //case 2: next and next next x are same	
				if(temp->key < temp->next->key){	 //case 3: x is < to the key of next x
					if(temp->next == min) min = temp; 
					if(temp->next->next != nullptr)temp->next->next->prev = temp; 
					temp->child.AddFront(temp->next); 
					temp->next = temp->next->next; 
					temp->subtrees++; 
					temp->degree++; 
					
				}
				else{								 //case 4: x key is > x next nkey then make  
					if(temp == min) min = temp->next; 
					temp->next->child.AddFront(temp);
					if(temp->prev != nullptr)temp->prev->next = temp->next; 
					temp->next->prev = temp->prev; 
					temp->next->subtrees++; 
					temp->next->degree++; 
					temp = temp->next;
				}
				trees--; 
			}
			else temp = temp->next;
		}
		else temp = temp->next; 			
	}
	}
	//delete newNode; 
}
template <typename keytype, typename valuetype>
void BHeap<keytype,valuetype>::merge(BHeap<keytype,valuetype> &H2){ 
	int total = trees + H2.trees; 
	struct node* temp = min;
	temp = min; 
	struct node* newTemp = H2.min;
	newTemp = H2.min; 

	CDA<node*> array; 
	
	int i =0; 
	int j =0;
	
	while(i<trees && j<H2.trees){		//sort heaps while both have remaining roots 
		if(temp->degree<newTemp->degree){
			array.AddEnd(temp); 
			temp = temp->next; 
			if(i+j>0){
				array[i+j-1]->next = array[i+j]; 
				array[i+j]->prev = array[i+j-1];			
			}
			i++;
		}
		else{
			array.AddEnd(newTemp); 
			newTemp = newTemp->next;
			if(i+j>0){
				array[i+j-1]->next = array[i+j]; 
				array[i+j]->prev = array[i+j-1];			
			}
			j++; 
		}
	}
		
	while(i<trees){				//fill in remaining roots in order
		array[i+j] = temp;
			array.AddEnd(temp); 
	    temp = temp->next;
	
		if(i+j>0){
			array[i+j-1]->next = array[i+j];
			array[i+j]->prev = array[i+j-1];
		}
		i++; 
	}
	while(j<H2.trees){
		array.AddEnd(newTemp); 
		newTemp = newTemp->next;
		if(i+j>0){
			array[i+j-1]->next = array[i+j];
			array[i+j]->prev = array[i+j-1];
		}
		j++; 
	}
	min = array[0]; 
	trees = total;
	temp = min;
	
	
	temp = min; 
	for(int i =0; i < total-1; i++){
		if(temp->degree == temp->next->degree){		//case 1: x and next x same done		
			if(i == total-2 || temp->next->degree != temp->next->next->degree){   //case 2: next and next next x are not the same	
				if(temp->key < temp->next->key){//case 3: x key is < to key of next x
					if(temp->next == min) min = temp; 
					if(temp->next->next != nullptr)temp->next->next->prev = temp; 
					temp->child.AddFront(temp->next); 
					temp->next = temp->next->next; 
					temp->subtrees++; 
					temp->degree++; 
					
				}
				else{								 //case 4: x is > then next x
					if(temp == min) min = temp->next; 
					temp->next->child.AddFront(temp);
					if(temp->prev != nullptr)temp->prev->next = temp->next; 
					temp->next->prev = temp->prev; 
					temp->next->subtrees++; 
					temp->next->degree++; 
					temp = temp->next;
				}
				trees--; 
			}
			else temp = temp->next;
		}
		else temp = temp->next; 	
	}
	temp = min; 

}
template <typename keytype, typename valuetype>
void BHeap<keytype,valuetype>::printKey(){
	struct node* temp = min; 
	
	for(int i = 0; i < trees; i++){
		cout<<"B"<<temp->degree<<endl;
		cout<<temp->key<<" ";
		traverse(temp);
		temp = temp->next;
		cout<<"\n"<<endl;
	}
		
}
template <typename keytype, typename valuetype>
void BHeap<keytype,valuetype>::traverse(struct node* c){
	
	for(int i = 0; i<c->subtrees; i++){
		cout<<c->child[i]->key<<" "; 
		traverse(c->child[i]); 
		
		}
	
}
template <typename keytype, typename valuetype>
keytype BHeap<keytype,valuetype>::extractMin(){
	
	struct node* small = min; 					//find min node in tree
	struct node* temp = min; 
	for(int i = 0; i <trees; i++){
		if(temp->key < small->key) small = temp; 
		temp = temp->next;
	}
	if(small == min) min = small->next; 
	if(small->prev != nullptr) small->prev->next = small->next;  //change pointers in main heap 
	if(small->next != nullptr) small->next->prev = small->prev; 
	trees--; 
	
	BHeap<keytype,valuetype> t; 				//set min value of new heap
	if(small->subtrees>0){
	t.min->key = small->child[small->subtrees-1]->key;
	t.min->value = small->child[small->subtrees-1]->value;
	t.min->degree = small->child[small->subtrees-1]->degree;
	t.min->child = small->child[small->subtrees-1]->child;
	
	temp = t.min;
	}
	struct node* p = temp; 
	for(int i = small->subtrees-2; i >= 0; i--){	//set remainder of min heap
		temp->next = new node; 
		temp->next->key = small->child[i]->key;
		temp->next->value = small->child[i]->value; 
		temp->next->degree = small->child[i]->degree; 
		temp->next->child = small->child[i]->child;	
		if(i != small->subtrees-2){
			temp->prev = p; 
			p->next = temp; 
			p = temp;
		}			
		temp = temp->next;
	}

	t.trees = small->subtrees; 				//update main tree size				
	merge(t); 
	//merge new trees 
	return small->key; 
}

template <typename keytype, typename valuetype>
keytype BHeap<keytype,valuetype>::peekKey(){
	struct node* small = min; 					//find min node in tree
	struct node* temp = min; 
	for(int i = 0; i <trees; i++){
		if(temp->key < small->key) small = temp; 
		temp = temp->next;
	}
	return small->key; 
}

template <typename keytype, typename valuetype>
valuetype BHeap<keytype,valuetype>::peekValue(){
	struct node* small = min; 					//find min node in tree
	struct node* temp = min; 
	for(int i = 0; i <trees; i++){
		if(temp->key < small->key) small = temp; 
		temp = temp->next;
	}
	return small-> value; 
}	


