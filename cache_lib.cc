// defining the different include functions
#include "cache.hh"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>
//   defining the different variables used to write the code
using namespace std;
using storage_unit = pair<Cache::val_type, Cache::size_type>;
// defining the cache class
class Cache::Impl
{
	
public:
	size_type maximummemory;
	size_type usedmemory;
	Evictor* evictor_;
	hash_func used_hasher; 
	unordered_map<key_type, storage_unit, hash_func> map;
	// using the IF function
	bool del(key_type key) {
if (map.find(key) != map.end())
{
usedmemory -= map.at(key).second;
delete[] map.at(key).first;   
map.erase(key);
return true;
}
return false; 
// return false statement
}
// void statement for setting variables
void set(key_type key, val_type val, size_type size) {
byte_type *copyavalue = new byte_type[size];
int j =0;
while (val[j] != '\0')
{
copyavalue[j] = val[j];
j++;
}
if (map.find(key) != map.end())
{
del(key);
}
if (usedmemory + size <= maximummemory)
{
storage_unit onepair = make_pair(copyavalue, size);
map[key] = onepair;
usedmemory += size;
map.reserve(map.size());
if (evictor_ != nullptr)
{
evictor_->touch_key(key);
}
}
else
{
if (evictor_ == nullptr)
{
delete[] copyavalue;
}
else{
while (usedmemory + size > maximummemory)
{
auto evictoristhere = del(evictor_->evict());
while (!evictoristhere)
{
evictoristhere = del(evictor_->evict());
}
}
storage_unit onepair = make_pair(copyavalue, size);
map[key] = onepair;
usedmemory += size;
map.reserve(map.size());
}
}
}
val_type get(key_type key, size_type& val_size) const{
if (map.find(key) == map.end()){
return nullptr;
}
if (evictor_ != nullptr)
{
evictor_->touch_key(key);
}
val_size = map.at(key).second;
return map.at(key).first;
}
size_type space_used() const{
return usedmemory;
}
void reset(){
auto i = map.begin();
while (i != map.end())
{
del(i->first);
i = map.begin();
}
		
usedmemory = 0;
map.clear();
maximummemory = 0;
}
};
Cache::Cache(size_type maxmem, float max_load_factor, Evictor* evictor , hash_func hasher) : pImpl_(new Impl()) {
if (hasher == nullptr)
{
unordered_map<key_type, storage_unit, hash_func> altmap(4);
pImpl_->map = altmap;
}
else{
pImpl_->used_hasher =  hasher;
unordered_map<key_type, storage_unit, hash_func> altmap(4, hasher);
pImpl_->map = altmap;
}
pImpl_->map.max_load_factor(max_load_factor);
pImpl_->usedmemory =0;
pImpl_->maximummemory = maxmem;
pImpl_->evictor_ = evictor;
}
Cache::~Cache() = default;
void Cache::set(key_type key, val_type val, size_type size) {
pImpl_->set(key, val, size);
}
Cache::val_type Cache::get(key_type key, size_type& val_size) const {
return pImpl_->get(key, val_size);
}
bool Cache::del(key_type key) {
return pImpl_->del(key);
}
Cache::size_type Cache::space_used() const {
return pImpl_->space_used();
}
void Cache::reset(){
pImpl_->reset();
}
