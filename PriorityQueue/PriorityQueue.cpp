#include "PriorityQueue.h"

PriorityQueue::PriorityQueue(){
}

PriorityQueue::~PriorityQueue(){
}


shared_ptr<pq> PriorityQueue::InitPriorityQueue(){
    shared_ptr<pq> ret(new pq);
    ret->name = "";
    ret->priority = 0;
    ret->next = nullptr;
    return ret;
}

void PriorityQueue::Insert(shared_ptr<pq> queue, string text, float priority){
    shared_ptr<pq> curNode = queue;
    
    // if the queue is empty
    if (curNode->name == "") {
        curNode->name = text;
        curNode->priority = priority;
        curNode->next = nullptr;
        return;
    }
    // create new pq item
    shared_ptr<pq> new_item(new pq);
    
    // if new item is highest priority
    if (curNode->priority < priority) {
        new_item->name = curNode->name;
        new_item->priority = curNode->priority;
        new_item->next = curNode->next;
        curNode->name = text;
        curNode->priority = priority;
        curNode->next = new_item;
        return;
    }
    // otherwise find where to insert our new item
    while(curNode->next != nullptr and curNode->next->priority >= priority) {
        curNode = curNode->next;
    }
    new_item->name = text;
    new_item->priority = priority;
    new_item->next = curNode->next;
    curNode->next = new_item;
    return;
}


string PriorityQueue::Remove(shared_ptr<pq> queue){
    
    // if the queue is empty
    if (queue->name == "") {
        return "";
    }
    // otherwise, return first item's name, change queue to second item
    string text = queue->name;
    if (queue->next == nullptr) {
        queue->name = "";
        queue->priority = 0;
    }
    else {
        queue->name = queue->next->name;
        queue->priority = queue->next->priority;
        queue->next = queue->next->next;
    }
    return text;
}

string PriorityQueue::Peek(shared_ptr<pq> queue){
    string text = queue->name;
    return text;
}