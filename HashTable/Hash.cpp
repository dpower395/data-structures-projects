#include "Hash.h"

// implemented for you - don't change this one
unsigned int DJB2(std::string key){
  unsigned int hash = 5381;
  // Leaving the debugging stuff commented out, in case you want to
  // play. It will output the hash at each incremental step so you can
  // see how it takes shape.
  //
  //  cout << "Hashing string \"" << key << "\"" << endl;
  //  cout << bitset<32>(hash) << " " << hash << endl;
  for (size_t i=0; i < key.length(); i++) {
    char c = key[i]; 
    hash = ((hash << 5) + hash) + c;
    // cout << bitset<32>(hash) << " " << hash << endl;
  }
  // cout << "Hashed string \"" << key << "\" to " << hash << endl;
  //
  // for light reading on why djb2 might not be the best
  // production-grade hash function, go to
  // http://dmytry.blogspot.com/2009/11/horrible-hashes.html
  return hash;
}

// implemented for you - don't change this one
unsigned int ModuloBucketFunc(unsigned int hashcode, unsigned int cap){
  unsigned int b = hashcode % cap;
  return b;
}

// constructor, initialize class variables and pointers here if need.
Hash::Hash(){
  // your code here
}

//deconstructor,
Hash::~Hash(){
}

shared_ptr<hash_table> Hash::InitTable(unsigned int cap){
    shared_ptr<hash_table> ret(new hash_table);
    ret->capacity = cap;
    ret->size = 0;
    ret->occupied = 0;
    ret->table = shared_ptr<htable>(new htable(cap));
    for (unsigned int i = 0; i < cap; i++) {
        (*ret->table)[i] = nullptr;
    }
    ret->hash_func = DJB2;
    ret->bucket_func = ModuloBucketFunc;
    return ret;
}

shared_ptr<hash_node> Hash::InitNode(std::string key, unsigned int hashcode, std::string val){
    shared_ptr<hash_node> ret(new hash_node);
    ret->deleted = false;
    ret->key = key;
    ret->hashcode = hashcode;
    ret->value = val;
    return ret;
}

bool Hash::SetKVP(shared_ptr<hash_table> tbl, std::string key, std::string value){
    unsigned int hashcode = tbl->hash_func(key);
    unsigned int bucket_idx = tbl->bucket_func(hashcode, tbl->capacity);
    unsigned int cap = tbl->capacity;
    unsigned int index;
    
    for (unsigned int j = 0; j < cap; j++) {
        index = (bucket_idx + j) % cap;
        
        if ((*tbl->table)[index] != nullptr and (*tbl->table)[index]->hashcode == hashcode) {
            if ((*tbl->table)[index]->deleted == true) {
                tbl->size++;
            }
            shared_ptr<hash_node> new_node = InitNode(key, hashcode, value);
            (*tbl->table)[index] = new_node;
            return true;
        }
    }
    
    for (unsigned int i = 0; i < cap; i++) {
        index = (bucket_idx + i) % cap;
        
        if ((*tbl->table)[index] == nullptr or (*tbl->table)[index]->deleted == true) { 
            if ((*tbl->table)[index] == nullptr) {
                tbl->size++;
                tbl->occupied++;
            }
            else {
                tbl->size++;
            }
            shared_ptr<hash_node> new_node = InitNode(key, hashcode, value);
            (*tbl->table)[index] = new_node;
            return true;
        }
    }
    return false;
}

float Hash::Load(shared_ptr<hash_table> tbl){
    float load = float(tbl->size) / tbl->capacity;
    return load;
}

std::string Hash::GetVal(shared_ptr<hash_table> tbl, std::string key){
    std::string val = "";
    unsigned int cap = tbl->capacity;
    unsigned int hashcode = tbl->hash_func(key);
    unsigned int bucket_idx = tbl->bucket_func(hashcode, tbl->capacity);
    unsigned int index;
    
    for (unsigned int i = 0; i < cap; i++) {
        index = (bucket_idx + i) % cap;
        if ((*tbl->table)[index] != nullptr and (*tbl->table)[index]->hashcode == hashcode) {
            if ((*tbl->table)[index]->key == key and (*tbl->table)[index]->deleted == false) {
                val = (*tbl->table)[index]->value;
            }
        }
    }
    return val;
}

bool Hash::Contains(shared_ptr<hash_table> tbl, std::string key){
    unsigned int cap = tbl->capacity;
    unsigned int hashcode = tbl->hash_func(key);
    unsigned int bucket_idx = tbl->bucket_func(hashcode, tbl->capacity);
    unsigned int index;
    
    for (unsigned int i = 0; i < cap; i++) {
        index = (bucket_idx + i) % cap;
        if ((*tbl->table)[index] != nullptr and (*tbl->table)[index]->hashcode == hashcode) {
            if ((*tbl->table)[index]->deleted == false) {
                return true;
            }
        }
    }
    return false;
}

bool Hash::Remove(shared_ptr<hash_table> tbl, std::string key){
    unsigned int cap = tbl->capacity;
    unsigned int hashcode = tbl->hash_func(key);
    unsigned int bucket_idx = tbl->bucket_func(hashcode, tbl->capacity);
    unsigned int index;
    
    for (unsigned int i = 0; i < cap; i++) {
        index = (bucket_idx + i) % cap;
        if ((*tbl->table)[index] != nullptr and (*tbl->table)[index]->hashcode == hashcode) {
            if ((*tbl->table)[index]->deleted == false) {
                (*tbl->table)[index]->deleted = true;
                tbl->size--;
                return true;
            }
        }
    }
    return false;
}

void Hash::Resize(shared_ptr<hash_table> tbl, unsigned int new_capacity){
    // first need to back up all the existing nodes
    // for every non nullptr and non-deleted node, record the key and value pair
    unsigned int cap = tbl->capacity;
    std::string keys[cap];
    std::string vals[cap];
    for (unsigned int i = 0; i < cap; i++) {
        shared_ptr<hash_node> curNode = (*tbl->table)[i];
        if (curNode != nullptr and curNode->deleted == false) {
            keys[i] = curNode->key;
            vals[i] = curNode->value;
            Remove(tbl, curNode->key);
        }
        else {
            keys[i] = "";
            vals[i] = "";
        }
    }
    // now, create a new htable vector thing with the given capacity
    tbl->capacity = new_capacity;
    tbl->size = 0;
    tbl->table = shared_ptr<htable>(new htable(new_capacity));
    // go through and fill it out with each recorded key and value pair
    for (unsigned int i = 0; i < cap; i++) {
        if (keys[i] != "") {
            SetKVP(tbl, keys[i], vals[i]);
        }
    }
}

// implemented for you - feel free to change this one if you like
void Hash::PrintTable(shared_ptr<hash_table> tbl){
  cout << "Hashtable:" << endl;
  cout << "  capacity: " << tbl->capacity << endl;
  cout << "  size:     " << tbl->size << endl;
  cout << "  occupied: " << tbl->occupied << endl;
  cout << "  load:     " << Load(tbl) << endl;
  if (tbl->capacity < 130) {
    for (unsigned int i=0; i < tbl->capacity; i++) {
      cout << "[" << i << "]    ";
      if (!tbl->table->at(i)) {
        cout << "<empty>" << endl;
      } else if (tbl->table->at(i)->deleted) {
        cout << "<deleted>" << endl;
      } else {
        cout << "\"" << tbl->table->at(i)->key << "\" = \"" << tbl->table->at(i)->value << "\"" << endl;
      }
    }
  } else {
    cout << "    <hashtable too big to print out>" << endl;
  }
}

