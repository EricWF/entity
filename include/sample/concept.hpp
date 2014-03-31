#ifndef SAMPLE_CONCEPT_HPP
#define SAMPLE_CONCEPT_HPP

# include "entity.hpp"
# include "sample/attribute.hpp"
# include "sample/method.hpp"
# include <elib/aux.hpp>
# include <functional>
# include <vector>

/**
 * Concepts are used to build predicates that can be checked against single
 * entities and lists of entities. The concept interface is provided via
 * CRTP. To define a concept you need to inherit from concept_base AND
 * provide a method of the following signature:
 *     bool test(entity const &) const;
 */
namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                            META CONCEPTS
////////////////////////////////////////////////////////////////////////////////
/**
 * It is usefull to define "meta-concepts" that can be used to build larger
 * Concepts
 */

    /// A very basic concept that tests if an entity is alive.
    struct Alive : concept_base<Alive>
    {
        bool test(entity const & e) const
        {
            return e.alive();
        }
    };

    /// EntityIs checks to see if an entity's ID matches one of the provided
    /// ID's
    template <entity_id ...IDList>
    struct EntityIs : concept_base<EntityIs<IDList...>>
    {
        bool test(entity const & e) const
        {
            // variadic logical OR
            return concept_or(
                (e.id() == IDList)...
              );
        }
    };

    /// EntityHas checks to see if an entity has ALL of the methods/attributes
    /// specified
    template <class ...MethodOrAttribute>
    struct EntityHas : concept_base<EntityHas<MethodOrAttribute...>>
    {
        bool test(entity const & e) const
        {
            // concept_check takes either a concept, an attribute, or an method
            // and checks it against the entity
            return concept_and(
                concept_check<MethodOrAttribute>(e)...
            );
        }
    };
    
    /// EntityHasNone checks that an entity has none of the methods/attributes
    /// specified.
    template <class ...MethodOrAttribute>
    struct EntityHasNone : concept_base<EntityHasNone<MethodOrAttribute...>>
    {
        bool test(entity const & e) const
        {
            return not concept_or(
                concept_check<MethodOrAttribute>(e)...
            );
        }
    };
    
    /// Define a "meta-concept" that stores a position and checks if
    /// tested entities are at that position.
    struct AtPosition : concept_base<AtPosition>
    {
        AtPosition(position p)
          : m_pos(p)
        {}
        
        bool test(entity const & e) const
        {
            return e.get<position>() == m_pos;
        }
        
    private:
        position m_pos;
    };
    
    /// Predicate takes a std::function object with the same signature as test.
    /// It allows arbitrary predicates to be used as concepts.
    struct Predicate : concept_base<Predicate>
    {
        Predicate(std::function<bool(entity const &)> p)
          : m_pred(p)
        {}
        
        bool test(entity const & e) const
        {
            return m_pred(e);
        }

    private:
        std::function<bool(entity const &)> m_pred;
    };
    
////////////////////////////////////////////////////////////////////////////////
//                            SPECIFIC CONCEPTS
////////////////////////////////////////////////////////////////////////////////
/**
 * Once a nice set of meta-concepts have been defined, it is simple to define
 * specific concepts using them.
 */

    /// IsHero and IsMonster use EntityIs to define specific concepts
    using IsHero    = EntityIs<entity_id::hero>;
    using IsMonster = EntityIs<entity_id::monster>;
    using IsWall    = EntityIs<entity_id::wall>;

    /// Specific contents to check if an entity has an attribute/method
    using HasHP     = EntityHas<hp_t>;
    using HasPos    = EntityHas<position>;
    using HasDir    = EntityHas<direction>;
    using HasWeapon = EntityHas<weapon>;
    
    using HasPrint  = EntityHas<print_m>;
    using HasMove   = EntityHas<move_m>;
    using HasAttack = EntityHas<attack_m>;
    
    /// It is often important to check if an entity has the attributes and 
    /// methods needed by some logic. 
    
    /// Check if an entity can be moved.
    using Moveable = Concept<HasMove, HasPos>;
    
    /// This is how the CanAttack concept would be defined
    /// NOTE: the type of the method must be used here, not the instance of it.
    using CanAttack = Concept<Alive, HasAttack, HasWeapon>;
    
    /// Check if a given entity can BE attacked
    using Attackable = Concept<Alive, HasHP, IsMonster>;
    
    
////////////////////////////////////////////////////////////////////////////////
//                           CONCEPT CLASS
////////////////////////////////////////////////////////////////////////////////
/**
 * Concept<...> is a generic concept builder. It takes other concepts one of two ways.
 * 1) via class template type. If a Concept is default constructible it should
 *    be specified in the class template list.
 * 2) via constructor. If a concept is not default constructable it can be passed
 *    to Concept via it's constructor.
 * NOTE: concepts passed via template argument are run BEFORE concepts passed
 *       via constructor. If the template arguments fail the other concepts are
 *       not checked.
 */

# if defined(SAMPLE_EXPOSITION)
#   error SAMPLE_EXPOSITION should never be defined
    /// Concepts frequently need to have non-compile time information.
    /// In this example we build a concept that finds an Monster at a given
    /// position.
 
    /// This is how you would create the concept.
    auto MonsterAtPos = Concept<IsMonster, HasPos>( AtPosition(position{0, 0}) );
# endif

////////////////////////////////////////////////////////////////////////////////
//                            USING CONCEPTS
////////////////////////////////////////////////////////////////////////////////
# if defined(SAMPLE_EXPOSITION)
#   error SAMPLE_EXPOSITION should never be defined
    
    /// This shows how to use concepts against single entities
    void check_entity_example()
    {
        entity e(entity_id::hero);
        e << position{0, 0}
          << hp_t(100)
          << weapon("Sword", 5);
          
        // test e against the concept
        // result == true
        bool result = IsHero().check(e); 
        
        // same as above.
        result = IsHero()(e);
        
        // test e against the concept. If it is not satisfied throw an error.
        IsHero().require(e);
        
        // same as above, but this uses concepts that are not default constructible
        auto HeroAtPos = Concept<IsHero, HasPos>( AtPosition(position{0, 0}) );
        HeroAtPos.check(e);
        HeroAtPos(e);
        HeroAtPos.require(e);
    }
    
    /// This shows how to apply a concept to a collection of entities.
    void check_entity_collection_example()
    {
        std::vector<entity> elist = { /* ... lots of entities ... */ };
        
        /// Check is elist contains AT LEAST one entity that satisfys
        /// the concept
        bool result = IsMonster().contains(elist);
        
        /// Try and find the first entity in a list that satisfys the concept.
        /// Return an iterator at the position of the entity 
        /// (or end() if none is found)
        std::vector<entity>::iterator pos = IsHero().find(elist);
        
        /// Try and find the first entity that satisfys the concept.
        /// return a reference to that entity. Throw if not found.
        entity & hero = IsHero().get(elist);
        
        /// Create a "filtered_view" of a list of entities that contains
        /// only the entities that satisfy the concept.
        /// NOTE: filtered_view only provides iterators over the filtered view
        ///       NO COPYS ARE MADE. 
        auto view = IsMonster().filter(elist);
        
        /// Views can be used to iterator over matching entities
        for (auto & monster : view)
        {
            // this never fires
            REQUIRE_CONCEPT(monster, IsMonster);
        }
        
        /// Same as above, but iterators it reverse order
        auto rview = IsMonster().rfilter(elist);
        
        /// "apply" a filter to a given list. DO NOT modify the list.
        /// instead return a vector of reference's to entities that match.
        /// This is useful when you want to sort the matching entities.
        std::vector<std::reference<entity>> 
        matching_entities = IsMonster().apply_filter(elist);
    }
    
# endif
}                                                           // namespace chips
#endif /* SAMPLE_CONCEPT_HPP */