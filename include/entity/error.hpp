#ifndef CHIPS_ERROR_HPP
#define CHIPS_ERROR_HPP

# include <elib/aux.hpp>
# include <elib/except.hpp>

namespace chips
{
    /// The error used in chips. It should be thrown with
    /// ELIB_THROW_EXCEPTION to get throwsite information
    class entity_error : public elib::exception
    {
    public:
        using elib::exception::exception;
        ELIB_DEFAULT_COPY_MOVE(entity_error);
    };
}                                                           // namespace chips
#endif /* CHIPS_ERROR_HPP */