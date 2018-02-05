/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_SIZE_HPP
#define RANGES_V3_SIZE_HPP

#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \cond
        namespace size_detail
        {
            // HACKHACK: Unqualified name lookup for "size" must find a function
            // - **any** function - so that the object definition one namespace
            // up doesn't impede ADL under "one-phase" name lookup.
            void size();

            class fn : public iter_size_fn
            {
                template<typename Rng, typename I = decltype(begin(std::declval<Rng&>())),
                    CONCEPT_REQUIRES_(ForwardIterator<I>::value)>
                static constexpr auto impl(Rng && rng, priority_tag<0>)
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    end(rng) - begin(rng)
                )
                template<typename Rng>
                static constexpr auto impl(Rng && rng, priority_tag<1>)
                    noexcept(noexcept(size(rng))) ->
                    if_decayed<Integral, decltype(size(rng))>
                {
                    return size(rng);
                }
                template<typename Rng>
                static constexpr auto impl(Rng && rng, priority_tag<2>)
                    noexcept(noexcept(rng.size())) ->
                    if_decayed<Integral, decltype(rng.size())>
                {
                    return rng.size();
                }
            public:
                using iter_size_fn::operator();

                template<typename Rng>
                constexpr auto operator()(Rng && rng) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    fn::impl(detail::forward<Rng>(rng), priority_tag<2>{})
                )
                template<typename T, std::size_t N>
                constexpr std::size_t operator()(T (&)[N]) const noexcept
                {
                    return N;
                }
                template<typename T, typename F = fn>
                constexpr auto operator()(std::reference_wrapper<T> rw) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    F{}(rw.get())
                )
                template<typename T, bool RValue, typename F = fn>
                constexpr auto operator()(ranges::reference_wrapper<T, RValue> t) const
                RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT
                (
                    F{}(t.get())
                )
            };
        }
        /// \endcond

        /// \ingroup group-core
        /// \return The result of an unqualified call to `size`
        namespace
        {
            constexpr auto&& size = static_const<size_detail::fn>::value;
        }
    }
}

#endif
