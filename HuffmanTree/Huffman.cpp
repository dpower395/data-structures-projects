#include "Huffman.h"

// constructor, initialize class variables and pointers here if need.
Huffman::Huffman(){
  // your code here
}

//deconstructor,
Huffman::~Huffman(){
}

shared_ptr<freq_info> Huffman::CreateLeaf(char symbol, int count) {
    shared_ptr<freq_info> ret(new freq_info);
    ret->symbol = symbol;
    ret->count = count;
    ret->left = nullptr;
    ret->right = nullptr;
    ret->is_leaf = true;
    return ret;
}

shared_ptr<freq_info> Huffman::CombineNodes(shared_ptr<freq_info> left, shared_ptr<freq_info> right) {
    shared_ptr<freq_info> ret(new freq_info);
    ret->count = left->count + right->count;
    ret->left = left;
    ret->right = right;
    ret->is_leaf = false;
    return ret;
}

void Huffman::IncrementLookup(map<char, int>& lookup, char symbol) {
    if (lookup.find(symbol) != lookup.end()) {
        lookup[symbol]++;
    }
    else {
        lookup[symbol] = 1;
    }
}

void Huffman::LoadQueue(const map<char, int>& lookup, tree_queue& q) {
    for (auto it = lookup.cbegin(); it != lookup.cend(); ++it) {
        char k = (*it).first;
        int v =  (*it).second;
        shared_ptr<freq_info> new_node = CreateLeaf(k, v);
        q.push(new_node);
    }
}

shared_ptr<freq_info> Huffman::BuildTree(tree_queue& q) {
    while (q.size() > 1) {
        shared_ptr<freq_info> left = q.top();
        q.pop();
        shared_ptr<freq_info> right = q.top();
        q.pop();
        shared_ptr<freq_info> new_node = CombineNodes(left, right);
        q.push(new_node);
    }
    shared_ptr<freq_info> root(new freq_info);
    root = q.top();
    return root;
}

void Huffman::TraverseEncode(shared_ptr<freq_info> node, map<char, string>& table, string code) {
    if (node->is_leaf == true) {
        table[node->symbol] = code;
        return;
    }
    TraverseEncode(node->left, table, code + LEFT_CHAR);
    TraverseEncode(node->right, table, code + RIGHT_CHAR);
}

map<char, string> Huffman::BuildEncodingTable(shared_ptr<freq_info> root) {
    map<char, string> table;
    TraverseEncode(root, table, "");
    return table;
}

string Huffman::Encode(map<char, string> enc_table, string input) {
    string cipher = "";
    for (int i = 0; i < int(input.size()); i++) {
        cipher = cipher + enc_table[input[i]];
    }
    return cipher;
}

string Huffman::Decode(shared_ptr<freq_info> root, string input) {
    string message = "";
    string cur_code = "";
    shared_ptr<freq_info> curNode;
    for (int i = 0; i < int(input.size()); i++) {
        cur_code = cur_code + input[i];
        curNode = root;
        for (int j = 0; j < int(cur_code.size()); j++) {
            if (cur_code[j] == '.') {
                curNode = curNode->left;
            }
            else {
                curNode = curNode->right;
            }
        }
        if (curNode->is_leaf == true) {
            message = message + curNode->symbol;
            cur_code = "";
        }
    }
    return message;
}