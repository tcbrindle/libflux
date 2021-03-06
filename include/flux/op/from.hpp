
// Copyright (c) 2022 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FLUX_OP_FROM_HPP_INCLUDED
#define FLUX_OP_FROM_HPP_INCLUDED

#include <flux/core.hpp>
#include <flux/op/ref.hpp>

namespace flux {

namespace detail {

struct from_fn {
    template <adaptable_sequence Seq>
    [[nodiscard]]
    constexpr auto operator()(Seq&& seq) const -> lens auto
    {
        if constexpr (lens<std::remove_cvref_t<Seq>>) {
            if constexpr (std::is_lvalue_reference_v<Seq>) {
                return flux::ref(seq);
            } else {
                return FLUX_FWD(seq);
            }
        } else if constexpr (std::is_lvalue_reference_v<Seq>) {
            return detail::ref_adaptor<std::remove_reference_t<Seq>>(seq);
        } else {
            return detail::owning_adaptor<Seq>(FLUX_FWD(seq));
        }
    }
};

// This should go elsewhere...
template <typename Seq>
using lens_t = decltype(from_fn{}(FLUX_DECLVAL(Seq)));

} // namespace detail

inline constexpr auto from = detail::from_fn{};

} // namespace flux

#endif // FLUX_OP_FROM_HPP_INCLUDED
