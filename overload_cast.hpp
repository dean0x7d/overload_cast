// Copyright (c) 2016, Dean Moldovan
// License: simplified BSD, see the license.md file

#pragma once

template<bool> struct const_t {};
static constexpr auto const_ = const_t<true>{};

template<class... Args>
struct overload_ {
    template<class Return>
    constexpr auto operator()(Return (* pf)(Args...)) const noexcept
                              -> decltype(pf) { return pf; }

    template<class Return, class Class>
    constexpr auto operator()(Return (Class::* pmf)(Args...), const_t<false> = {}) const noexcept
                              -> decltype(pmf) { return pmf; }

    template<class Return, class Class>
    constexpr auto operator()(Return (Class::* pmf)(Args...) const, const_t<true>) const noexcept
                              -> decltype(pmf) { return pmf; }
};

template<class... Args>
constexpr auto overload_cast = overload_<Args...>{};
