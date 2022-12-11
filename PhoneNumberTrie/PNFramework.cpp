#include "PNFramework.h"

PNFramework::PNFramework(){
}

PNFramework::~PNFramework(){
}

bool PNFramework::InsertNumber(string phoneNumber, string name) {
    // if the phoneNumber is not legit, return false
    if (phoneNumber.size() != 10) {
        return false;
    }
    shared_ptr<node> curNode = root;
    int index = 0;
    int curDigit;
    
    // for each digit in the phone number
    while (index < 10) {
        // store the integer in curDigit
        curDigit = stoi(phoneNumber.substr(index, 1));
        // if the branch for curDigit does not exist, create a node and add it
        if (curNode->next_digit[curDigit] == nullptr) {
            AddToNode(curNode, curDigit, false); 
        }
        // move on to the next digit in the number
        curNode = curNode->next_digit[curDigit];
        index++;
    }
    // set the last node as a leaf and update the name
    curNode->is_leaf = true;
    curNode->name = name;
    return true;
}

string PNFramework::SearchNumber(string phoneNumber) {
    // if the phone number is not valid, exit the function
    if (phoneNumber.size() != 10) {
        return "Error: Invalid number, please try again.";
    }
    shared_ptr<node> curNode = root;
    int index = 0;
    int curDigit;
    // for each digit in the phone number
    while (index < 10) {
        // store the integer in curDigit
        curDigit = stoi(phoneNumber.substr(index, 1));
        // if the branch for the current digit exists, follow it
        if (curNode->next_digit[curDigit] != nullptr) {
            curNode = curNode->next_digit[curDigit];
            index++;
        }
        // if the branch doesn't exist, return an error
        else {return "Error: This number is not present in the system.";}
    }
    // return the name of the final node
    string name = curNode->name;
    return name;
}

int PNFramework::FindAllNumbers(string fragment) {
    int size = fragment.size();
    // If the fragment size is invalid, return 0
    if (size > 10 or size == 0) {return 0;}
    shared_ptr<node> curNode = root;
    int curDigit;
    int count;
    
    // run through every digit in the fragment, assign to curDigit
    for (int i = 0; i < size; i++) {
        curDigit = stoi(fragment.substr(i, 1));
        // if the curNode has a pointer for curDigit, follow it
        if (curNode->next_digit[curDigit] != nullptr) {
            curNode = curNode->next_digit[curDigit];
            // if we're on the last digit of the fragment, call GetSize
            if (i == size - 1) {
                count = GetSize(curNode);
            }
        }
        // if curNode has a nullptr for the curDigit, return 0
        else {
            return 0;
        }
    }
    return count;
}

void PNFramework::DeleteShortBranch(string fragment) {
    int size = fragment.size();
    shared_ptr<node> parent = root;
    int last_dig;
    
    // for each digit in the fragment, call FindAllNumbers
    for (int i = 1; i < size; i++) {
        string curFragment = fragment.substr(0, i);
        last_dig = stoi(curFragment.substr(i-1, 1));
        // if there are no real phone numbers in this branch
        if (FindAllNumbers(curFragment) == 0) {
            parent->next_digit[last_dig] = nullptr;
            return;
        }
        parent = parent->next_digit[last_dig];
    }
}

bool PNFramework::DeleteNumber(string fragment) {
    int size = fragment.size();
    // if fragment is not valid, returns false
    if (size > 10 or size == 0) {return false;}
    shared_ptr<node> curNode = root;
    int curDigit;
    
    // if our fragment is only one digit
    if (size == 1) {
        curNode->next_digit[stoi(fragment.substr(0, 1))] = nullptr;
        return true;
    }
    
    // for each digit in the fragment, store in curDigit
    for (int i = 0; i < size; i++) {
        curDigit = stoi(fragment.substr(i, 1));
        // if there is a branch to the next digit, follow it
        if (curNode->next_digit[curDigit] != nullptr) {
            curNode = curNode->next_digit[curDigit];
            // if we are right before the last digit, delete last digit's branch
            if (i == size - 2) {
                curDigit = stoi(fragment.substr(i+1, 1));
                curNode->next_digit[curDigit] = nullptr;
                // clean up any left over nodes
                DeleteShortBranch(fragment);
                return true;
            }
        }
        // if there is not a branch to the next digit, return false
        else {
            return false;
        }
    }
    return false;
}

int PNFramework::GetSize(shared_ptr<node> subtree) {
    // if current node is a leaf return 1
    int count = 0;
    shared_ptr<node> curNode = subtree;
    if (curNode->is_leaf == true) {return 1;}
    
    // recursively call GetSize on all active pointers of curNode
    for (int i = 0; i < 10; i++) {
        if (curNode->next_digit[i] != nullptr) {
            count = count + GetSize(curNode->next_digit[i]);
        }
    }
    return count;
}

shared_ptr<node> PNFramework::InitializeNode() {
    // create a new node in heap memory and set all of its pointers to null
    shared_ptr<node> ret(new node);
    for (int i = 0; i < 10; i++) {ret->next_digit[i] = nullptr;}
    return ret;
}

void PNFramework::AddToNode(shared_ptr<node> curNode, int digit, bool is_leaf) {
    // create a new node and add it to curNode's list of pointers
    shared_ptr<node> new_node = InitializeNode();
    new_node->is_leaf = false;
    new_node->name = "";
    curNode->next_digit[digit] = new_node;
}

shared_ptr<node> PNFramework::GetRoot() {
    return root;
}

void PNFramework::SetRoot(shared_ptr<node> given_ptr) {
    root = given_ptr;
}