#include "BTree.h"

// constructor, initialize class variables and pointers here if need.
BTree::BTree(){
  // Your code here
}

//deconstructor,
BTree::~BTree(){
}

shared_ptr<btree> BTree::initialize_node() {
  shared_ptr<btree> ret = shared_ptr<btree>(new btree);
  ret->num_keys = 0;
  ret->is_leaf = true;
  for (int i=0; i <= BTREE_ORDER; i++) {
    ret->children[i] = NULL;
  }
  return ret;
}

shared_ptr<btree> BTree::create_node(int size, int* keys) {
  shared_ptr<btree> node = initialize_node();
  node->num_keys = size;
  for (int i=0; i < node->num_keys; i++) {
    node->keys[i] = keys[i];
  }
  return node;
}

shared_ptr<btree> BTree::find_leaf(shared_ptr<btree> root, int key) {
    if (root == nullptr or root->is_leaf == true) {
        return root;
    }
    bool found = false;
    int active_keys = root->num_keys;
    int child_index = active_keys;
        
    // find out where in this node the key would fit
    for (int i = 0; i < active_keys; i++) {
        // if the key is in this node
        if (root->keys[i] == key) {
            found = true;
            break;
        }
        // if we reach a key that is larger than our key
        if (root->keys[i] > key) {
            child_index = i;
            break;
        }
    }
    // if found was switched on, return null
    if (found == true) {
        return nullptr;
    }
    // otherwise, check the appropriate child (marked by index)
    // if the proper child is null
    if (root->children[child_index] == nullptr) {
        return root;
    }
    // if the proper child is not null
    return find_leaf(root->children[child_index], key);
}

shared_ptr<btree> BTree::find_parent(shared_ptr<btree> curNode, shared_ptr<btree> root) {
    // if curNode and root are one and the same
    if (curNode->keys == root->keys) {
        return nullptr;
    }
    int curKey = curNode->keys[0];
    int active_keys = root->num_keys;
    int child_index = active_keys;   
    // find out where in this node the key would fit
    for (int i = 0; i < active_keys; i++) {
        // if we reach a key that is larger than our key
        if (root->keys[i] > curKey) {
            child_index = i;
            break;
        }
    }
    // if root's child is curNode
    if (root->children[child_index]->keys == curNode->keys) {
        return root;
    }
    // if root is a grandparent of curNode
    else {
        return find_parent(curNode, root->children[child_index]);
    }
}

void BTree::move_and_split(shared_ptr<btree> curNode, shared_ptr<btree> root) {
    // if curNode is the root
    if (curNode->keys == root->keys) {
        // create root's new children
        shared_ptr<btree> new_node1 = initialize_node();
        new_node1->num_keys = 2;
        new_node1->keys[0] = root->keys[0];
        new_node1->keys[1] = root->keys[1];
        new_node1->children[0] = root->children[0];
        new_node1->children[1] = root->children[1];
        new_node1->children[2] = root->children[2];
        shared_ptr<btree> new_node2 = initialize_node();
        new_node2->num_keys = 2;
        new_node2->keys[0] = root->keys[3];
        new_node2->keys[1] = root->keys[4];
        new_node2->children[0] = root->children[3];
        new_node2->children[1] = root->children[4];
        new_node2->children[2] = root->children[5];
        // if the root is not a leaf, neither are it's new children
        if (root->is_leaf == false) {
            new_node1->is_leaf = false;
            new_node2->is_leaf = false;
        }
        // update the keys, children, and is_leaf of the new root node
        root->num_keys = 1;
        root->keys[0] = root->keys[2];
        for (int i = 1; i <= 4; i++) {
            root->keys[i] = 0;
        }
        root->is_leaf = false;
        root->children[0] = new_node1;
        root->children[1] = new_node2;
        for (int j = 2; j <= 5; j++) {
            root->children[j] = nullptr;
        }
        return;
    }
    // if curNode is not the root
    else {
        // gotta find the parent node
        shared_ptr<btree> parent = find_parent(curNode, root);
        // create a new node for the right split of curNode
        shared_ptr<btree> new_node = initialize_node();
        new_node->num_keys = 2;
        new_node->keys[0] = curNode->keys[3];
        new_node->keys[1] = curNode->keys[4];
        new_node->children[0] = curNode->children[3];
        new_node->children[1] = curNode->children[4];
        new_node->children[2] = curNode->children[5];
        if (curNode->is_leaf == false) {
            new_node->is_leaf = false;
        }
        // find appropriate spot for key2 in parent
        int key2 = curNode->keys[2];
        int num_children = parent->num_keys + 1;
        int the_spot;
        for (int spot = 0; spot < num_children; spot++) {
            if (parent->children[spot]->keys == curNode->keys) {
                the_spot = spot;
                break;
            }
        }
        // shift over keys and children of parent to make room
        parent->num_keys++;
        int in2move = 3;
        while (in2move >= the_spot) {
            parent->keys[in2move+1] = parent->keys[in2move];
            parent->children[in2move+2] = parent->children[in2move+1];
            in2move--;
        }
        // delete right half of node to be split
        curNode->num_keys = 2;
        for (int k = 2; k < 5; k++) {
            curNode->keys[k] = 0;
            curNode->children[k+1] = nullptr;
        }
        // insert key2 and new node
        parent->keys[the_spot] = key2;
        parent->children[the_spot+1] = new_node;
        // if overfull, recursively call move_and_split
        if (parent->num_keys < 5) {
            return;
        }
        else {
            move_and_split(parent, root);
        }
    }
}

void BTree::insert(shared_ptr<btree> root, int key){
    // find the leaf node where this key belongs
    shared_ptr<btree> curNode = find_leaf(root, key);
    // make sure it's not already anywhere in the tree
    if (curNode == nullptr) {
        return;
    }
    // add this key to the leaf node
    int active_keys = curNode->num_keys;
    int add_index = active_keys;
    for (int i = 0; i < active_keys; i++) {
        if (curNode->keys[i] > key) {
            add_index = i;
            break;
        }
    }
    int key_to_move = active_keys-1;
    while (key_to_move >= add_index) {
        curNode->keys[key_to_move+1] = curNode->keys[key_to_move];
        key_to_move--;
    }
    curNode->keys[add_index] = key;
    curNode->num_keys++;
    // if the node is not overfull, return
    if (curNode->num_keys < 5) {
        return;
    }
    // if it is we need to split and move up recursively
    move_and_split(curNode, root);
    return;
}

void BTree::remove(shared_ptr<btree> root, int key){
    int active_keys = root->num_keys;
    int index = -1;
    for (int i = 0; i < active_keys; i++) {
        if (root->keys[i] == key) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        while (index < 3) {
            root->keys[index] = root->keys[index+1];
            root->children[index+1] = root->children[index+2];
            index++;
        }
    }
}

shared_ptr<btree> BTree::find(shared_ptr<btree> root, int key){
    if (root == nullptr or root->is_leaf == true) {
        return root;
    }
    bool found = false;
    int active_keys = root->num_keys;
    int child_index = active_keys;
        
    // find out where in this node the key would fit
    for (int i = 0; i < active_keys; i++) {
        // if the key is in this node
        if (root->keys[i] == key) {
            found = true;
            break;
        }
        // if we reach a key that is larger than our key
        if (root->keys[i] > key) {
            child_index = i;
            break;
        }
    }
    // if found was switched on, return our current node
    if (found == true) {
        return root;
    }
    // otherwise, check the appropriate child (marked by i)
    // if the proper child is null
    if (root->children[child_index] == nullptr) {
        return root;
    }
    // if the proper child is not null
    return find(root->children[child_index], key);
}

int BTree::count_nodes(shared_ptr<btree> root){
    int count = 1;
    if (root == nullptr) {
        return 0;
    }
    if (root->is_leaf == true) {
        return count;
    }
    for (int i = 0; i <= root->num_keys + 1; i++) {
        count = count + count_nodes(root->children[i]);
    }
    
    return count;
}

int BTree::count_keys(shared_ptr<btree> root) {
    if (root == nullptr) {
        return 0;
    }
    int count = root->num_keys;
    if (root->is_leaf == true) {
        return count;
    }
    for (int i = 0; i <= root->num_keys + 1; i++) {
        count = count + count_keys(root->children[i]);
    }
    return count;
}