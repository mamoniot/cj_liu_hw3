// CJ Liu
// defining the functions used 
#include <queue>
#include "evictor.hh"
#include <list>
#include <deque>
#include <vector>
// defining the variables to be used
using namespace std;
// defining class name
class Fifo_Evictor:public Evictor
{
private:
    queue<key_type, list<key_type>> values;
public:
    const key_type evict();
    void touch_key(const key_type&);
    Fifo_Evictor();//running the fifo evictor function
    ~Fifo_Evictor();
};
