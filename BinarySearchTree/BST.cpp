#include "BST.h"

// constructor, initialize class variables and pointers here if need.
BST::BST(){
  root_ptr_ = shared_ptr<bst_node>(NULL);
}

//deconstructor,
BST::~BST(){
}

shared_ptr<bst_node> BST::InitNode(int data){
    
    shared_ptr<bst_node> ret(new bst_node);
    ret->data = data;
    ret->left = shared_ptr<bst_node>(NULL);
    ret->right = shared_ptr<bst_node>(NULL);
    return ret;
}

void BST::Insert(shared_ptr<bst_node> new_node){
    
    int data = new_node->data;
    if (root_ptr_ == nullptr) {
        root_ptr_ = new_node;
    }
    else {
        shared_ptr<bst_node> curNode = root_ptr_;
        int found = 0;
        while (found == 0) {
            if (data < curNode->data) {
                if (curNode->left == nullptr) {
                    curNode->left = new_node;
                    found = 1;
                }
                else {
                    curNode = curNode->left;
                }
            }
            else {
                if (curNode->right == nullptr) {
                    curNode->right = new_node;
                    found = 1;
                }
                else {
                    curNode = curNode->right;
                }
            }
        }
    }
}

void BST::InsertData(int data){
    
    shared_ptr<bst_node> ret(new bst_node);
    ret->data = data;
    ret->left = shared_ptr<bst_node>(NULL);
    ret->right = shared_ptr<bst_node>(NULL);
    
    if (root_ptr_ == nullptr) {
        root_ptr_ = ret;
    }
    else {
        shared_ptr<bst_node> curNode = root_ptr_;
        int found = 0;
        while (found == 0) {
            if (data < curNode->data) {
                if (curNode->left == nullptr) {
                    curNode->left = ret;
                    found = 1;
                }
                else {
                    curNode = curNode->left;
                }
            }
            else {
                if (curNode->right == nullptr) {
                    curNode->right = ret;
                    found = 1;
                }
                else {
                    curNode = curNode->right;
                }
            }
        }
    }
}

void BST::removenode(shared_ptr<bst_node> mark, shared_ptr<bst_node> marksdad) {
    
    // Code to delete leaf, root_ptr_ or not
    if (mark->left == nullptr and mark->right == nullptr) {
        if (mark == root_ptr_) {
            root_ptr_ = shared_ptr<bst_node>(NULL);
        }
        else { // mark is not root pointer, doesnt matter if dad is root (Trust me)
            if (marksdad->data > mark->data) {
                marksdad->left = shared_ptr<bst_node>(NULL);
            }
            else { // dad smaller or equal to mark
                marksdad->right = shared_ptr<bst_node>(NULL);
            }
        }
    }
    
    // Code to delete a node with a single child, root_ptr_ or not
    else if (mark->left == nullptr or mark->right == nullptr) {
        // find mark's child
        shared_ptr<bst_node> child;
        if (mark->left == nullptr) {
            child = mark->right;
        }
        else {
            child = mark->left;
        }
        // assign mark's dad to the child
        if (mark == root_ptr_) {
            root_ptr_ = child;
        }
        else { // mark is not root
            if (marksdad->data > mark->data) {
                marksdad->left = child;
            }
            else { // mark is right child
                marksdad->right = child;
            }
        }
    }
    
    // Code to delete a node with two children
    else {
        // find the successor and its parent
        shared_ptr<bst_node> buddy = mark->right;
        shared_ptr<bst_node> budparent = mark;
        while (buddy->left != nullptr) {
            budparent = buddy;
            buddy = buddy->left;
        }
        // copy successor data into mark
        mark->data = buddy->data;
        // recursively remove buddy from the tree
        removenode(buddy, budparent);
    }
    return;
}

void BST::Remove(int data) {
    
    shared_ptr<bst_node> curNode;
    shared_ptr<bst_node> preNode = root_ptr_;
    curNode = GetNode(root_ptr_, data);
    if (curNode == nullptr) {
        return;
    }
    while (curNode != root_ptr_ and preNode->left != curNode and preNode->right != curNode) {
        if (preNode->data > curNode->data) {
            preNode = preNode->left;
        }
        else {
            preNode = preNode->right;
        }
    }
    removenode(curNode, preNode);
}

bool BST::Contains(shared_ptr<bst_node> subt, int data){
    
    shared_ptr<bst_node> curNode = subt;
    if (curNode == nullptr) {
        return false;
    }
    
    int found_end = 0;
    while (found_end == 0) {
        if (curNode->data == data) {
            return true;
        }
        if (data < curNode->data) {
            if (curNode->left == nullptr) {
                found_end = 1;
            }
            else {
                curNode = curNode->left;
            }
        }
        else {
            if (curNode->right == nullptr) {
                found_end = 1;
            }
            else {
                curNode = curNode->right;
            }
        }
    }
    return false;
}

shared_ptr<bst_node> BST::GetNode(shared_ptr<bst_node> subt, int data){
    
    shared_ptr<bst_node> curNode = subt;
    if (curNode == nullptr) {
        return shared_ptr<bst_node>(NULL);
    }
    
    int found_end = 0;
    while (found_end == 0) {
        if (curNode->data == data) {
            return curNode;
        }
        if (data < curNode->data) {
            if (curNode->left == nullptr) {
                found_end = 1;
            }
            else {
                curNode = curNode->left;
            }
        }
        else {
            if (curNode->right == nullptr) {
                found_end = 1;
            }
            else {
                curNode = curNode->right;
            }
        }
    }
    
    
    return shared_ptr<bst_node>(NULL);
}

int BST::inordercnt(shared_ptr<bst_node> subt, int count) {
    if (subt == nullptr) {
        return count;
    }
    count = inordercnt(subt->left, count);
    count++;
    count = inordercnt(subt->right, count);
    
    return count;
}

int BST::Size(shared_ptr<bst_node> subt){
  
    int size = 0;
    size = inordercnt(subt, size);
    
    if (size > 0) {
        return size;
    }
    
    return 0;
}

void BST::ToVector(shared_ptr<bst_node> subt, vector<int>& vec){
    if (subt == nullptr) {
        return;
    }
    ToVector(subt->left, vec);
    vec.push_back(subt->data);
    ToVector(subt->right, vec);
}

// This function is implemented for you
// Returns the root pointer
shared_ptr<bst_node> BST::GetRoot(){
  return root_ptr_;
}

// This function is implemented for you
// sets a given pointer as the top pointer
void BST::SetRoot(shared_ptr<bst_node> root_ptr){
  root_ptr_ = root_ptr;
}