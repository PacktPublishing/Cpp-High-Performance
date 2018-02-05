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

#ifndef RANGES_V3_VIEW_PARTIAL_SUM_HPP
#define RANGES_V3_VIEW_PARTIAL_SUM_HPP

#include <utility>
#include <iterator>
#include <functional>
#include <type_traits>
#include <meta/meta.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/size.hpp>
#include <range/v3/empty.hpp>
#include <range/v3/front.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/view_adaptor.hpp>
#include <range/v3/utility/optional.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/semiregular.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/view/view.hpp>
#include <range/v3/view/all.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-views
        /// @{
        template<typename Rng, typename Fun>
        struct partial_sum_view
          : view_adaptor<partial_sum_view<Rng, Fun>, Rng>
        {
        private:
            friend range_access;
            semiregular_t<function_type<Fun>> fun_;
            using single_pass = SinglePass<range_iterator_t<Rng>>;
            using use_sentinel_t = meta::or_<meta::not_<BoundedRange<Rng>>, single_pass>;

            template<bool IsConst>
            struct adaptor : adaptor_base
            {
            private:
                using partial_sum_view_t = meta::apply<meta::add_const_if_c<IsConst>, partial_sum_view>;
                optional<range_value_t<Rng>> sum_;
                partial_sum_view_t *rng_;
            public:
#ifdef RANGES_WORKAROUND_MSVC_207089
                using single_pass = SinglePass<range_iterator_t<Rng>>;
#else
                using single_pass = partial_sum_view::single_pass;
#endif
                adaptor() = default;
                adaptor(partial_sum_view_t &rng)
                  : sum_{}, rng_(&rng)
                {}
                adaptor(partial_sum_view_t &rng, range_value_t<Rng> sum)
                  : sum_(std::move(sum)), rng_(&rng)
                {}
                range_value_t<Rng> get(range_iterator_t<Rng> it) const
                {
                    return *sum_;
                }
                void next(range_iterator_t<Rng> &it)
                {
                    using R = range_common_reference_t<Rng>;
                    if(++it != ranges::end(rng_->mutable_base()))
                    {
                        sum_ = rng_->fun_(R(*sum_), R(*it));
                    }
                }
                void prev() = delete;
            };

            adaptor<false> begin_adaptor()
            {
#ifdef RANGES_WORKAROUND_MSVC_PERMISSIVE_DEPENDENT_BASE
                return ranges::empty(this->base()) ? adaptor<false>{*this} :
                    adaptor<false>{*this, ranges::front(this->base())};
#else
                return empty(this->base()) ? adaptor<false>{*this} :
                    adaptor<false>{*this, front(this->base())};
#endif
            }
            meta::if_<use_sentinel_t, adaptor_base, adaptor<false>> end_adaptor()
            {
#ifdef RANGES_WORKAROUND_MSVC_PERMISSIVE_DEPENDENT_BASE
                if(use_sentinel_t() || ranges::empty(this->base()))
                    return {*this};
                return {*this, ranges::front(this->base())};
#else
                if(use_sentinel_t() || empty(this->base()))
                    return {*this};
                return {*this, front(this->base())};
#endif
            }
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
            CONCEPT_REQUIRES(Callable<Fun const, range_common_reference_t<Rng>,
                range_common_reference_t<Rng>>::value)
#else
            CONCEPT_REQUIRES(Callable<Fun const, range_common_reference_t<Rng>,
                range_common_reference_t<Rng>>())
#endif
            adaptor<true> begin_adaptor() const
            {
#ifdef RANGES_WORKAROUND_MSVC_PERMISSIVE_DEPENDENT_BASE
                return ranges::empty(this->base()) ? adaptor<true>{*this} :
                    adaptor<true>{*this, ranges::front(this->base())};
#else
                return empty(this->base()) ? adaptor<true>{*this} :
                    adaptor<true>{*this, front(this->base())};
#endif
            }
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
            CONCEPT_REQUIRES(Callable<Fun const, range_common_reference_t<Rng>,
                range_common_reference_t<Rng>>::value)
#else
            CONCEPT_REQUIRES(Callable<Fun const, range_common_reference_t<Rng>,
                range_common_reference_t<Rng>>())
#endif
            meta::if_<use_sentinel_t, adaptor_base, adaptor<true>> end_adaptor() const
            {
#ifdef RANGES_WORKAROUND_MSVC_PERMISSIVE_DEPENDENT_BASE
                if(use_sentinel_t() || ranges::empty(this->base()))
                    return {*this};
                return {*this, ranges::front(this->base())};
#else
                if(use_sentinel_t() || empty(this->base()))
                    return {*this};
                return {*this, front(this->base())};
#endif
            }
        public:
            partial_sum_view() = default;
            partial_sum_view(Rng rng, Fun fun)
              : partial_sum_view::view_adaptor{std::move(rng)}
              , fun_(as_function(std::move(fun)))
            {}
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
            CONCEPT_REQUIRES(SizedRange<Rng>::value)
#else
            CONCEPT_REQUIRES(SizedRange<Rng>())
#endif
            range_size_t<Rng> size() const
            {
                return ranges::size(this->base());
            }
        };

        namespace view
        {
            struct partial_sum_fn
            {
            private:
                friend view_access;
                template<typename Fun = plus>
                static auto bind(partial_sum_fn partial_sum, Fun fun = {})
                RANGES_DECLTYPE_AUTO_RETURN
                (
                    make_pipeable(std::bind(partial_sum, std::placeholders::_1,
                        protect(std::move(fun))))
                )
            public:
                template<typename Rng, typename Fun>
                using Concept = meta::and_<
                    InputRange<Rng>,
                    IndirectCallable<Fun, range_iterator_t<Rng>, range_iterator_t<Rng>>,
                    ConvertibleTo<
                        concepts::Callable::result_t<Fun, range_common_reference_t<Rng>,
                            range_common_reference_t<Rng>>,
                        range_value_t<Rng>>>;

                template<typename Rng, typename Fun,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                    CONCEPT_REQUIRES_(Concept<Rng, Fun>::value)>
#else
                    CONCEPT_REQUIRES_(Concept<Rng, Fun>())>
#endif
                partial_sum_view<all_t<Rng>, Fun> operator()(Rng && rng, Fun fun) const
                {
                    return {all(std::forward<Rng>(rng)), std::move(fun)};
                }
            #ifndef RANGES_DOXYGEN_INVOKED
                template<typename Rng, typename Fun,
#ifdef RANGES_WORKAROUND_MSVC_SFINAE_CONSTEXPR
                    CONCEPT_REQUIRES_(!Concept<Rng, Fun>::value)>
#else
                    CONCEPT_REQUIRES_(!Concept<Rng, Fun>())>
#endif
                void operator()(Rng && rng, Fun fun) const
                {
                    CONCEPT_ASSERT_MSG(InputRange<Rng>(),
                        "The first argument passed to view::partial_sum must be a model of the "
                        "InputRange concept.");
                    CONCEPT_ASSERT_MSG(IndirectCallable<Fun, range_iterator_t<Rng>,
                        range_iterator_t<Rng>>(),
                        "The second argument passed to view::partial_sum must be callable with "
                        "two values from the range passed as the first argument.");
                    CONCEPT_ASSERT_MSG(ConvertibleTo<
                        concepts::Callable::result_t<Fun, range_common_reference_t<Rng>,
                        range_common_reference_t<Rng>>, range_value_t<Rng>>(),
                        "The return type of the function passed to view::partial_sum must be "
                        "convertible to the value type of the range.");
                }
            #endif
            };

            /// \relates partial_sum_fn
            /// \ingroup group-views
            namespace
            {
                constexpr auto&& partial_sum = static_const<view<partial_sum_fn>>::value;
            }
        }
        /// @}
    }
}

#endif
