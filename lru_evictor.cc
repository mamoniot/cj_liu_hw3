// CJ Liu
//defining the include function
#include "lru_evictor.hh"

LRU_Evictor::LRU_Evictor(){

}

LRU_Evictor::~LRU_Evictor() = default;
//setting it to default value
// setting the const value variable
const key_type LRU_Evictor::evict(){ 
    if (values.empty() == false)
    {
        key_type lastkey = values.front();
     // checking if values been marked
        values.pop_front();
        return lastkey;
// return last key
    }
    return "";
//    return values.front();
}


void LRU_Evictor::touch_key(const key_type& key){  
    auto location  = find(values.begin(), values.end(), key);
    if (location != values.end())
    {
        values.erase(location);
    }
    
    values.push_back(key);
}

