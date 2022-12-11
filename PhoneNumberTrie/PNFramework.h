#ifndef PNFRAMEWORK_H__
#define PNFRAMEWORK_H__

#include <string>
#include <memory>
#include <iostream>

using namespace std;

struct node {
    
    // is this node a leaf?
    bool is_leaf;
    
    // pointers to next digits
    shared_ptr<node> next_digit[10];
    
    // name of number holder, empty unless tenth digit
    string name;
};

class PNFramework {
public:
    
    // constructor
    PNFramework();
    
    // deconstructor
    ~PNFramework();
    
    // inserts given name and number into the trie, updates if already exists
    // returns true or false depending on success
    bool InsertNumber(string phoneNumber, string name);
    
    // returns name of number holder if it exists, otherwise returns error message
    string SearchNumber(string phoneNumber);
    
    // this function returns a count of all numbers beginning with this fragment
    int FindAllNumbers(string fragment);
    
    // removes all numbers starting with given number fragment
    // returns true if successful, false if not
    bool DeleteNumber(string fragment);
    
    // recursively counts and returns the number of numbers in given subtree
    int GetSize(shared_ptr<node> subtree);
    
    // dynamically allocates memory for a new node
    shared_ptr<node> InitializeNode();
    
    // returns the root pointer
    shared_ptr<node> GetRoot();
    
    // sets the root pointer
    void SetRoot(shared_ptr<node> given_ptr);
    
private:
    
    // pointer to root of trie
    shared_ptr<node> root;
    
    // creates a new node and adds it to curNodes next_digit
    void AddToNode(shared_ptr<node> curNode, int digit, bool is_leaf);
    
    // delete helper function; checks if fragment should be deleted early
    void DeleteShortBranch(string fragment);
};

#endif // PNFRAMEWORK_H__