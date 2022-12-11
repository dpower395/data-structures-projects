#include "LinkedList.h"
// constructor, initialize class variables and pointers here if need.
LinkedList::LinkedList(){
    top_ptr_ = shared_ptr<node>(NULL);
  }

//deconstructor,
LinkedList::~LinkedList(){
}


shared_ptr<node> LinkedList::InitNode(int data){
    shared_ptr<node> ret(new node);
    ret->data = data;
    ret->next = shared_ptr<node>(NULL);
    return ret;
}

string LinkedList::Report(){
    string ret = "";
    string value;
    shared_ptr<node> curNode = top_ptr_;
    if (top_ptr_ != nullptr) {
        while (curNode != nullptr) {
            value = to_string(curNode->data);
            ret = ret + value + " ";
            curNode = curNode->next;
        }
    }
    return ret;
}

void LinkedList::AppendData(int data){
    shared_ptr<node> curNode = top_ptr_;
    if (top_ptr_ != nullptr) {
        while (curNode->next != nullptr) {
            curNode = curNode->next;
        }
        curNode->next = shared_ptr<node>(new node);
        curNode->next->next = shared_ptr<node>(NULL);
        curNode->next->data = data;
    }
    else {
        top_ptr_ = shared_ptr<node>(new node);
        top_ptr_->data = data;
        top_ptr_->next = shared_ptr<node>(NULL);
    }
}

void LinkedList::Append(shared_ptr<node> new_node){
    shared_ptr<node> curNode = top_ptr_;
    if (top_ptr_ != nullptr) {
        while (curNode->next != nullptr) {
            curNode = curNode->next;
        }
        curNode->next = new_node;
        new_node->next = shared_ptr<node>(NULL);
    }
    else {
        top_ptr_ = new_node;
        new_node->next = shared_ptr<node>(NULL);
    }
}

void LinkedList::InsertData(int offset, int data){
    int index = 0;
    shared_ptr<node> curNode = top_ptr_;
    if (offset == 0) {
        if (top_ptr_ == nullptr) {
            top_ptr_ = shared_ptr<node>(new node);
            top_ptr_->next = shared_ptr<node>(NULL);
            top_ptr_->data = data;
        }
        else {
            shared_ptr<node> tmp(new node);
            tmp->data = data;
            tmp->next = top_ptr_;
            top_ptr_ = tmp;
        }
    }
    else {
        while (curNode != nullptr) {
            if (index == offset - 1) {
                shared_ptr<node> tmp(new node);
                tmp->next = curNode->next;
                tmp->data = data;
                curNode->next = tmp;
                break;
            }
            curNode = curNode->next;
            index++;
        }
    }
}

void LinkedList::Insert(int offset, shared_ptr<node> new_node){
    int index = 0;
    shared_ptr<node> curNode = top_ptr_;
    if (offset == 0) {
        if (top_ptr_ == nullptr) {
            top_ptr_ = new_node;
            top_ptr_->next = shared_ptr<node>(NULL);
        }
        else {
            new_node->next = top_ptr_;
            top_ptr_ = new_node;
        }
    }
    else {
        while (curNode != nullptr) {
            if (index == offset - 1) {
                new_node->next = curNode->next;
                curNode->next = new_node;
                break;
            }
            curNode = curNode->next;
            index++;
        }
    }
}

void LinkedList::Remove(int offset){
    int index = 0;
    shared_ptr<node> curNode = top_ptr_;
    if (offset == 0 and top_ptr_ != nullptr) {
        top_ptr_ = top_ptr_->next;
    }
    else {
        while (curNode->next != nullptr) {
            if (index == offset - 1) {
                curNode->next = curNode->next->next;
                break;
            }
            curNode = curNode->next;
            index++;
        }
    }
}

int LinkedList::Size(){
    int cnt = 0;
    shared_ptr<node> curNode = top_ptr_;
    while (curNode != nullptr) {
        cnt++;
        curNode = curNode->next;
    }
    return cnt;
}

bool LinkedList::Contains(int data){
    bool ret = false;
    shared_ptr<node> curNode = top_ptr_;
    while (curNode != nullptr) {
        if (curNode->data == data) {
            ret = true;
            break;
        }
        curNode = curNode->next;
    }
  return ret;
}

// This function is implemented for you
// It returns the top pointer
shared_ptr<node> LinkedList::GetTop(){
  return top_ptr_;
}

// This function is implemented for you
// It sets a given pointer as the top pointer
void LinkedList::SetTop(shared_ptr<node> top_ptr){
  top_ptr_ = top_ptr;
}
