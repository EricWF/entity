#ifndef SAMPLE_METHOD_HPP
#define SAMPLE_METHOD_HPP

# include "entity.hpp"
# include <iostream>

/** 
 * Methods are defined via MethodTags using the CRTP. 
 * A MethodTag inherits from method_base. method_base takes the derived class 
 * as well as the VISIBLE SIGNATURE of the method. REMEBER these are methods, 
 * not functions. The ACTUAL SIGNATURE takes a reference to the entity it 
 * belongs to as the first parameter. 
 * 
 * Instances (not the type) of MethodTags are used as "method_names". 
 * 
 */
namespace chips
{
# if defined(SAMPLE_EXPOSITION)
#   error SAMPLE_EXPOSITION should never be defined

////////////////////////////////////////////////////////////////////////////////
//                           CONST METHOD EXAMPLE
////////////////////////////////////////////////////////////////////////////////

    /// print_name_m defines a const method.
    /// VISIBLE SIGNATURE: void() const
    /// ACTUAL SIGNATURE: void(entity const & self) const
    /// NOTE: we still need to create an instance of print_name_m to use it easily.
    struct print_name_m : method_base<print_name_m, void() const> {};

    /// print_name_ should be passed to entity to call print_name_m
    constexpr print_name_m print_name_{};

    
    /// This function shows how methods are typically defined and then added
    /// to an entity
    void add_default_print_name(entity & e)
    {
        /// the definition must have the exact ACTUAL SIGNATURE.
        auto print_name_def =
        [](entity const & self)
        {
            std::cout << "My name is " << to_string(self.id()) << std::endl;
        };
        
        /// Set the print_name_ method to use print_name_def when called
        e.set(print_name_, print_name_def);
    };
    
    /// This function shows how print_name_ may have multiple different 
    /// definitions
    void add_hero_print_name(entity & e)
    {
        auto hero_print_name_def =
        [](entity const & self)
        {
            std::cout << "My name is King " << to_string(self.id()) << std::endl;
        };
        
        /// method(MethodTag, MethodDef) is used to group the relevent information
        /// together so operator<< can be used.
        e << method(print_name_, print_name_def);
    }


////////////////////////////////////////////////////////////////////////////////
//                            NON-CONST METHOD
////////////////////////////////////////////////////////////////////////////////

    /// Attack another entity
    /// VISIBLE SIGNATURE: void(entity & other)
    /// ACTUAL SIGNATURE: void(entity & self, entity & other)
    constexpr struct attack_m : method_base<attack_m, void(entity &)>
    {} attack_ {};
    
    /// Example definition and addition of attack method
    void add_attack_method(entity & e)
    {
        auto attack_def =
        [](entity &, entity & other)
        {
            other.kill();
        };
        
        e << method(attack_, attack_def);
    }
# endif /* SAMPLE_EXPOSITION */
////////////////////////////////////////////////////////////////////////////////
//                             COMMON METHODS
////////////////////////////////////////////////////////////////////////////////
/**
 * These are methods that are used elsewhere in the sample.
 * They may have already been defined in the example
 */

    constexpr struct print_m : method_base<print_m, void() const>
    {} print_ {};
    
    constexpr struct move_m  : method_base<move_m, void(direction)>
    {} move_ {};
    
    constexpr struct attack_m : method_base<attack_m, void(entity &)>
    {} attack_ {};
}                                                           // namespace chips
#endif /* SAMPLE_METHOD_HPP */