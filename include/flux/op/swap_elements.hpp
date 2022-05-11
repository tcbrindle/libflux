
#ifndef FLUX_OP_SWAP_ELEMENTS_HPP_INCLUDED
#define FLUX_OP_SWAP_ELEMENTS_HPP_INCLUDED

#include <flux/core.hpp>

namespace flux {

namespace detail {

struct swap_elements_fn {
    template <sequence Seq1, sequence Seq2>
        requires element_swappable_with<Seq1&, Seq2&>
    constexpr void operator()(Seq1&& seq1, Seq2&& seq2) const
    {
        auto idx1 = flux::first(seq1);
        auto idx2 = flux::first(seq2);

        while (!flux::is_last(seq1, idx1) && !flux::is_last(seq2, idx2)) {
            flux::swap_with(seq1, idx1, seq2, idx2);
            flux::inc(seq1, idx1);
            flux::inc(seq2, idx2);
        }
    }
};

}

inline constexpr auto swap_elements = detail::swap_elements_fn{};

}

#endif
