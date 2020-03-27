#include "fifo_evictor.hh"
//  including the function fifo evictor
Fifo_Evictor::Fifo_Evictor(){

}
// setting default value at first
Fifo_Evictor::~Fifo_Evictor() = default; 
// running constant variable command
const key_type Fifo_Evictor::evict(){
    if (values.empty() == false)
    {
        key_type lastkey = values.front();
        values.pop();
        return lastkey;//returning values
    }
    return "";
//    return values.front();
}


void Fifo_Evictor::touch_key(const key_type& key){
    values.push(key);
}

