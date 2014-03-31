#include "entity.hpp"
#include "sample.hpp"
#include <iostream>
#include <string>

using namespace chips;

int main()
{
    std::cout << "Creating a hero. This is the heros state:" << std::endl;
    entity hero = create_entity(entity_id::hero);
    hero(print_);
    std::cout << std::endl;
    
    std::cout << "Moving our hero to the South. This is the hero's state:" << std::endl;
    hero(move_, direction::S);
    hero(print_);
    std::cout << std::endl;
    
    std::cout << "Removing our hero's sword. This is the hero's state:\n";
    hero.remove<weapon>();
    hero(print_);
    std::cout << std::endl;
    
    std::cout << "Giving our hero a dagger. This is the hero's state:\n";
    hero << weapon("dagger", 5);
    hero(print_);
    std::cout << std::endl;
    
    std::cout << "Creating a monster. This is the monsters state:" << std::endl;
    entity monster = create_entity(entity_id::monster);
    monster(print_);
    std::cout << std::endl;
    
    std::cout << "Our Hero encounters the monster!\n";
    while (monster) {
        hero(attack_, monster);
    }
    
    
    
}