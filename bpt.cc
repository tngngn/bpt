/* 
    @bpt implementation
    
    status:
    insert  : done
    delete  : inactive
    search  : inactive
    print   : done

*/

#include <iostream>
#include <vector>
using namespace std;

int ptrNum;

struct node {
    bool isLeaf;
    bool isRoot;
    node *parent;
    vector<int> value;
    vector<node*> child;
    node *next;
};

node* Root = NULL;

node* foundedNode(node *fNode, int k) {                 // traverse until desired leaf node
    if(fNode->isLeaf) return fNode;  
    int i;       
    for(i=0;i<fNode->value.size();i++) {
        if(fNode->value[i] > k) break;
    }
    return foundedNode(fNode->child[i], k);
}

node* newNode(bool isLeaf, bool isRoot) {
    node* temp = new node;
    temp->isRoot = isRoot;
    temp->isLeaf = isLeaf;
    temp->next = NULL;
    return temp;
} 

// INSERT IN PARENT

void insert_in_parent(node *n, int keyPrime, node* nPrime) {
    if(n->isRoot) {                                     // if N is the root of the tree then
        // create a new node R containing N, K',N'
        // make R the root of the tree return
        Root = newNode(false, true);                    
        n->isRoot = false;
        Root->child.push_back(n);
        Root->child.push_back(nPrime);
        Root->value.push_back(keyPrime);
        n->parent = Root;
        nPrime->parent = Root;
    }
    // similar to insert_in_leaf but change a bit for new child pointer
    else {
        node *p = n->parent;                            // let P = parent(N)

        // if P has less than n pointers then insert(K', N') in P just after N
        int i;                            
        for(i=0;i<p->value.size();i++){
            if(keyPrime<p->value[i]) break;
        }
        int tempKey;
        node *tempPtr;
        for(int j = i; j<p->value.size(); j++){
            tempKey = p->value[j];
            tempPtr = p->child[j+1];
            p->value[j] = keyPrime;
            p->child[j+1] = nPrime;
            keyPrime = tempKey;
            nPrime = tempPtr;
        }
        p->value.push_back(keyPrime);
        p->child.push_back(nPrime);
        nPrime->parent = p;

        if(p->child.size()>ptrNum) {                    // else
            node *pPrime = newNode(false, false);       // create node P'
            int keyPrimePrime = p->value[(ptrNum+1)/2]; // Let K'' = T.K[(n+1)/2]

            // copy T.P[[(n+1)/2]+1],...,T.P[n+1] into P'
            for(i=((ptrNum+1)/2)+1;i<p->value.size();i++) {
                pPrime->child.push_back(p->child[i]);
                p->child[i]->parent = pPrime;
                pPrime->value.push_back(p->value[i]);
            }
            pPrime->child.push_back(p->child[i]);
            p->child[i]->parent = pPrime;

            // copy T.P[1],...,T.P[(n+1)/2] into P
            p->value.erase(p->value.begin()+((ptrNum+1)/2),p->value.end());
            p->child.erase(p->child.begin()+(((ptrNum+1)/2)+1),p->child.end());

            insert_in_parent(p,keyPrimePrime,pPrime);   // insert_in_parent(P,K'',P')
        }
    }
}

// INSERT IN LEAF
// if K < L.K1 then insert P, K into L just before L.P1
// else insert P, K into L just after L.Ki (Ki be the highest)

void insert_in_leaf(node *leafNode, int k, node *p) {
    int i;                                           
    for(i=0;i<leafNode->value.size();i++) {
        if(k<leafNode->value[i]) break;
    }
    // temporarily store larger pointers and keys while inserting new key in the middel
    int tempKey;                            
    node *tempPtr;
    for(int j=i;j<leafNode->value.size();j++){
        tempPtr = leafNode->child[j];
        tempKey = leafNode->value[j];
        leafNode->child[j] = p;
        leafNode->value[j] = k;
        p = tempPtr;
        k = tempKey;
    }
    leafNode->child.push_back(p);
    leafNode->value.push_back(k);
}

// MAIN INSERT

void insertion(int k, node *p) {                        // insert(key K, pointer P)
    node *leafNode;
    if(Root == NULL) {                                  // if tree is empty then
        Root = newNode(true, true);                     // create an empty leaf node L,
        leafNode = Root;                                // which is also the root
    }
    else {                                              // else
        leafNode = foundedNode(Root, k);                // find the leaf node L that should contain key value K
    }

    // if L has less than n−1, normal insert
    if(leafNode->value.size() < (ptrNum-1)) insert_in_leaf(leafNode,k,p);
    
    // else, split
    else {                                              
        node* leafNodePrime = newNode(true, false);     // create node L'
        insert_in_leaf(leafNode,k,p);                   // insert_in_leaf(T,K,P)
        leafNodePrime->next = leafNode->next;           // set L'.Pn = L.Pn
        leafNode->next = leafNodePrime;                 // set L.Pn = L'

        // copy T.P[[n/2]+1] through T.K[n] from T into L' starting at L'.P1
        for(int i=(ptrNum/2);i<ptrNum;i++) {
            leafNodePrime->child.push_back(leafNode->child[i]);
            leafNodePrime->value.push_back(leafNode->value[i]);
        }

        // copy T.P[1] through T.K[n/2] from T into L starting at L.P1
        leafNode->value.erase(leafNode->value.begin()+(ptrNum/2),leafNode->value.end());
        leafNode->child.erase(leafNode->child.begin()+(ptrNum/2),leafNode->child.end());

        int keyPrime = leafNodePrime->value[0];         // Let K' be the smallest key in L'
        insert_in_parent(leafNode, keyPrime, leafNodePrime); 
    }
}

/*
// Deletion is in progress

// DELETE ENTRY

void delete_entry(node *n, int k, node *p) {

}

// MAIN DELETE

void deletion(int k, node *p) {
    node *deleteNode;
    deleteNode = foundedNode(Root, k);
    int i;
    for (i=0;i<deleteNode->value.size();i++) {
        if(deleteNode->value[i]==k) {
            delete_entry(deleteNode,k,p);
            return;
        }
    }
    printf("key to delete does not exist.");
    return;
}
*/

// PRINT TREE

void print(node *tNode) {
    printf(" [");
    int i ;
    for(i=0; i<tNode->value.size()-1;i++){
        printf("%d|",tNode->value[i]);
    }
    if(tNode->value.size()>0) printf("%d]",tNode->value[i]);
}

void printTree(node *tNode) {
    if(tNode->isLeaf==true) {
        printf(" [");
        int i ;
        for(i=0; i<tNode->value.size()-1;i++){
            printf("%d|",tNode->value[i]);
        }
        if(tNode->value.size()>0) printf("%d]",tNode->value[i]);
    }
    else {
        printf("\n");
        print(tNode);
        printf("\n");
        int i;
        for(i=0;i<tNode->value.size()+1;i++) {
            printTree(tNode->child[i]);
        }
        printf("\n");
    }
}

int main(){
    ptrNum = 4;
    /*
    int i;
    for(i=0;i<10000000;i++) {
        insertion(i, NULL);
    }
    printTree(Root);
    */
    
    int k;
    while(true){
        printf("\ninput 1 <v> RET to insert v \ninput 0 RET to exit\n");
        int choice;
        scanf("%d",&choice);
        if(choice==1){
            int value;
            scanf("%d",&value);
            insertion(value,NULL);
        
            printf("\n\n\n");
            printTree(Root);
            printf("\n\n\n");
        /*
        }else if(choice==2){
            int value;
            scanf("%d",&value);
            deletion(value,NULL);

            printf("\n\n\n");
            printTree(Root);
            printf("\n\n\n");
        */
        }else if(choice==0) break;
        else {
            printf("\n\n\n");
            printf("wrong input syntax, try again.");
            printf("\n\n\n");
        }

    }
    

    return 0;
}