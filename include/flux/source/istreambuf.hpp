
#ifndef FLUX_SOURCE_ISTREAMBUF_HPP_INCLUDED
#define FLUX_SOURCE_ISTREAMBUF_HPP_INCLUDED

#include <flux/core.hpp>

#include <flux/op/from.hpp>

#include <iosfwd>

namespace flux {

namespace detail {

template <typename CharT, typename Traits>
void derives_from_streambuf_test(std::basic_streambuf<CharT, Traits>&);

template <typename T>
concept derives_from_streambuf = requires (T& t) { derives_from_streambuf_test(t); };

struct from_istreambuf_fn {
    template <typename CharT, typename Traits>
    [[nodiscard]]
    auto operator()(std::basic_streambuf<CharT, Traits>* streambuf) const -> lens auto
    {
        assert(streambuf != nullptr);
        return flux::from(*streambuf);
    }

    template <typename CharT, typename Traits>
    [[nodiscard]]
    auto operator()(std::basic_istream<CharT, Traits>& istream) const -> lens auto
    {
        return flux::from(*istream.rdbuf());
    }
};

} // namespace detail

template <detail::derives_from_streambuf Streambuf>
struct sequence_iface<Streambuf>
{
private:
    struct index_type {
        index_type(index_type&&) = default;
        index_type& operator=(index_type&&) = default;
    private:
        friend struct sequence_iface;
        index_type() = default;
    };

    using traits_type = typename Streambuf::traits_type;
    using char_type = typename Streambuf::char_type;

public:
    static auto first(Streambuf&) -> index_type { return {}; }

    static auto is_last(Streambuf& self, index_type const&) -> bool
    {
        return self.sgetc() == traits_type::eof();
    }

    static auto inc(Streambuf& self, index_type& idx) -> index_type&
    {
        self.sbumpc();
        return idx;
    }

    static auto read_at(Streambuf& self, index_type const&) -> char_type
    {
        return traits_type::to_char_type(self.sgetc());
    }
};

inline constexpr auto from_istreambuf = detail::from_istreambuf_fn{};

} // namespace flux

#endif
