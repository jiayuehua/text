#ifndef BOOST_TEXT_NORMALIZE_STRING_HPP
#define BOOST_TEXT_NORMALIZE_STRING_HPP

#include <boost/text/normalize.hpp>
#include <boost/text/string.hpp>
#include <boost/text/detail/icu/normalize.hpp>


namespace boost { namespace text {

    namespace detail {
        namespace {
            constexpr char const * ncstr = nullptr;
        }

        // NFC dispatch
        template<
            typename CPIter,
            typename Sentinel,
            bool UTF8 = is_detected<utf8_range_expr, CPIter, Sentinel>::value,
            bool UTF16 = is_detected<utf16_range_expr, CPIter, Sentinel>::value>
        struct norm_nfc_append_impl
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
                auto f = make_utf_32_to_16_iterator(first, first, last);
                auto l = make_utf_32_to_16_iterator(first, last, last);
                auto out = utf_16_to_8_inserter(s, s.end());
                detail::icu::utf16_iter_appender<decltype(out)> appender(out);
                detail::icu::utf16_normalize_to_nfc_append(f, l, appender);
            }
        };

        template<typename CPIter, typename Sentinel>
        struct norm_nfc_append_impl<CPIter, Sentinel, true, false>
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
                auto const r = make_utf8_range(first, last);
                detail::icu::utf8_string_appender<String> appender(s);
                detail::icu::utf8_normalize_to_nfc_append(
                    r.begin(), r.end(), appender);
            }
        };

        template<typename CPIter, typename Sentinel>
        struct norm_nfc_append_impl<CPIter, Sentinel, false, true>
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
                auto const r = make_utf16_range(first, last);
                auto out = utf_16_to_8_inserter(s, s.end());
                detail::icu::utf16_iter_appender<decltype(out)> appender(out);
                detail::icu::utf16_normalize_to_nfc_append(
                    r.begin(), r.end(), appender);
            }
        };

        // NFKC dispatch
        template<
            typename CPIter,
            typename Sentinel,
            bool UTF8 = is_detected<utf8_range_expr, CPIter, Sentinel>::value,
            bool UTF16 = is_detected<utf16_range_expr, CPIter, Sentinel>::value>
        struct norm_nfkc_append_impl
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
                auto f = make_utf_32_to_16_iterator(first, first, last);
                auto l = make_utf_32_to_16_iterator(first, last, last);
                auto out = utf_16_to_8_inserter(s, s.end());
                detail::icu::utf16_iter_appender<decltype(out)> appender(out);
                detail::icu::utf16_normalize_to_nfkc_append(f, l, appender);
            }
        };

        template<typename CPIter, typename Sentinel>
        struct norm_nfkc_append_impl<CPIter, Sentinel, true, false>
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
                auto const r = make_utf8_range(first, last);
                detail::icu::utf8_string_appender<String> appender(s);
                detail::icu::utf8_normalize_to_nfkc_append(
                    r.begin(), r.end(), appender);
            }
        };

        template<typename CPIter, typename Sentinel>
        struct norm_nfkc_append_impl<CPIter, Sentinel, false, true>
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
                auto const r = make_utf16_range(first, last);
                auto out = utf_16_to_8_inserter(s, s.end());
                detail::icu::utf16_iter_appender<decltype(out)> appender(out);
                detail::icu::utf16_normalize_to_nfkc_append(
                    r.begin(), r.end(), appender);
            }
        };

        // FCC dispatch
        template<
            typename CPIter,
            typename Sentinel,
            bool UTF8 = is_detected<utf8_range_expr, CPIter, Sentinel>::value,
            bool UTF16 = is_detected<utf16_range_expr, CPIter, Sentinel>::value>
        struct norm_fcc_append_impl
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
#if 1 // TODO
                normalize_to_fcc(first, last, utf_32_to_8_inserter(s, s.end()));
#else
                auto f = make_utf_32_to_16_iterator(first, first, last);
                auto l = make_utf_32_to_16_iterator(first, last, last);
                auto out = utf_16_to_8_inserter(s, s.end());
                detail::icu::utf16_iter_appender<decltype(out)> appender(out);
                detail::icu::utf16_normalize_to_fcc_append(f, l, appender);
#endif
            }
        };

        template<typename CPIter, typename Sentinel>
        struct norm_fcc_append_impl<CPIter, Sentinel, true, false>
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
                auto const r = make_utf8_range(first, last);
                detail::icu::utf8_string_appender<String> appender(s);
                detail::icu::utf8_normalize_to_fcc_append(
                    r.begin(), r.end(), appender);
            }
        };

        template<typename CPIter, typename Sentinel>
        struct norm_fcc_append_impl<CPIter, Sentinel, false, true>
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
                auto const r = make_utf16_range(first, last);
                auto out = utf_16_to_8_inserter(s, s.end());
                detail::icu::utf16_iter_appender<decltype(out)> appender(out);
                detail::icu::utf16_normalize_to_fcc_append(
                    r.begin(), r.end(), appender);
            }
        };

        // NFD dispatch
        template<typename CPIter, typename Sentinel, typename String>
        static void
        norm_nfd_append_impl_impl(CPIter first, Sentinel last, String & s)
        {
            auto out = utf_16_to_8_inserter(s, s.end());
            detail::icu::utf16_iter_appender<decltype(out)> appender(out);

            int const chunk_size = 512;
            detail::icu::UnicodeString input(chunk_size * 2);
            auto input_first = input.data();

            while (first != last) {
                int n = 0;
                auto input_last = input_first;
                for (; first != last && n < chunk_size;
                     ++first, ++input_last, ++n) {
                    *input_last = *first;
                }
                auto const input_new_first =
                    detail::icu::utf16_normalize_to_nfd_append(
                        input.data(), input_last, appender);
                input_first =
                    std::copy(input_new_first, input_last, input.data());
            }
        }

        template<
            typename CPIter,
            typename Sentinel,
            bool UTF16 = is_detected<utf16_range_expr, CPIter, Sentinel>::value>
        struct norm_nfd_append_impl
        {
            template<typename String>
            static void call(CPIter first_, Sentinel last_, String & s)
            {
                auto first = make_utf_32_to_16_iterator(first_, first_, last_);
                auto last = make_utf_32_to_16_iterator(first_, last_, last_);
                norm_nfd_append_impl_impl(first, last, s);
             }
        };

        template<typename CPIter, typename Sentinel>
        struct norm_nfd_append_impl<CPIter, Sentinel, true>
        {
            template<typename String>
            static void call(CPIter first, Sentinel last, String & s)
            {
                auto const r = make_utf16_range(first, last);
                norm_nfd_append_impl_impl(r.begin(), r.end(), s);
            }
        };

        // NFKD dispatch
        template<typename CPIter, typename Sentinel, typename String>
        static void
        norm_nfkd_append_impl_impl(CPIter first, Sentinel last, String & s)
        {
            auto out = utf_16_to_8_inserter(s, s.end());
            detail::icu::utf16_iter_appender<decltype(out)> appender(out);

            int const chunk_size = 512;
            detail::icu::UnicodeString input(chunk_size * 2);
            auto input_first = input.data();

            while (first != last) {
                int n = 0;
                auto input_last = input_first;
                for (; first != last && n < chunk_size;
                     ++first, ++input_last, ++n) {
                    *input_last = *first;
                }
                auto const input_new_first =
                    detail::icu::utf16_normalize_to_nfkd_append(
                        input.data(), input_last, appender);
                input_first =
                    std::copy(input_new_first, input_last, input.data());
            }
        }

        template<
            typename CPIter,
            typename Sentinel,
            bool UTF16 = is_detected<utf16_range_expr, CPIter, Sentinel>::value>
        struct norm_nfkd_append_impl
        {
            template<typename String>
            static void call(CPIter first_, Sentinel last_, String & s)
            {
                auto first = make_utf_32_to_16_iterator(first_, first_, last_);
                auto last = make_utf_32_to_16_iterator(first_, last_, last_);
                norm_nfkd_append_impl_impl(first, last, s);
             }
        };

        template<typename CPIter, typename Sentinel>
        struct norm_nfkd_append_impl<CPIter, Sentinel, true>
        {
            template<typename String>
            static void call(CPIter first_, Sentinel last_, String & s)
            {
                auto const r = make_utf16_range(first_, last_);
                norm_nfkd_append_impl_impl(r.begin(), r.end(), s);
            }
        };
    }

    /** Appends sequence `[first, last)` in normalization form NFD to `s`, in
        UTF-8 encoding.

        This function only participates in overload resolution if `CPIter`
        models the CPIter concept.

        \see https://unicode.org/notes/tn5 */
    template<typename CPIter, typename Sentinel, typename String>
    inline auto
    normalize_to_nfd_append_utf8(CPIter first, Sentinel last, String & s)
        -> detail::cp_iter_ret_t<
            decltype(s.insert(s.end(), detail::ncstr, detail::ncstr), s),
            CPIter>
    {
        detail::norm_nfd_append_impl<CPIter, Sentinel>::call(first, last, s);
        return s;
    }

    /** Appends sequence `r` in normalization form NFD to `s`, in UTF-8
        encoding.

        \see https://unicode.org/notes/tn5 */
    template<typename CPRange, typename String>
    inline String & normalize_to_nfd_append_utf8(CPRange const & r, String & s)
    {
        return normalize_to_nfd_append_utf8(std::begin(r), std::end(r), s);
    }

    /** Appends sequence `[first, last)` in normalization form NFKD to `s`, in
        UTF-8 encoding.

        This function only participates in overload resolution if `CPIter`
        models the CPIter concept.

        \see https://unicode.org/notes/tn5 */
    template<typename CPIter, typename Sentinel, typename String>
    inline auto
    normalize_to_nfkd_append_utf8(CPIter first, Sentinel last, String & s)
        -> detail::cp_iter_ret_t<
            decltype(s.insert(s.end(), detail::ncstr, detail::ncstr), s),
            CPIter>
    {
#if 0 // TODO: Badly broken!
        detail::norm_nfkd_append_impl<CPIter, Sentinel>::call(first, last, s);
#else
        detail::normalize_to_decomposed(
            first,
            last,
            utf_32_to_8_inserter(s, s.end()),
            [](uint32_t cp) { return detail::compatible_decompose(cp); });
#endif

        return s;
    }

    /** Appends sequence `r` in normalization form NFKD to `s`, in UTF-8
        encoding.

        \see https://unicode.org/notes/tn5 */
    template<typename CPRange, typename String>
    inline String & normalize_to_nfkd_append_utf8(CPRange const & r, String & s)
    {
        return normalize_to_nfkd_append_utf8(std::begin(r), std::end(r), s);
    }

    /** Appends sequence `[first, last)` in normalization form NFC to `s`, in
        UTF-8 encoding.

        This function only participates in overload resolution if `CPIter`
        models the CPIter concept.

        \see https://unicode.org/notes/tn5 */
    template<typename CPIter, typename Sentinel, typename String>
    inline auto
    normalize_to_nfc_append_utf8(CPIter first, Sentinel last, String & s)
        -> detail::cp_iter_ret_t<
            decltype(s.insert(s.end(), detail::ncstr, detail::ncstr), s),
            CPIter>
    {
        detail::norm_nfc_append_impl<CPIter, Sentinel>::call(first, last, s);
        return s;
    }

    /** Appends sequence `r` in normalization form NFC to `s`, in UTF-8
        encoding.

        \see https://unicode.org/notes/tn5 */
    template<typename CPRange, typename String>
    inline String & normalize_to_nfc_append_utf8(CPRange const & r, String & s)
    {
        return normalize_to_nfc_append_utf8(std::begin(r), std::end(r), s);
    }

    /** Appends sequence `[first, last)` in normalization form NFKC to `s`, in
        UTF-8 encoding.

        This function only participates in overload resolution if `CPIter`
        models the CPIter concept.

        \see https://unicode.org/notes/tn5 */
    template<typename CPIter, typename Sentinel, typename String>
    inline auto
    normalize_to_nfkc_append_utf8(CPIter first, Sentinel last, String & s)
        -> detail::cp_iter_ret_t<
            decltype(s.insert(s.end(), detail::ncstr, detail::ncstr), s),
            CPIter>
    {
#if 0 // TODO: Badly broken!
        detail::norm_nfkc_append_impl<CPIter, Sentinel>::call(first, last, s);
#else
        detail::normalize_to_composed<false>(
            first,
            last,
            utf_32_to_8_inserter(s, s.end()),
            [](uint32_t cp) { return detail::compatible_decompose(cp); },
            [](uint32_t cp) {
                return detail::quick_check_nfkc_code_point(cp);
            });
#endif
        return s;
    }

    /** Appends sequence `r` in normalization form NFKC to `s`, in UTF-8
        encoding.

        \see https://unicode.org/notes/tn5 */
    template<typename CPRange, typename String>
    inline String & normalize_to_nfkc_append_utf8(CPRange const & r, String & s)
    {
        return normalize_to_nfkc_append_utf8(std::begin(r), std::end(r), s);
    }

    /** Appends sequence `[first, last)` in normalization form FCC to `s`, in
        UTF-8 encoding.

        This function only participates in overload resolution if `CPIter`
        models the CPIter concept.

        \see https://unicode.org/notes/tn5 */
    template<typename CPIter, typename Sentinel, typename String>
    inline auto
    normalize_to_fcc_append_utf8(CPIter first, Sentinel last, String & s)
        -> detail::cp_iter_ret_t<
            decltype(s.insert(s.end(), detail::ncstr, detail::ncstr), s),
            CPIter>
    {
        detail::norm_fcc_append_impl<CPIter, Sentinel>::call(first, last, s);
        return s;
    }

    /** Appends sequence `r` in normalization form FCC to `s`, in UTF-8
        encoding.

        \see https://unicode.org/notes/tn5 */
    template<typename CPRange, typename String>
    inline String & normalize_to_fcc_append_utf8(CPRange const & r, String & s)
    {
        return normalize_to_fcc_append_utf8(std::begin(r), std::end(r), s);
    }

    /** Puts the contents of `s` in Unicode normalization form NFD.
        Normalization is not performed if `s` passes a normalization
        quick-check. */
    inline void normalize_to_nfd(string & s)
    {
        utf32_range as_utf32(s);
        if (detail::normalized_quick_check(
                as_utf32.begin(), as_utf32.end(), [](uint32_t cp) {
                    return detail::quick_check_nfd_code_point(cp);
                }) == detail::quick_check::yes) {
            return;
        }

        string temp;
        temp.reserve(s.size() / 2 * 3);

        normalize_to_nfd_append_utf8(as_utf32.begin(), as_utf32.end(), temp);

        if (temp.size() <= s.capacity())
            s = temp;
        else
            s.swap(temp);
    }

    /** Puts the contents of `s` in Unicode normalization form NFKD.
        Normalization is not performed if `s` passes a normalization
        quick-check. */
    inline void normalize_to_nfkd(string & s)
    {
        utf32_range as_utf32(s);
        if (detail::normalized_quick_check(
                as_utf32.begin(), as_utf32.end(), [](uint32_t cp) {
                    return detail::quick_check_nfkd_code_point(cp);
                }) == detail::quick_check::yes) {
            return;
        }

        string temp;
        temp.reserve(s.size() / 2 * 3);

        normalize_to_nfkd_append_utf8(as_utf32.begin(), as_utf32.end(), temp);

        if (temp.size() <= s.capacity())
            s = temp;
        else
            s.swap(temp);
    }

    /** Puts the contents of `s` in Unicode normalization form NFC.
        Normalization is not performed if `s` passes a normalization
        quick-check. */
    inline void normalize_to_nfc(string & s)
    {
        utf32_range as_utf32(s);
        if (detail::normalized_quick_check(
                as_utf32.begin(), as_utf32.end(), [](uint32_t cp) {
                    return detail::quick_check_nfc_code_point(cp);
                }) == detail::quick_check::yes) {
            return;
        }

        string temp;
        temp.reserve(temp.size());

        normalize_to_nfc_append_utf8(as_utf32.begin(), as_utf32.end(), temp);

        if (temp.size() <= s.capacity())
            s = temp;
        else
            s.swap(temp);
    }

    /** Puts the contents of `s` in Unicode normalization form NFKC.
        Normalization is not performed if `s` passes a normalization
        quick-check. */
    inline void normalize_to_nfkc(string & s)
    {
        utf32_range as_utf32(s);
        if (detail::normalized_quick_check(
                as_utf32.begin(), as_utf32.end(), [](uint32_t cp) {
                    return detail::quick_check_nfkc_code_point(cp);
                }) == detail::quick_check::yes) {
            return;
        }

        string temp;
        temp.reserve(s.size());

        normalize_to_nfkc_append_utf8(as_utf32.begin(), as_utf32.end(), temp);

        if (temp.size() <= s.capacity())
            s = temp;
        else
            s.swap(temp);
    }

    /** Puts the contents of `s` in normalization form FCC. */
    inline void normalize_to_fcc(string & s)
    {
        // http://www.unicode.org/notes/tn5/#FCC
        utf32_range as_utf32(s);

        string temp;
        temp.reserve(s.size());

        normalize_to_fcc_append_utf8(as_utf32.begin(), as_utf32.end(), temp);

        if (temp.size() <= s.capacity())
            s = temp;
        else
            s.swap(temp);
    }

}}

#endif
