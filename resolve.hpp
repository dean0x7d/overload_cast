// Copyright (c) 2016, Dean Moldovan
// License: simplified BSD, see the license.md file

#pragma once

template<class... Args>
struct resolve {
    template<class Return>
    friend constexpr auto operator|(Return (* pf)(Args...), resolve) noexcept
                                    -> decltype(pf) { return pf; }

    template<class Return, class Class>
    friend constexpr auto operator|(Return (Class::* pmf)(Args...), resolve) noexcept
                                    -> decltype(pmf) { return pmf; }
};

template<class... Args>
struct resolve_const {
    template<class Return, class Class>
    friend constexpr auto operator|(Return (Class::* pmf)(Args...) const, resolve_const) noexcept
                                    -> decltype(pmf) { return pmf; }
};
