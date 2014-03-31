#ifndef SAMPLE_CREATION_HPP
#define SAMPLE_CREATION_HPP

# include "entity.hpp"
# include "sample/attribute.hpp"
# include "sample/method.hpp"
# include "sample/concept.hpp"
# include <elib/aux.hpp>
# include <elib/fmt.hpp>
# include <iostream>

/**
 * Entities contain no attributes or methods upon construction.
 * In order to construct entities with their required logic a dispatch function
 * is used. the "class definition" of an entity with a specific ID 
 * (ex. entity_id::hero) is the function used to create it. 
 * 
 * The create function should contain:
 *   - Lambdas that define the methods an entity has.
 *   - The default values for the attributes.
 * 
 */
namespace chips
{
    entity create_hero(entity_id);
    entity create_monster(entity_id);
    entity create_wall(entity_id);
    entity create_default(entity_id);
    
    /// Dispatch the creation to coorisponding method for each ID.
    inline entity create_entity(entity_id id)
    {
        switch (id)
        {
            case entity_id::hero:
                return create_hero(id);
            case entity_id::monster:
                return create_monster(id);
            default:
                return create_default(id);
        }
    }
    
    /// COMMON MOVE
    /// Some methods are common to multiple types. This is a common move method
    /// NOTE: methods just have to be function pointers. They can be provided
    ///       by both lambdas AND regular functions
    inline void common_move(entity & self, direction d)
    {
        REQUIRE_CONCEPT(self, Moveable);
        position p = self.get<position>();
        switch (d)
        {
            case direction::N: p.y -= 1; break;
            case direction::S: p.y += 1; break;
            case direction::E: p.x += 1; break;
            case direction::W: p.x -= 1; break;
        }
        self << p;
    }
    
    /// COMMON PRINT
    inline void common_print(entity const & self)
    {
        std::cout << "Entity: id = " << to_string(self.id());
        
        if (self.has<position>()) {
            position p = self.get<position>();
            std::cout << "\n    position = (" <<  p.x << ", " << p.y << ")";
        }
        if (self.has<hp_t>()) {
            std::cout << "\n    hp = " << *(self.get<hp_t>());
        }
        if (self.has<weapon>()) {
            weapon w = self.get<weapon>();
            std::cout << "\n    weapon: name = " << w.name 
                      << " damage = " << w.damage;
        }
        
        std::cout << std::endl;
    }
    
    /// CREATE HERO
    /// This function defines the "hero class". 
    /// It defines the methods that the hero provides and gives
    /// the hero the required attributes and methods
    inline entity create_hero(entity_id id)
    {
        /// The definition of attack for a hero-type
        auto hero_attack_def =
        [](entity & self, entity & other)
        {
            REQUIRE_CONCEPT(self, CanAttack);
            REQUIRE_CONCEPT(other, Attackable);
            
            weapon my_wep = self.get<weapon>();
            hp_t & other_hp = other.get<hp_t>();
            
            if (my_wep.damage >= *other_hp) {
                std::cout << elib::fmt(
                    "%s killed %s with %s"
                  , to_string(self.id()), to_string(other.id())
                  , my_wep.name
                );
                
                other.kill();
            }
            else {
                std::cout << elib::fmt(
                    "%s attacked %s with %s"
                  , to_string(self.id()), to_string(other.id())
                  , my_wep.name
                );
                
                other << hp_t(other_hp - my_wep.damage);
            }
            
            std::cout << std::endl;
        };
        
        /// Create the actual entity with default attributes and defined methods
        entity e(id);
        e << hp_t(100) 
          << position(0, 0)
          << weapon("sword", 10)
          << method(print_, common_print)
          << method(move_, common_move)
          << method(attack_, hero_attack_def);
          
        return e;
    }
    
    
    /// CREATE MONSTER
    inline entity create_monster(entity_id id)
    {
        entity e(id);
        e << hp_t(15)
          << position(0, 0)
          << method(print_, common_print)
          << method(move_,  common_move);
        return e;
    }
    
    
    
    /// CREATE DEFAULT
    inline entity create_default(entity_id id)
    {
        entity e(id);
        e << method(print_, common_print);
        return e;
    }
    
}                                                           // namespace chips
#endif /* SAMPLE_CREATION_HPP */