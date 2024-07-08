#pragma once

#include <concepts>
#include <utility>

#include <iostream>

#include <constrained/optimizer/optimizer.hpp>
#include <constrained/optimizer/all.hpp>
#include <constrained/combinators/all.hpp>
#include <constrained/value_pack/to.hpp>

namespace ct {
    inline constexpr auto type_eq = []<class T, class U>(T const & x, U const & y) {
        return std::same_as<T, U>;
    };

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

    namespace detail {
        template <typename P, typename T>
        struct is_predicate_pack : std::false_type {};

        template <auto... Ps, typename T>
            requires (... && predicate<decltype(Ps), T>)
        struct is_predicate_pack<value_pack<Ps...>, T> : std::true_type {};

        template <typename P, typename T>
        struct is_fail_handler_pack : std::false_type {};

        template <auto... Hs, typename T>
            requires (... && fail_handler<decltype(Hs), T>)
        struct is_fail_handler_pack<value_pack<Hs...>, T> : std::true_type {};
    } // namespace detail

    template <typename Pack, typename T>
    concept predicate_pack = detail::is_predicate_pack<Pack, T>::value;

    template <typename Pack, typename T>
    concept fail_handler_pack = detail::is_fail_handler_pack<Pack, T>::value;

    struct is_even {
        constexpr bool operator ()(auto x) const {
            std::cout  << "[is_even]\n";
            return x % 2 == 0;

        }
    };

    struct gt_3 {
        constexpr bool operator ()(auto x) const {
            std::cout << "[gt3]\n";
            return x > 3;
        }
    };

    struct nocheck {};

    template <
        typename T,
        typename ConstraintPack,
        typename OnFailPack,
        typename HasValuePack = ConstraintPack
    >
        requires
            predicate_pack<ConstraintPack, T> and
            fail_handler_pack<OnFailPack, T> and
            predicate_pack<HasValuePack, T>

    class constrained_type
    {
    public:
        constexpr constrained_type() 
            noexcept(
                std::is_nothrow_default_constructible_v<T> and
                noexcept(check(ConstraintPack{}))
            )
            requires std::is_default_constructible_v<T>
        {
            std::puts("default constructor");
            check(ConstraintPack{});
        }

        constexpr constrained_type(nocheck)
            noexcept(std::is_nothrow_default_constructible_v<T>)
            requires std::is_default_constructible_v<T>
        {
            std::puts("default nocheck constructor");
        };

        template <typename... Args>
        constexpr constrained_type(Args&&... args)
            noexcept(
                std::is_nothrow_constructible_v<T, Args...> and
                noexcept(check(ConstraintPack{}))
            )
            requires std::is_constructible_v<T, Args...>
            : _value(std::forward<Args>(args)...)
        { 
            std::puts("emplace constructor");
            check(ConstraintPack{});
        }

        constexpr constrained_type(constrained_type const & other)
            : _value(other._value)
        {
            std::puts("copy constructor (no checks performed)");
        }

        template <auto Eq = type_eq, predicate_pack<T> RhsConstraints>
            requires (!std::same_as<ConstraintPack, RhsConstraints>)
        constexpr constrained_type(constrained_type<T, RhsConstraints, OnFailPack, HasValuePack> const & other)
            : _value(other.value())
        {
            std::puts("create from other constructor");
            using constraints = optimize_pass<Eq, RhsConstraints, ConstraintPack>::type::result;
            if (valid()) check(constraints{});
        }

        constexpr bool valid() const
            noexcept(noexcept(valid_impl(HasValuePack{})))
        {
            return valid_impl(HasValuePack{});
        }

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
        T _value{}; // TODO: Allow non-initializing deafult construction via flags like nocheck

        template <auto... Validator>
        constexpr bool valid_impl(value_pack<Validator...>) const
            noexcept(noexcept((... && Validator(_value))))
        {
            return (... && Validator(_value));
        }

        template <auto... Predicate>
        constexpr void check(value_pack<Predicate...>)
            noexcept(
                noexcept(!(... && Predicate(_value))) and
                noexcept(fail(OnFailPack{}))
            )
        {
            std::cout << "[";
            if (!(... && Predicate(_value)))
                fail(OnFailPack{});
            std::cout << "]" << std::endl;
        }

        template <auto... FailHandler>
        constexpr void fail(value_pack<FailHandler...>)
            noexcept (noexcept((... , FailHandler(_value))))
        {
            (... , FailHandler(_value));
        }
    };
} // namespace ct {