#ifndef SAMPLE_ATTRIBUTE_HPP
#define SAMPLE_ATTRIBUTE_HPP

# include "entity/fwd.hpp"
# include "entity/attribute.hpp"
# include <elib/aux.hpp>

/**
 * Custom attribute types can be created one of three ways.
 * 1. A type may inherit from attribute_base
 * 2. A type may overload extension::is_attribute_impl
 * 3. A type may be defined using any_attribute.
 * 
 * Once a type has been marked as an attribute, it can be used with an entity.
 */
namespace chips
{
# if defined(SAMPLE_EXPOSITION)
#   error SAMPLE_EXPOSITION should never be defined
////////////////////////////////////////////////////////////////////////////////
//                              EXAMPLE ONE
////////////////////////////////////////////////////////////////////////////////
/**
 * This is how an custom type may be marked as an attribute by inheriting from
 * attribute base.
 */

    struct weapon : attribute_base
    {
        constexpr weapon(const char *xname, int xdamage) noexcept
          : name(xname), damage(xdamage)
        {}
        
        const char *const name;
        int damage;
    };
    
    static_assert(
        is_attribute<weapon>::value
      , "A weapon is now an attribute"
    );
    
////////////////////////////////////////////////////////////////////////////////
//                            EXAMPLE TWO
////////////////////////////////////////////////////////////////////////////////
/**
 * This example shows how to overload is_attribute_impl for types that cannot
 * inherit from attribute_base.
 */

    enum class actor_state
    {
        none, 
        walking,
        attacking, 
        frozen
    };
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<actor_state> : elib::true_ {};
    }                                                       // namespace extension

    static_assert(
        is_attribute<actor_state>::value
      , "actor state is now an attribute"
    );
    
////////////////////////////////////////////////////////////////////////////////
//                          EXAMPLE THREE
////////////////////////////////////////////////////////////////////////////////
/**
 * This example shows how to use any_attribute to create attribute types
 * that "act" like the type they store. Since an entity can only store one
 * of each type, it is usefull to create unique types that act like the value
 * they store.
 */

    /// used to make any_attribute a unique type
    struct hp_tag {};
    
    /// hp_t acts like an int.
    using hp_t = any_attribute<int, hp_tag>;


    void any_attribute_usage_example()
    {
        /// Constructors (non-move/copy)
        {
            // default construct
            hp_t hp1;
            
            // construct from value
            hp_t hp2(100);
        }
        /// Set
        {
            hp_t hp;
            hp = 100;
            hp.set(100);
        }
        /// Get
        {
            hp_t hp;
            // uses conversion operator
            int & hp_ref1 = hp;
            // explicitly use conversion operator
            int & hp_ref2 = (int &)hp;
            // deference operator
            int & hp_ref3 = *hp;
            // get method
            int & hp_ref4 = hp.get();
            // get by value
            int hp_val3 = hp.value();
        }
    }
# endif 

////////////////////////////////////////////////////////////////////////////////
//                          COMMON ATTRIBUTES
////////////////////////////////////////////////////////////////////////////////
/**
 * This is a set of attributes we will be using in examples.
 * some of them were already defined in the above examples (but that is not compiled)
 */


    /** Position */    
    struct position : attribute_base
    {
        constexpr position() noexcept
          : x(0), y(0)
        {}
        
        constexpr position(int _x, int _y) noexcept
          : x(_x), y(_y)
        {}
        
        constexpr position(position const & other) noexcept
          : x(other.x), y(other.y)
        {}
        
        position & operator=(position const &) = default;
        
        int x, y;
    };
    
    constexpr bool operator==(position const & lhs, position const & rhs) noexcept
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    
    constexpr bool operator !=(position const & lhs, position const & rhs) noexcept
    {
        return !(lhs == rhs);
    }
    
    /** Direction */
    enum class direction 
    {
        N, W, S, E
    };
    
    constexpr direction turn_left(direction d) noexcept
    {
        return static_cast<direction>(
            (static_cast<int>(d) + 1) % 4
        );
    }
    
    constexpr direction turn_right(direction d) noexcept
    {
        return static_cast<direction>(
            (static_cast<int>(d) + 3) % 4
        );
    }
    
    constexpr direction turn_around(direction d) noexcept
    {
        return static_cast<direction>(
            (static_cast<int>(d) + 2) % 4
        );
    }
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<direction> : elib::true_ {};
    }                                                       // namespace extension
    
    
    /** HP */
    struct hp_tag {};
    using hp_t = any_attribute<int, hp_tag>;
    
    /** Weapon */
    struct weapon : attribute_base
    {
        constexpr weapon(const char *xname, int xdamage) noexcept
          : name(xname), damage(xdamage)
        {}
        
        constexpr weapon(weapon const & other) noexcept
          : name(other.name), damage(other.damage)
        {}
        
        weapon & operator=(weapon const &) = default;
        
        const char *name;
        int damage;
    };
    
    constexpr weapon dagger("dagger", 1);
    constexpr weapon sword("sword", 5);
    
}                                                           // namespace entity
#endif /* SAMPLE_ATTRIBUTE_HPP */