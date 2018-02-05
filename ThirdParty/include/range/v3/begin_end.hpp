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

#ifndef RANGES_V3_BEGIN_END_HPP
#define RANGES_V3_BEGIN_END_HPP

#include <utility>
#include <iterator>
#include <functional>
#include <initializer_list>
#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/utility/dangling.hpp>
#include <range/v3/utility/iterator.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \cond
        namespace begin_detail
        {
            void begin();

            class fn
            {
                template<typename Rng>
                static constexpr auto impl(Rng& rng, priority_tag<0>)
                    noexcept(noexcept(detail::decay_copy(begin(rng)))) ->
                    if_decayed<Iterator, decltype(begin(rng))>
                {
                    return begin(rng);
                }
                template<typename Rng>
                static constexpr auto impl(Rng& rng, priority_tag<1>)
                    noexcept(noexcept(detail::decay_copy(rng.begin()))) ->
                    if_decayed<Iterator, decltype(rng.begin())>
                {
                    return rng.begin();
                }
                template<typename T, std::size_t N>
                static constexpr T* impl(T (&t)[N], priority_tag<1>) noexcept
                {
                    return t + 0;
                }
                template<typename Rng, typename = meta::if_c<!std::is_lvalue_reference<Rng>::value>>
                RANGES_DEPRECATED("begin(Rvalue) is deprecated")
                static constexpr auto impl(Rng&& rng, priority_tag<2>)
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    fn::impl(detail::as_const(rng), priority_tag<1>{})
                )
                // Extension: reference wrapped ranges are ranges
                template<typename T>
                static constexpr auto impl(std::reference_wrapper<T> rw, priority_tag<3>)
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    fn::impl(rw.get(), priority_tag<3>{})
                )
                template<typename T, bool B>
                static constexpr auto impl(ranges::reference_wrapper<T, B> rw, priority_tag<3>)
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    fn::impl(rw.get(), priority_tag<3>{})
                )
            public:
                template<typename Rng>
                constexpr auto operator()(Rng&& rng) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    fn::impl(static_cast<Rng &&>(rng), priority_tag<3>{})
                )
            };
        }
        /// \endcond

        /// \ingroup group-core
        /// \return The result of an unqualified call to the `begin` free function
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& begin = static_const<begin_detail::fn>::value;
        }

        /// \cond
        namespace end_detail
        {
            void end();

            class fn
            {
                // TODO: constrain return type
                template<typename Rng>
                static constexpr auto impl(Rng& rng, priority_tag<0>)
                    noexcept(noexcept(detail::decay_copy(end(rng)))) ->
                    detail::decay_t<decltype(end(rng))>
                {
                    return end(rng);
                }
                template<typename Rng>
                static constexpr auto impl(Rng& rng, priority_tag<1>)
                    noexcept(noexcept(detail::decay_copy(rng.end()))) ->
                    detail::decay_t<decltype(rng.end())>
                {
                    return rng.end();
                }
                template<typename T, std::size_t N>
                static constexpr T* impl(T (&t)[N], priority_tag<1>) noexcept
                {
                    return t + N;
                }
                template<typename Rng, typename = meta::if_c<!std::is_lvalue_reference<Rng>::value>>
                RANGES_DEPRECATED("end(Rvalue) is deprecated")
                static constexpr auto impl(Rng&& rng, priority_tag<2>)
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    fn::impl(detail::as_const(rng), priority_tag<1>{})
                )
                // Extension: reference wrapped ranges are ranges
                template<typename T>
                static constexpr auto impl(std::reference_wrapper<T> rw, priority_tag<3>)
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    fn::impl(rw.get(), priority_tag<3>{})
                )
                template<typename T, bool B>
                static constexpr auto impl(ranges::reference_wrapper<T, B> rw, priority_tag<3>)
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    fn::impl(rw.get(), priority_tag<3>{})
                )
            public:
                template<typename Rng>
                constexpr auto operator()(Rng&& rng) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    fn::impl(static_cast<Rng &&>(rng), priority_tag<3>{})
                )
            };
        }
        /// \endcond

        /// \ingroup group-core
        /// \return The result of an unqualified call to the `end` free function
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& end = static_const<end_detail::fn>::value;
        }

        /// \cond
        namespace adl_begin_end_detail
        {
            template<typename T>
            auto rbegin(std::reference_wrapper<T> ref) -> decltype(rbegin(ref.get()))
            {
                return rbegin(ref.get());
            }

            template<typename T>
            auto rend(std::reference_wrapper<T> ref) -> decltype(rend(ref.get()))
            {
                return rend(ref.get());
            }

            template<typename T, bool RValue>
            auto rbegin(ranges::reference_wrapper<T, RValue> ref) -> decltype(rbegin(ref.get()))
            {
                return rbegin(ref.get());
            }

            template<typename T, bool RValue>
            auto rend(ranges::reference_wrapper<T, RValue> ref) -> decltype(rend(ref.get()))
            {
                return rend(ref.get());
            }

            struct rbegin_fn
            {
            private:
                template<typename Rng>
                static constexpr auto impl(Rng && rng, long)
                    noexcept(noexcept(detail::decay_copy(rbegin(static_cast<Rng &&>(rng))))) ->
                    detail::decay_t<decltype(rbegin(static_cast<Rng &&>(rng)))>
                {
                    return rbegin(static_cast<Rng &&>(rng));
                }
                template<typename Rng,
                    meta::if_c<std::is_lvalue_reference<Rng>::value, int> = 0>
                static constexpr auto impl(Rng && rng, int)
                    noexcept(noexcept(detail::decay_copy(rng.rbegin()))) ->
                    detail::decay_t<decltype(rng.rbegin())>
                {
                    return rng.rbegin();
                }
            public:
                template<typename Rng>
                constexpr auto operator()(Rng && rng) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    rbegin_fn::impl(static_cast<Rng &&>(rng), 0)
                )
                template<typename T, std::size_t N>
                constexpr
                ranges::reverse_iterator<T*> operator()(T (&t)[N]) const noexcept
                {
                    return ranges::reverse_iterator<T*>(t + N);
                }
                template<typename T>
                constexpr
                ranges::reverse_iterator<T const*> operator()(std::initializer_list<T> il) const noexcept
                {
                    return ranges::reverse_iterator<T const*>(il.end());
                }
            };

            struct rend_fn
            {
            private:
                template<typename Rng>
                static constexpr auto impl(Rng && rng, long)
                    noexcept(noexcept(detail::decay_copy(rend(static_cast<Rng &&>(rng))))) ->
                    detail::decay_t<decltype(rend(static_cast<Rng &&>(rng)))>
                {
                    return rend(static_cast<Rng &&>(rng));
                }
                template<typename Rng,
                    meta::if_c<std::is_lvalue_reference<Rng>::value, int> = 0>
                static constexpr auto impl(Rng && rng, int)
                    noexcept(noexcept(detail::decay_copy(rng.rend()))) ->
                    detail::decay_t<decltype(rng.rend())>
                {
                    return rng.rend();
                }
            public:
                template<typename Rng>
                constexpr auto operator()(Rng && rng) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    rend_fn::impl(static_cast<Rng &&>(rng), 0)
                )
                template<typename T, std::size_t N>
                constexpr
                ranges::reverse_iterator<T*> operator()(T (&t)[N]) const noexcept
                {
                    return ranges::reverse_iterator<T*>(t);
                }
                template<typename T>
                constexpr
                ranges::reverse_iterator<T const*> operator()(std::initializer_list<T> il) const noexcept
                {
                    return ranges::reverse_iterator<T const*>(il.begin());
                }
            };
        }
        /// \endcond

        /// \ingroup group-core
        /// \return The result of an unqualified call to the `rbegin` free function
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& rbegin = static_const<rbegin_fn>::value;
        }

        /// \ingroup group-core
        /// \return The result of an unqualified call to the `rend` free function
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& rend = static_const<rend_fn>::value;
        }

        /// \cond
        namespace cbegin_end_detail
        {
            struct cbegin_fn
            {
                template<typename Rng>
                constexpr auto operator()(Rng const & rng) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    begin(rng)
                )
            };

            struct cend_fn
            {
                template<typename Rng>
                constexpr auto operator()(Rng const & rng) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    end(rng)
                )
            };

            struct crbegin_fn
            {
                template<typename Rng>
                constexpr auto operator()(Rng const & rng) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    rbegin(rng)
                )
            };

            struct crend_fn
            {
                template<typename Rng>
                constexpr auto operator()(Rng const & rng) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    rend(rng)
                )
            };
        }
        /// \endcond

        /// \ingroup group-core
        /// \return The result of an unqualified call to the `begin` free function
        /// with a const-qualified argument.
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& cbegin = static_const<cbegin_end_detail::cbegin_fn>::value;
        }

        /// \ingroup group-core
        /// \return The result of an unqualified call to the `end` free function
        /// with a const-qualified argument.
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& cend = static_const<cbegin_end_detail::cend_fn>::value;
        }

        /// \ingroup group-core
        /// \return The result of an unqualified call to the `rbegin` free function
        /// with a const-qualified argument.
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& crbegin = static_const<cbegin_end_detail::crbegin_fn>::value;
        }

        /// \ingroup group-core
        /// \return The result of an unqualified call to the `rend` free function
        /// with a const-qualified argument.
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& crend = static_const<cbegin_end_detail::crend_fn>::value;
        }

        /// \ingroup group-core
        struct safe_begin_fn
        {
            /// \return `begin(rng)` if \p rng is an lvalue; otherwise, it returns `begin(rng)`
            /// wrapped in \c ranges::dangling.
            template<typename Rng, typename I = decltype(begin(std::declval<Rng &>()))>
            constexpr meta::if_<std::is_lvalue_reference<Rng>, I, dangling<I>>
            operator()(Rng && rng) const
            {
                return begin(rng);
            }
        };

        /// \ingroup group-core
        struct safe_end_fn
        {
            /// \return `begin(rng)` if \p rng is an lvalue; otherwise, it returns `begin(rng)`
            /// wrapped in \c ranges::dangling.
            template<typename Rng, typename I = decltype(end(std::declval<Rng &>()))>
            constexpr meta::if_<std::is_lvalue_reference<Rng>, I, dangling<I>>
            operator()(Rng && rng) const
            {
                return end(rng);
            }
        };

        /// \ingroup group-core
        /// \return `begin(rng)` if `rng` is an lvalue; otherwise, it returns `begin(rng)`
        /// wrapped in \c ranges::dangling.
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& safe_begin = static_const<safe_begin_fn>::value;
        }

        /// \ingroup group-core
        /// \return `end(rng)` if `rng` is an lvalue; otherwise, it returns `end(rng)`
        /// wrapped in \c ranges::dangling.
        RANGES_GCC_BROKEN_CUSTPOINT namespace
        {
            constexpr auto&& safe_end = static_const<safe_end_fn>::value;
        }
    }
}

#endif
