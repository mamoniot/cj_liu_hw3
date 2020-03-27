#define CATCH_CONFIG_MAIN

#include "evictor.hh"
#include "lru_evictor.hh"
#include "fifo_evictor.hh"
#include "cache.hh"
#include <cassert>
#include "catch.hpp"

/*We have assumed the data payload is always of type C string for all testing purposes, and also account for the last '\0' character
at the end of the line for better debugging understanding.
*/
using namespace std;


int return_number(key_type str){
    size_t length = str.length();
    return length;
}

function<size_t(key_type)> customhasher = return_number;  //takes str turns it into its str size

/*
No interdependencies between tests.
No stateful tests--each test is standalone. Your test suite should use a single (global) Cache object, but reset it at the end or beginning of each test.
One piece of functionality tested per test (function). You can extract common set up code to a fixture object.
"Black box" testing only: no testing of internal functionality, optional parameters, or implementation details.
*/


TEST_CASE("First test, checking that Set works"){
//Gonna have to assume that get works here
auto cache = Cache(40, 0.75, nullptr, customhasher);
Cache::size_type size;
Cache::val_type val1 = "value_1";
Cache::val_type val2 = "value_2alright";
Cache::val_type valr = "reject?";
Cache::val_type val3 = "Floccinaucinihilipilification";  //It's 29 characters!


SECTION("ability to add a key-value pair"){
cache.set("key1", val1, 8);
REQUIRE(*cache.get("key1", size) == *val1);
}
SECTION("ability to overwrite the old value if key exists"){
cache.set("key1", val1, 8);
cache.set("key1", val2, 16);
REQUIRE(*cache.get("key1", size) == *val2);
}
SECTION("Check if no new value is inserted if maxmem is exceeded"){
//Once cache is created evictor cannot be reassigned so Evictor is tested elsewhere as by this implication from cache.hh.
cache.set("key1", val3, 30);
cache.set("key2", valr, 8);
REQUIRE(*cache.get("key1", size) == *val3);
REQUIRE(*cache.get("key2", size) == *valr);
cache.set("key3", val2, 16); //Should not get inserted
REQUIRE(cache.get("key3", size) == nullptr);

}
cache.reset();
//as per instructions need to reset after test so all the test is not dependent.
}

TEST_CASE("Second test, checking that Get works"){
auto cache = Cache(40, 0.75, nullptr, customhasher);
Cache::size_type size;
Cache::val_type val1 = "value_1";
Cache::val_type val2 = "value_2alright";
Cache::val_type valr = "reject?";

SECTION("Should throw null pointer if trying to access nothing"){ //
REQUIRE(cache.get("anykey", size) == nullptr);
}
SECTION("Should get correct size value from cache of size 1"){
cache.set("key1", val1, 8);
REQUIRE(*cache.get("key1", size) == *val1);
}
SECTION("Should get correct size value from cache of multiple objects"){
cache.set("key1", val1, 8);
cache.set("key2", val2, 16);
cache.set("key3", valr, 8);
REQUIRE(*cache.get("key2", size) == *val2);
REQUIRE(*cache.get("key1", size) == *val1);
REQUIRE(*cache.get("key3", size) == *valr);

}
SECTION("Check if size_type is getting changed"){
cache.set("key1", val1, 8);
cache.get("key1", size);
auto sizebefore = size;
cache.set("key2", val2, 16);
cache.get("key2", size);
REQUIRE(sizebefore != size);

}
cache.reset();
}




TEST_CASE("Third test,checking that SpaceUsed works"){
auto cache = Cache(40, 0.75, nullptr, customhasher);
Cache::val_type val1 = "value_1";
Cache::val_type val2 = "value_2alright";
Cache::val_type valr = "reject?";

SECTION("The spaceused size should be 0 of empty cache"){
REQUIRE(cache.space_used() == 0);
}
SECTION("Returns sum of size of one value cache"){
cache.set("key1", val1, 8);
REQUIRE(cache.space_used() == 8);
}
SECTION("Returns sum of size of all values"){
cache.set("key1", val1, 8);
cache.set("key2", val2, 16);
cache.set("keyr", valr, 8);
REQUIRE(cache.space_used() == 32);
}

cache.reset();
}

TEST_CASE("Fourth test,checking that Del works"){
auto cache = Cache(40, 0.75, nullptr, customhasher);
Cache::size_type size;
Cache::val_type val1 = "value_1";
Cache::val_type val2 = "value_2alright";
Cache::val_type valr = "reject?";

SECTION("Should return false if attempting to delete something from empty cache"){
REQUIRE(cache.del("key1") == false);
}
SECTION("Attempt to delete something that is not in cache when has other things in it"){
cache.set("key1", val1, 8);
cache.set("key2", val2, 16);
cache.set("keyr", valr, 8);
REQUIRE(cache.del("key3") == false);
}
SECTION("Successfully delete something that is in the cache"){
cache.set("key1", val1, 8);
cache.set("key2", val2, 16);
cache.set("keyr", valr, 8);
auto boolresult = cache.del("keyr");
REQUIRE(boolresult == true);
REQUIRE(cache.get("keyr", size) == nullptr);
REQUIRE(cache.space_used() != 32);

}
SECTION("Attempt to delete something that has already been deleted"){
//Should return false
cache.set("key1", val1, 8);
cache.set("key2", val2, 16);
cache.set("keyr", valr, 8);
cache.del("keyr");
REQUIRE(cache.del("keyr") == false);
}
cache.reset();
}
TEST_CASE("Fifth Test,checking that reset works"){
auto cache = Cache(40, 0.75, nullptr, customhasher);
Cache::size_type size;
Cache::val_type val1 = "value_1";
Cache::val_type val2 = "value_2alright";
Cache::val_type valr = "reject?";
cache.set("key1", val1, 8);
cache.set("key2", val2, 16);
cache.set("keyr", valr, 8);
REQUIRE(cache.space_used() != 0);
cache.reset();
REQUIRE(cache.space_used() == 0);
REQUIRE(cache.get("key2", size) == nullptr);
REQUIRE(cache.get("key1", size) == nullptr);
REQUIRE(cache.get("keyr", size) == nullptr);

}

//Below are the test cases for evictor.hh functionality
TEST_CASE("Generic FIFO Tests without a cache"){
//Trying to use evictor.hh only things here, not trying to use anything special with FIFO
auto FIFO = Fifo_Evictor();

SECTION("Check that the touching key funtion?"){
FIFO.touch_key("anykey");
auto holdthing = FIFO.evict();
REQUIRE(holdthing != "");
}

SECTION("Does Eviction work when there is nothing in Evictor?"){
//Should return empty key ("")
REQUIRE(FIFO.evict() == "");

}

SECTION("Does Eviction work when there is something in Evictor?"){
FIFO.touch_key("anykey");
auto holdthing = FIFO.evict();
REQUIRE(holdthing != "");
}

}

//Note: Below case specifically for FIFO
//No way to mess around specifically for an evictor.
TEST_CASE("Testing for Touch: FIFO"){
auto FIFO = Fifo_Evictor();
auto cache = Cache(40, 0.75, &FIFO, customhasher);
Cache::size_type size;
Cache::val_type val1 = "value_1";
Cache::val_type val2 = "value_2alright";
Cache::val_type valr = "reject?";
Cache::val_type valf = "Floccinaucinihilipilerionajd"; // 28 characters



SECTION("Testing - Touch happened in Get"){

cache.set("key1", val1, 8);
cache.set("key2", val2, 16);
cache.get("key1", size);
cache.set("keyf", valf, 28);
REQUIRE(cache.get("key1", size) == nullptr);
REQUIRE(*cache.get("key2", size) == *val2);
REQUIRE(*cache.get("keyf", size) == *valf);

}
SECTION("Testing - Touch happened in Set"){
cache.set("keyf", valf, 28);
cache.set("key1", val1, 8);
REQUIRE(*cache.get("keyf", size) == *valf);
REQUIRE(*cache.get("key1", size) == *val1);
cache.set("keyr", valr, 8);
cache.set("key2", val2, 16);
REQUIRE(cache.get("keyf", size) == nullptr);
REQUIRE(*cache.get("key1", size) == *val1);
REQUIRE(*cache.get("keyr", size) == *valr);
}

cache.reset();

}

TEST_CASE("Testing for Evict: FIFO"){
auto FIFO = Fifo_Evictor();
auto cache = Cache(40, 0.75, &FIFO, customhasher);
Cache::size_type size;
Cache::val_type val1 = "value_1";
Cache::val_type val2 = "value_2alright";
Cache::val_type val3 = "value3";
Cache::val_type val4 = "value_4";
Cache::val_type val5 = "value5";
Cache::val_type valr = "reject?";

SECTION("Testing - Simple deletion without outside meddling"){
cache.set("key1", val1, 8);
cache.set("key2", val2, 16);
cache.set("keyr", valr, 8);
cache.set("key3", val3, 7 );
REQUIRE(cache.space_used() == 39);
cache.set("key4", val4, 8);
REQUIRE(cache.space_used() == 39);
REQUIRE(cache.get("key1", size) == nullptr);
REQUIRE(*cache.get("key2", size) == *val2);
REQUIRE(*cache.get("keyr", size) == *valr);
REQUIRE(*cache.get("key3", size) == *val3);

}
SECTION("Testing - Evicting something that has been deleted"){
cache.set("key1", val1, 8);
cache.set("key2", val2, 16);
cache.set("keyr", valr, 8);
cache.set("key3", val3, 7 );
REQUIRE(cache.space_used() == 39);
cache.del("key1"); //Should end up with same result?
cache.set("key4", val4, 8);

REQUIRE(cache.space_used() == 39);
REQUIRE(cache.get("key1", size) == nullptr);
REQUIRE(*cache.get("key2", size) == *val2);
REQUIRE(*cache.get("keyr", size) == *valr);
REQUIRE(*cache.get("key3", size) == *val3);
}
SECTION("Testing for proper eviction order"){

cache.set("key3", val3, 7);
cache.set("key2", val2, 16);
cache.set("key1", val1, 8);
cache.set("keyr", valr, 8);
cache.get("key2", size); //Since this is FIFO, this should not matter.
cache.set("key4", val4, 8);
REQUIRE(cache.get("key3", size) == nullptr);
REQUIRE(*cache.get("key2", size) == *val2);
REQUIRE(*cache.get("keyr", size) == *valr);
REQUIRE(*cache.get("key1", size) == *val1);
cache.set("key5", val4, 8);
REQUIRE(cache.get("key3", size) == nullptr);
REQUIRE(cache.get("key2", size) == nullptr);
REQUIRE(*cache.get("keyr", size) == *valr);
REQUIRE(*cache.get("key1", size) == *val1);

}
SECTION("Testing - Overwriting values should not change eviction order"){
cache.set("key3", val3, 7);
cache.set("key2", val2, 16);
cache.set("key1", val1, 8);
cache.set("keyr", valr, 8);
cache.set("key3", val5, 7);

cache.set("key4", val4, 8);
REQUIRE(cache.get("key3", size) == nullptr);
REQUIRE(*cache.get("key2", size) == *val2);
REQUIRE(*cache.get("keyr", size) == *valr);
REQUIRE(*cache.get("key1", size) == *val1);
cache.set("key5", val4, 8);
REQUIRE(cache.get("key3", size) == nullptr);
REQUIRE(cache.get("key2", size) == nullptr);
REQUIRE(*cache.get("keyr", size) == *valr);
REQUIRE(*cache.get("key1", size) == *val1);
}

cache.reset();

}


