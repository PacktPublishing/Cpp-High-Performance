/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_ISTREAM_RANGE_HPP
#define RANGES_V3_ISTREAM_RANGE_HPP

#include <istream>
#include <range/v3/range_fwd.hpp>
#include <range/v3/view_facade.hpp>
#include <range/v3/utility/semiregular.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-core
        /// @{
        template<typename Val>
        struct istream_range
          : view_facade<istream_range<Val>, unknown>
        {
        private:
            friend range_access;
            std::istream *sin_;
            semiregular_t<Val> obj_;
            struct cursor
            {
            private:
                istream_range *rng_;
            public:
                cursor() = default;
                explicit cursor(istream_range &rng)
                  : rng_(&rng)
                {}
                void next()
                {
                    rng_->next();
                }
                Val const &get() const
                {
                    return rng_->cached();
                }
                bool done() const
                {
                    return !*rng_->sin_;
                }
            };
            void next()
            {
                *sin_ >> cached();
            }
            cursor begin_cursor()
            {
                return cursor{*this};
            }

            istream_range(std::istream &sin, Val *)
              : sin_(&sin), obj_{}
            {}
            istream_range(std::istream &sin, semiregular<Val> *)
              : sin_(&sin), obj_{in_place}
            {}
        public:
            istream_range() = default;
            istream_range(std::istream &sin)
              : istream_range(sin, _nullptr_v<semiregular_t<Val>>())
            {
                next(); // prime the pump
            }
            Val & cached()
            {
                return obj_;
            }
        };

    #if !RANGES_CXX_VARIABLE_TEMPLATES
        template<typename Val>
        istream_range<Val> istream(std::istream & sin)
        {
            CONCEPT_ASSERT_MSG(DefaultConstructible<Val>(),
               "Only DefaultConstructible types are extractable from streams.");
            return istream_range<Val>{sin};
        }
    #else
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
        template<typename Val, CONCEPT_REQUIRES_(DefaultConstructible<Val>::value)>
#else
        template<typename Val, CONCEPT_REQUIRES_(DefaultConstructible<Val>())>
#endif
        struct istream_fn
        {
            istream_range<Val> operator()(std::istream & sin) const
            {
                return istream_range<Val>{sin};
            }
        };

        namespace
        {
            template<typename Val>
#if defined(_MSC_VER) && !defined(__clang__) && _MSC_VER < 1910
            // Unspecified VS14 bug that is fixed in VS15.
            constexpr const istream_fn<Val> &istream = static_const<istream_fn<Val>>::value;
#else
            constexpr auto && istream = static_const<istream_fn<Val>>::value;
#endif
        }
    #endif
        /// @}
    }
}

#endif
