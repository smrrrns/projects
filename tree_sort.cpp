#include <iostream>
#include <stdlib.h>

using namespace std;

typedef struct node {
    int value;
    //struct node *parent;
    struct node *left;
    struct node *right;
} node;

node* node_get(int value)//, node* parent)
{
    node* result = (node*) malloc(sizeof(node));
    result->left = NULL;
    result->right = NULL;
    result->value = value;
    //result->parent = parent;
    return result;
}

void node_insert(node **root, int value){
    if(*root == NULL){
        *root = node_get(value);//, NULL);
        return;
    }
    node *temp = NULL;
    temp = *root;
    while(temp){
        if (value > (temp->value)){
            if (temp->right){
                temp = temp->right;
                continue;
            }
            else {
                temp->right = node_get(value);//, temp);
                return;
            }
        }
        else if(value < (temp->value)){
            if(temp->left){
                temp = temp->left;
                continue;
            }
            else{
                temp->left = node_get(value);//, temp);
                return;
            }
        }
        else
            exit(2);
    }
}

void tree_print(node *tree)
{
  if (tree) {      
    tree_print(tree->left); 
    cout << tree->value << " "; 
    tree_print(tree->right);
  }
}

int main(){
    int n;
    cout << "enter number of elements: ";
    cin >> n;
    //5
    int arr[15] = {12,3,1,8,2,5,7,6,4,10,15,11,13,9,14};
    cout << "enter elements: ";
    node* tree = NULL;
    for(int i = 0; i<n; i++) {
        int el;
        cin >> el;
        node_insert(&tree, el);
    }
    tree_print(tree); 
    return 0;
}