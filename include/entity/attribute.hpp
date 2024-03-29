#ifndef ENTITY_ATTRIBUTE_HPP
#define ENTITY_ATTRIBUTE_HPP

# include "entity/fwd.hpp"
# include <elib/aux.hpp>
# include <utility> /* for std::swap */

namespace chips
{       
//========================== ATTRIBUTE EXAMPLE ===============================//
# if defined(CHIPS_EXAMPLE)
#   error "CHIPS_EXAMPLE should never be defined"

    /// Inheriting from attribute_base makes a class an attribute
    struct SampleTypeOne : attribute_base
    {
        // SampleTypeOne definition
    };
    
    /// In this case enum types cant inherit from attribute base.
    /// Therefore we overload is_attribute_impl to make it an attribute
    enum class SampleTypeTwo {};
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<SampleTypeTwo> : elib::true_ {};
    }
# endif 

////////////////////////////////////////////////////////////////////////////////
//                               ANY_ATTRIBUTE
////////////////////////////////////////////////////////////////////////////////
  
    /// Used to easily make attributes of a given type
    /// The dummy is used to make different attributes even if they store the
    /// same type.
    /// Usage: typedef any_attribute<std::string, SomeType> msg_str;
    ///        You can now use the msg_str type as an attribute that wraps
    ///        A string
    template <class T, class Dummy = void> 
    class any_attribute : attribute_base
    {
    public:
        using type = any_attribute;
        
        /// The stored type
        using value_type = T;
            
        static_assert(
            !elib::aux::is_reference<T>::value
            , "T may not be a reference type"
        );
    public:
        constexpr any_attribute()
          : m_value{}
        {}
        
        constexpr any_attribute(any_attribute const & other)
          : m_value{other.m_value}
        {}
        
        any_attribute(any_attribute &&) = default;
        any_attribute & operator=(any_attribute const &) = default;
        any_attribute & operator=(any_attribute &&) = default;
            
        /// Construct from T
        any_attribute(T const & v) : m_value(v) {}
        any_attribute(T && v) : m_value(v) {}
            
        /// Assign from T
        any_attribute & operator=(T const & v) 
        {
            m_value = v;
            return *this;
        }
            
        any_attribute & operator=(T && v) 
        {
            m_value = elib::move(v);
            return *this;
        }
        
        /// Convert to T
        constexpr operator T const &() const noexcept { return m_value; }
        operator T &() noexcept { return m_value; }
        
        /// Deref to T
        constexpr T const & operator*() const noexcept { return m_value; }
        T & operator*() noexcept { return m_value; }
        
        /// This magical operator
        T const * operator->() const noexcept { return elib::addressof(m_value); }
        T * operator->() noexcept { return elib::addressof(m_value); }
        
            
        /// Set T
        void set(T const & v) { m_value = v; }
        void set(T && v)      { m_value = elib::move(v); }
            
        /// Get T by reference
        constexpr T const & get() const noexcept { return m_value; }
        T &       get() noexcept       { return m_value; }
        
        /// Get T by value.
        constexpr T value() const { return m_value; }
        
        void swap(any_attribute & other) noexcept
        {
            using std::swap;
            swap(m_value, other.m_value);
        }
            
    private:
        T m_value;
    };
    
    
    template <class T, class Dummy>
    void swap(any_attribute<T, Dummy> & lhs, any_attribute<T, Dummy> & rhs) noexcept
    {
        lhs.swap(rhs);
    }
    

///  Define comparison operators for any_attribute. 
/// The following comparisons are provided:
///    - any_attribute<T> == any_attribute<T>
///    - any_attribute<T> == T
///    - T == any_attribute<T>
///  NOTE: any_attribute's with the same value type but different
///  dummy params ARE NOT comparible.
# define ANY_ATTRIBUTE_CMP(Op)                                                      \
    template <class T, class Dummy>                                                 \
    constexpr auto operator Op (any_attribute<T, Dummy> const & lhs                 \
                  , any_attribute<T, Dummy> const & rhs)                            \
    ELIB_AUTO_RETURN_NOEXCEPT( lhs.get() Op rhs.get() )                             \
                                                                                    \
    template <class T, class Dummy>                                                 \
    constexpr auto operator Op (any_attribute<T, Dummy> const & lhs, T const & rhs) \
    ELIB_AUTO_RETURN_NOEXCEPT( lhs.get() Op rhs )                                   \
                                                                                    \
    template <class T, class Dummy>                                                 \
    constexpr auto operator Op (T const & lhs, any_attribute<T, Dummy> const & rhs) \
    ELIB_AUTO_RETURN_NOEXCEPT( lhs Op rhs.get() )                                   \
                                                                                    \
    template <class T, class Dummy1, class Dummy2>                                  \
    bool operator Op (any_attribute<T, Dummy1> const &                              \
                    , any_attribute<T, Dummy2> const &) = delete;
    
    ANY_ATTRIBUTE_CMP( == )
    ANY_ATTRIBUTE_CMP( != )
    ANY_ATTRIBUTE_CMP( <  )
    ANY_ATTRIBUTE_CMP( <= )
    ANY_ATTRIBUTE_CMP( >  )
    ANY_ATTRIBUTE_CMP( >= )
    
# undef ANY_ATTRIBUTE_OP

}                                                           // namespace chips
#endif /* ENTITY_ATTRIBUTE_HPP */