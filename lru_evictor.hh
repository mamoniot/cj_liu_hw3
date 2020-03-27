// CJ Liu
// defining the different include functions used
#include <deque>
#include <vector>
#include <list>
#include <queue>
#include "evictor.hh"

// defining variables
using namespace std;
// defining class lru evictor variable
class LRU_Evictor:public Evictor
{
private: 
    
   deque<key_type> values; // deque values
public:
    const key_type evict();
    void touch_key(const key_type&);
    LRU_Evictor(); //running the lru evictor function
    ~LRU_Evictor();
};
