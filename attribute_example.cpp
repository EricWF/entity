#include "entity.hpp"
#include "sample.hpp"
#include <iostream>

using namespace chips;



int main()
{
    // create our entity
    entity e(entity_id::hero);
    
    assert( not e.has<hp_t>() );
}