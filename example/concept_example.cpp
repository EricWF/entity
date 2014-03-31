#include "entity.hpp"
#include "sample.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace chips;

int main()
{
    std::vector<entity> elist =
    {
        // 1 hero
        create_entity(entity_id::hero)
        // 5 monsters
      , create_entity(entity_id::monster)
      , create_entity(entity_id::monster)
      , create_entity(entity_id::monster)
      , create_entity(entity_id::monster)
      , create_entity(entity_id::monster)
      // 10 walls
      , create_entity(entity_id::wall)
      , create_entity(entity_id::wall)
      , create_entity(entity_id::wall)
      , create_entity(entity_id::wall)
      , create_entity(entity_id::wall)
      , create_entity(entity_id::wall)
      , create_entity(entity_id::wall)
      , create_entity(entity_id::wall)
      , create_entity(entity_id::wall)
      , create_entity(entity_id::wall)
    };
    
    // shuffle the vector (so we don't know where everything is)
    std::random_shuffle(elist.begin(), elist.end());
    
    
    // count each entity;
    int hero_count = 0;
    int monster_count = 0;
    int wall_count = 0;
    for (auto & h : IsHero().filter(elist)) {
        ((void)h);
        ++hero_count;
    }
    for (auto & m : IsMonster().filter(elist)) {
        ((void)m);
        ++monster_count;
    }
    for (auto & w : IsWall().filter(elist)) {
        ((void)w);
        ++wall_count;
    }
    
    std::cout << "There are " << hero_count << " Heros, " 
              << monster_count << " Monsters, and "
              << wall_count << " walls.\n\n";
    
    
    
    // Use a concept to find out hero
    entity & hero = IsHero().get(elist);
    // give a one-hit weapon
    hero << weapon("Doomhammer", 1000);
    // print the state
    std::cout << "Found our Hero. This is the hero's state\n";
    hero(print_);
    std::cout << std::endl;
    
    
    std::cout << "Our hero now attacks all the monsters\n";
    int mon_count = 0;
    for (auto & monster : Attackable().filter(elist))
    {
        ++mon_count;
        hero(attack_, monster);
    }
    
    std::cout << "\n";
    std::cout << "Our hero killed " << mon_count << " Monsters!\n";
    
    if (not Attackable().contains(elist)) {
        std::cout << "There are no more monsters left." << std::endl;
    }
}