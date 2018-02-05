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

#ifndef RANGES_V3_UTILITY_MOVE_HPP
#define RANGES_V3_UTILITY_MOVE_HPP

#include <utility>
#include <type_traits>
#include <meta/meta.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/utility/associated_types.hpp>

namespace ranges
{
    inline namespace v3
    {
        namespace aux
        {
            /// \ingroup group-utility
            struct move_fn : move_tag
            {
                template<typename T,
                    typename U = meta::_t<std::remove_reference<T>>>
                U && operator()(T && t) const noexcept
                {
                    return static_cast<U &&>(t);
                }
            };

            /// \ingroup group-utility
            /// \sa `move_fn`
            namespace
            {
                constexpr auto&& move = static_const<move_fn>::value;
            }

            /// \ingroup group-utility
            /// \sa `move_fn`
            template<typename T>
            meta::_t<std::remove_reference<T>> && operator|(T && t, move_fn move) noexcept
            {
                return move(t);
            }

            /// \ingroup group-utility
            /// \sa `move_fn`
            template<typename R>
            using move_t =
                meta::if_<
                    std::is_reference<R>,
                    meta::_t<std::remove_reference<R>> &&,
                    detail::decay_t<R>>;
        }

        /// \cond
        namespace adl_move_detail
        {
            void indirect_move();

            // Default indirect_move overload.
            template<typename I, typename R = reference_t<I const>>
            auto indirect_move(I const &i)
            RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
            (
                static_cast<aux::move_t<R>>(aux::move(*i))
            )

            struct indirect_move_fn
            {
                template<typename I>
                auto operator()(I const &i) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    indirect_move(i)
                )
            };
        }
        /// \endcond

        namespace
        {
            constexpr auto&& indirect_move =
                static_const<adl_move_detail::indirect_move_fn>::value;
        }

#ifdef RANGES_WORKAROUND_MSVC_SFINAE_ALIAS_DECLTYPE
        template<typename>
        using rvalue_reference_t_void_t = void;
        template<typename, typename = void> struct rvalue_reference_t_helper {};
        template<typename I> struct rvalue_reference_t_helper<I, rvalue_reference_t_void_t<decltype(indirect_move(std::declval<I>()))>> {
            using type = decltype(indirect_move(std::declval<I>()));
        };
        template<typename I>
        using rvalue_reference_t = meta::_t<rvalue_reference_t_helper<I>>;
#else
        template<typename I>
        using rvalue_reference_t = decltype(indirect_move(std::declval<I>()));
#endif

        namespace detail
        {
            // impact algorithm\swap_ranges.cpp
            template<typename I, typename O>
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_FUNCTION_DECLTYPE
            meta::_t<meta::detail::_and_<
                std::is_constructible<
                    meta::_t<value_type<I>>,
                    rvalue_reference_t<I>>,
                std::is_assignable<
                    reference_t<O>,
                    rvalue_reference_t<I>>>>
#else
            meta::and_<
                std::is_constructible<
                    meta::_t<value_type<I>>,
                    rvalue_reference_t<I>>,
                std::is_assignable<
                    decltype(*std::declval<O>()),
                    rvalue_reference_t<I>>>
#endif
            is_indirectly_movable_(int);

            template<typename I, typename O>
            std::false_type
            is_indirectly_movable_(long);

            template<typename I, typename O>
            meta::and_<
                std::is_nothrow_constructible<
                    meta::_t<value_type<I>>,
                    rvalue_reference_t<I>>,
                std::is_nothrow_assignable<
                    decltype(*std::declval<O>()),
                    rvalue_reference_t<I>>>
            is_nothrow_indirectly_movable_(int);

            template<typename I, typename O>
            std::false_type
            is_nothrow_indirectly_movable_(long);
        }

        template<typename I, typename O>
        struct is_indirectly_movable
          : decltype(detail::is_indirectly_movable_<I, O>(42))
        {};

        template<typename I, typename O>
        struct is_nothrow_indirectly_movable
          : decltype(detail::is_nothrow_indirectly_movable_<I, O>(42))
        {};
    }
}

#endif
