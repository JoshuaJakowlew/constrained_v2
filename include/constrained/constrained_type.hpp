#pragma once

#include <concepts>
#include <utility>

namespace ct {
    template <typename F, typename T>
    concept predicate = std::predicate<F, T const &>;

    template <typename F, typename T>
    concept nothrow_predicate = 
        predicate<F, T>
        and std::is_nothrow_invocable_v<F, T const &>;

    template <typename F, typename T>
    concept fail_handler =
        std::invocable<F, T &> and
        std::is_void_v<std::invoke_result_t<F, T &>>;

    template <typename F, typename T>
    concept nothrow_fail_handler =
        fail_handler<F, T> and
        std::is_nothrow_invocable_v<F, T &>;

    template <
        typename T,
        auto Predicate,
        auto Fail,
        auto HasValue = Predicate
    >
        requires
            predicate<decltype(Predicate), T> and
            fail_handler<decltype(Fail), T> and
            predicate<decltype(HasValue), T>
    class constrained_type
    {
    public:
        constexpr constrained_type() 
            noexcept(std::is_nothrow_default_constructible_v<T>)
            requires std::is_default_constructible_v<T>
        {}

        template <typename... Args>
        constexpr constrained_type(Args&&... args)
            noexcept(
                std::is_nothrow_constructible_v<T, Args...> and
                noexcept(check())
            )
            requires std::is_constructible_v<T, Args...>
            : _value(std::forward<Args>(args)...)
        { check(); }

        constexpr bool valid() const
            noexcept(nothrow_predicate<decltype(HasValue), T>)
        { return HasValue(_value); }

        constexpr explicit operator bool () const
            noexcept(valid())
        { return valid(); }

        constexpr auto value() const & noexcept -> T const &
        { return _value; }
        constexpr auto value() && noexcept -> T &&
        { return std::move(_value); }
        constexpr auto value() const && noexcept -> T const &&
        { return std::move(_value); }
    private:
        T _value;

        constexpr void check()
            noexcept(
                nothrow_predicate<decltype(Predicate), T> and
                noexcept(fail())
            )
        {
            if (!Predicate(_value))
                fail();
        }

        constexpr void fail()
            noexcept (nothrow_fail_handler<decltype(Fail), T>)
        {
            Fail(_value);
        }
    };
} // namespace ct {