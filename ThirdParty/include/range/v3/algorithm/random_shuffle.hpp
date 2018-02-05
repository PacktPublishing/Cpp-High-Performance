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
#ifndef RANGES_V3_ALGORITHM_RANDOM_SHUFFLE_HPP
#define RANGES_V3_ALGORITHM_RANDOM_SHUFFLE_HPP

#include <random>
#include <utility>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/swap.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \cond
        namespace detail
        {
            template<typename = void>
            std::random_device::result_type random_seed_()
            {
#if RANGES_CXX_THREAD_LOCAL < RANGES_CXX_THREAD_LOCAL_11
#error This fork requires C++11 thread_local support.
#endif
                static thread_local std::random_device s_rd;
                return s_rd();
            }
        }
        /// \endcond

        /// \addtogroup group-concepts
        /// @{
        namespace concepts
        {
            struct RandomNumberGenerator
            {
                template<typename Gen, typename D>
                auto requires_(Gen&&, D&& d) -> decltype(
                    concepts::valid_expr(
                        concepts::model_of<Integral, D>(),
                        concepts::convertible_to<D>(val<Gen>()(d))
                    ));
            };
        }

        template<typename Gen, typename D>
        using RandomNumberGenerator = concepts::models<concepts::RandomNumberGenerator, Gen, D>;
        /// @}

        /// \addtogroup group-algorithms
        /// @{
        struct random_shuffle_fn
        {
            template<typename I, typename S,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                CONCEPT_REQUIRES_(RandomAccessIterator<I>::value && IteratorRange<I, S>::value && Permutable<I>::value)>
#else
                CONCEPT_REQUIRES_(RandomAccessIterator<I>() && IteratorRange<I, S>() && Permutable<I>())>
#endif
            I operator()(I begin, S end_) const
            {
                I end = ranges::next(begin, end_), orig = end;
                auto d = end - begin;
                if(d > 1)
                {
                    using param_t = std::uniform_int_distribution<std::ptrdiff_t>::param_type;
                    std::default_random_engine gen(detail::random_seed_());
                    std::uniform_int_distribution<std::ptrdiff_t> uid;
                    for(--end, --d; begin < end; ++begin, --d)
                    {
                        auto i = uid(gen, param_t{0, d});
                        if(i != 0)
                            ranges::iter_swap(begin, begin + i);
                    }
                }
                return orig;
            }

            template<typename I, typename S, typename Gen,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                CONCEPT_REQUIRES_(RandomAccessIterator<I>::value && IteratorRange<I, S>::value &&
                    Permutable<I>::value &&
                    RandomNumberGenerator<Gen, iterator_difference_t<I>>::value)>
#else
                CONCEPT_REQUIRES_(RandomAccessIterator<I>() && IteratorRange<I, S>() &&
                                  Permutable<I>() &&
                                  RandomNumberGenerator<Gen, iterator_difference_t<I>>())>
#endif
            I operator()(I begin, S end_, Gen && rand) const
            {
                I end = ranges::next(begin, end_), orig = end;
                auto d = end - begin;
                if(d > 1)
                {
                    for(--end, --d; begin < end; ++begin, --d)
                    {
                        auto i = rand(d);
                        ranges::iter_swap(begin, begin + i);
                    }
                }
                return orig;
            }

            template<typename Rng, typename I = range_iterator_t<Rng>,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                CONCEPT_REQUIRES_(RandomAccessRange<Rng>::value &&
                    Permutable<I>::value)>
#else
                CONCEPT_REQUIRES_(RandomAccessRange<Rng>() &&
                                  Permutable<I>())>
#endif
            range_safe_iterator_t<Rng> operator()(Rng &&rng) const
            {
                return (*this)(begin(rng), end(rng));
            }

            template<typename Rng, typename Gen, typename I = range_iterator_t<Rng>,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                CONCEPT_REQUIRES_(RandomAccessRange<Rng>::value &&
                    Permutable<I>::value &&
                    RandomNumberGenerator<Gen, iterator_difference_t<I>>::value)>
#else
                CONCEPT_REQUIRES_(RandomAccessRange<Rng>() &&
                                  Permutable<I>() &&
                                  RandomNumberGenerator<Gen, iterator_difference_t<I>>())>
#endif
            range_safe_iterator_t<Rng> operator()(Rng &&rng, Gen && rand) const
            {
                return (*this)(begin(rng), end(rng), std::forward<Gen>(rand));
            }
        };

        /// \sa `random_shuffle_fn`
        /// \ingroup group-algorithms
        namespace
        {
            RANGES_DEPRECATED("The function templates random_shuffle are deprecated in C++14.")
            constexpr auto&& random_shuffle = static_const<with_braced_init_args<random_shuffle_fn>>::value;
        }

        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
