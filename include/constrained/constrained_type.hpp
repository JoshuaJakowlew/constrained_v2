#pragma once

#include <concepts>
#include <utility>

#include <iostream>

#include <constrained/optimizers/optimizer.hpp>
#include <constrained/optimizers/all.hpp>
#include <constrained/combinators/all.hpp>
#include <constrained/value_pack/to.hpp>

namespace ct {
    constexpr auto type_eq = []<class T, class U>(T const & x, U const & y) {
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
        { 
            std::puts("emplace constructor");
            check<Predicate>();
        }

        constexpr constrained_type(constrained_type const & other)
            : _value(other._value)
        {
            std::puts("copy constructor");
        }

        template <auto FromPredicate, auto Eq = type_eq>
        constexpr constrained_type(constrained_type<T, FromPredicate, Fail, HasValue> const & other)
            // noexcept
            // requires
            : _value(other.value())
        {
            // TODO: this copy constructor must use optimizer
            
            using from_t = std::decay_t<decltype(FromPredicate)>;
            using to_t = std::decay_t<decltype(Predicate)>;

            using passed_from_t = std::conditional_t<combinator<from_t>, from_t, value_pack<from_t{}>>;
            using passed_to_t = std::conditional_t<combinator<to_t>, to_t, value_pack<to_t{}>>;

            // static_assert(std::same_as<all<is_even{}>, all<is_even{}>>);
            // static_assert(std::same_as<passed_from_t, all<is_even{}>>);
            // static_assert(std::same_as<passed_to_t, all<gt_3{}, is_even{}>>);

            using OptT = optimizer<Eq, passed_from_t, passed_to_t>::type;
            // static_assert(std::same_as<OptT, all<is_even{}>{}>>);
            check<OptT{}>();

            // using predicate_pack = optimizer<Eq, std::decay_t<decltype(FromPredicate)>, std::decay_t<decltype(Predicate)>>
            //     ::type;
            std::puts("other copy constructor");
            // predicate_pack{}(_value);
            // check<predicate_pack{}>();

            // using opt3 = optimizer<
            //     type_eq,
            //     const all<is_even{}>,
            //     const all<gt_3{}, is_even{}>
            // >::type;
            // static_assert(std::same_as<opt3, all<gt_3{}>>);
        }

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

        template <auto... Ps>
        constexpr void check()
            noexcept(
                (... && nothrow_predicate<decltype(Ps), T>) and
                noexcept(fail())
            )
        {
            std::cout << "check: sizeof...(Ps) == " << sizeof...(Ps) << std::endl;
            if (!(... && Ps(_value)))
                fail();
        }

        constexpr void fail()
            noexcept (nothrow_fail_handler<decltype(Fail), T>)
        {
            Fail(_value);
        }
    };
} // namespace ct {