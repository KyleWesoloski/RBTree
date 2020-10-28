#include "CDA.cpp"

#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

template <typename keytype, typename valuetype>
class Heap {
	struct node {
	keytype key;
	valuetype value; 
	};
	
public:
	int nodes = 0;
	CDA<node> H;
	Heap();
	Heap(keytype k[], valuetype V[], int s); 
	Heap(Heap& obj);
	~Heap();
    keytype peekKey(); 
	valuetype peekValue(); 
	keytype extractMin();
	void insert(keytype k, valuetype v); 
	void printKey();

	Heap& operator=(const Heap& src) {
		if (this != &src) {
			deepCopy(src);
		}
		return *this;
	}
private:
	void deepCopy(const Heap& src);
	void bottomUp(int n, int i); 
};

template <typename keytype, typename valuetype>
Heap<keytype,valuetype>::Heap(keytype k[], valuetype V[], int s){
		struct node null; 
		H.AddEnd(null); 
		nodes = s; 
		for(int i = 0; i<s; i++){
			struct node newNode; 
			newNode.key = k[i];
			newNode.value = V[i]; 
			H.AddEnd(newNode); 
		}
		
		for(int i = (s/2) ; i > 0; i--) bottomUp(s, i); 
	}
	
template <typename keytype, typename valuetype>
void Heap<keytype,valuetype>::bottomUp(int n, int i){
	int m = i; 
	int l = 2*i; 
	int r = 2*i +1;
	if(l<=n && H[l].key < H[m].key) m =l; 		//fin min between child and parent
	if(r<=n && H[r].key < H[m].key) m =r;
	if(m!=i){						//swap parent and child, recur 
		struct node temp; 
		temp.key = H[i].key;
		temp.value = H[i].value;
		H[i].key = H[m].key; 
		H[i].value = H[m].value; 
		H[m].key = temp.key; 
		H[m].value = temp.value; 
		bottomUp(n,m); 
	}	
}
	
template <typename keytype, typename valuetype>
Heap<keytype,valuetype>::Heap(){
		struct node null; 
		H.AddEnd(null); 
	}
template <typename keytype, typename valuetype>	
Heap<keytype,valuetype>::Heap(Heap& obj) {
		deepCopy(obj);
	}
	

template <typename keytype, typename valuetype>
void Heap<keytype,valuetype>::deepCopy(const Heap& src) {
		H = src.H;
		nodes = src.nodes; 
	}
template <typename keytype, typename valuetype>
Heap<keytype,valuetype>::~Heap(){
}
	
template <typename keytype, typename valuetype>
void Heap<keytype,valuetype>::insert(keytype k, valuetype v){
	struct node x; 
	struct node newNode; 
	newNode.key = k;
	newNode.value = v; 
	nodes++; 
	
	H.AddEnd(newNode);	
	if(nodes>1){
		int i = nodes; 
		while(newNode.key < H[i/2].key){		//swap with parent if key is < parent key 
			x = H[i/2]; 
			H[i/2] = newNode; 
			H[i] = x; 
			i = i/2; 
		}
	}
	
	
}

template <typename keytype, typename valuetype>
keytype Heap<keytype,valuetype>::peekKey(){
	return H[1].key; 
}

template <typename keytype, typename valuetype>
valuetype Heap<keytype,valuetype>::peekValue(){
	return H[1].value; 
}

template <typename keytype, typename valuetype>
keytype Heap<keytype,valuetype>::extractMin(){
	keytype k = H[1].key; 
	struct node end = H[nodes]; 
	H[1] = H[nodes]; 
	H.DelEnd(); 
	int i = 1; 
	nodes--; 
	if(nodes == 2){
		if(H[1].key>H[2].key){
			end = H[1]; 
			H[1] = H[2]; 
			H[2] = end; 
		}
		else{
			end = H[2]; 
			H[2] = H[1]; 
			H[1] = end; 
		}
		return k; 
	}
	while(1){
		
		struct node x; 
		int c = 0; 
		if(i*2 <nodes && (i*2)+1 < nodes+1){
			if(H[(i*2)+1].key < H[(i*2)].key) c = 1; 
			else c = 2; 
		}			
		else if(i*2 < nodes) c =1; 
		else break;
		switch(c){
		case 1:							//swap current node with right child
			if(end.key > H[(i*2)+1].key){
			x = H[(i*2)+1]; 
			H[(i*2)+1] = end; 
			H[i] = x; 
			}
			i = (i*2) + 1;
			break; 
		case 2:						//swap current node with left child
			if(end.key > H[(i*2)].key){
			x = H[(i*2)]; 
			H[(i*2)] = end; 
			H[i] = x; 
			}
			i = (i*2);
			break; 

		default: break;
		}		
	}
	
	return k; 
}

template <typename keytype, typename valuetype>
void Heap<keytype,valuetype>::printKey(){
	for(int i = 1; i <= nodes; i++){
		cout<<H[i].key<<" ";
	}
	if(nodes>0)cout<<"\n"<<endl;
}

