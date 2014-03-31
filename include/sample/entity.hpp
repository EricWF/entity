#ifndef SAMPLE_ENTITY_HPP
#define SAMPLE_ENTITY_HPP

# include "entity.hpp"
# include "sample/attribute.hpp"
# include "sample/method.hpp"
# include "sample/concept.hpp"

namespace chips
{
# if defined(SAMPLE_EXPOSITION)
#   error SAMPLE_EXPOSITION should never be defined
    /// CONSTRUCT
    void construct_example()
    {
        /// create a dead, default entity.
        {
            entity e;
        }
        
        /// Create a entity with the ID hero
        {
            entity e(entity_id::hero);
        }
        
        /// Create an entity with the ID monster, and position and hp_t attributes
        {
            entity e(entity_id::monster, position(0, 0), hp_t(100));
        }
    }
    
    /// LIVENESS
    /// this shows how to use the interface that handles the liveness of an
    /// entity as well as its on_death function.
    void liveness_example()
    {
        /// Query liveness
        {
            entity e(entity_id::hero);
            
            /// bool alive() const;
            bool is_alive = e.alive();
            
            /// explicit operator bool() const
            if (e) { /* do stuff... */ }
        }
        
        /// Handle on_death logic
        {
            entity e(entity_id::hero);
            
            /// The on_death callback function type
            using death_fn_type = entity::death_function;
            
            /// Get the function pointer for the current on_death function.
            /// (possible null)
            death_fn_type fn = e.on_death();
            
            /// Set the on_death function
            e.on_death( [](entity &) { /* do stuff... */ } );
        }
        
        /// Kill an entity (and execute on_death)
        {
            entity e(entity_id::hero);
            
            assert(e.alive());
            
            e.kill();
            
            assert(!e.alive());
        }
    }
    
    /// ATTRIBUTES
    void attribute_example()
    {
        entity e;
        
        /// Query
        {
            /// bool has<Attribute>() const
            e.has<hp_t>();
        }
        
        /// Set
        {
            /// Insert if not already present
            /// bool insert(Attribute)
            e.insert(hp_t(100));
            
            /// Always insert
            e.set(hp_t(100));
            
            /// Always insert using stream insertion
            e << hp_t(100);
        }
        
        /// Get
        {
            /// Get a pointer to a given attribute or NULL
            /// if no attribute is found.
            hp_t * hp_ptr = e.get_raw<hp_t>();
            
            /// Get a reference to a given attribute.
            /// Throw if not found
            hp_t & hp_ref = e.get<hp_t>();
        }
        
        /// Remove
        {
            /// void remove()
            e.remove<hp_t>();
            
            /// remove all attributes
            e.clear_attributes();
        }
    }
    
    /// METHODS
    void methods_example()
    {
        entity e;
        /// Query
        {
            /// bool has(MethodTag) const
            e.has(move_);
        }
        
        /// Set
        {
            /// Insert a method if not already present.
            e.insert(move_, common_move);
            
            /// Always insert the method
            e.set(move_, common_move);
            
            /// Always insert using stream insertion
            e << method(move_, common_move);
        }
        
        /// Get
        {
            /// Get a pointer to the method or NULL if the method
            /// is not found.
            function_type* fn_ptr = e.get_raw(move_);
            
            /// Get a pointer to the method
            /// Throw if not found
            fn_ptr = e.get(move_);
        }
        
        /// Invoke
        {
            /// Invoke the method using the function call operator
            /// result_type operator()(Args...)
            e(move_, direction::N);
            
            /// Same as above
            e.call(move_, direction::N);
        }
        
        /// Conditional Invokation
        {
            /// Invoke the method if found, otherwise do nothing.
            /// Return true if the method was called.
            /// bool call_if(MethodTag, Args...)
            e.call_if(move_, direction::N);
            
            /// Invoke the method if found, otherwise do nothing.
            /// Store the results in Result and return true if the
            /// method was called
            /// bool call_if(Result &, MethodTag, Args...)
            bool ret;
            e.call_if(ret, collides_, entity());
        }
        
        /// Remove
        {
            /// Remove a given method if present.
            e.remove(move_);
            
            /// Remove all methods
            e.clear_methods();
        }
    }
    
    /// MISC FUNCTIONALITY
    void misc_example()
    {
        entity e;
        entity other;
        
        /// clear all attributes and methods. set on_death to null.
        /// DO NOT change liveness or ID
        e.clear();
        
        /// Swap two entities
        e.swap(other);
        swap(e, other);
    }
# endif
}                                                           // namespace chips
#endif /* SAMPLE_ENTITY_HPP */