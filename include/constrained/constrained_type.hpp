#pragma once

#include <concepts>
#include <utility>

#include <iostream>

#include <constrained/optimizer/constraints/optimize.hpp>
#include <constrained/optimizer/fail_handlers/optimize.hpp>
#include <constrained/optimizer/validators/optimize.hpp>

#include <constrained/combinators/all.hpp>
#include <constrained/value_pack/to.hpp>

namespace ct {
    inline constexpr auto type_eq = []<class T, class U>(T const & x, U const & y) {
        return std::same_as<T, U>;
    };

#pragma region concepts
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

    template <typename F, typename T>
    concept validator = std::predicate<F, T>;

    template <typename F, typename T>
    concept nothrow_validator = nothrow_predicate<F, T>;

    namespace detail {
        template <typename P, typename T>
        struct is_predicate_pack : std::false_type {};

        template <auto... Ps, typename T>
            requires (... && predicate<decltype(Ps), T>)
        struct is_predicate_pack<value_pack<Ps...>, T> : std::true_type {};

        template <typename P, typename T>
        struct is_nothrow_predicate_pack : std::false_type {};

        template <auto... Ps, typename T>
            requires (... && nothrow_predicate<decltype(Ps), T>)
        struct is_nothrow_predicate_pack<value_pack<Ps...>, T> : std::true_type {};


        template <typename H, typename T>
        struct is_fail_handler_pack : std::false_type {};

        template <auto... Hs, typename T>
            requires (... && fail_handler<decltype(Hs), T>)
        struct is_fail_handler_pack<value_pack<Hs...>, T> : std::true_type {};

        template <typename H, typename T>
        struct is_nothrow_fail_handler_pack : std::false_type {};

        template <auto... Hs, typename T>
            requires (... && nothrow_fail_handler<decltype(Hs), T>)
        struct is_nothrow_fail_handler_pack<value_pack<Hs...>, T> : std::true_type {};

        template <typename P, typename T>
        struct is_validator_pack : is_predicate_pack<P, T> {};

        template <typename P, typename T>
        struct is_nothrow_validator_pack : is_nothrow_predicate_pack<P, T> {};
    } // namespace detail

    template <typename Pack, typename T>
    concept predicate_pack = detail::is_predicate_pack<Pack, T>::value;

    template <typename Pack, typename T>
    concept nothrow_predicate_pack = detail::is_nothrow_predicate_pack<Pack, T>::value;

    template <typename Pack, typename T>
    concept fail_handler_pack = detail::is_fail_handler_pack<Pack, T>::value;

    template <typename Pack, typename T>
    concept nothrow_fail_handler_pack = detail::is_nothrow_fail_handler_pack<Pack, T>::value;

    template <typename Pack, typename T>
    concept validator_pack = predicate_pack<Pack, T>;

    template <typename Pack, typename T>
    concept nothrow_validator_pack = nothrow_predicate_pack<Pack, T>;
#pragma endregion

    struct nocheck {};

    // TODO: Optimize self predicates, fail handlers and validators
    template <
        typename T,
        predicate_pack<T> ConstraintPack,
        fail_handler_pack<T> FailHandlerPack,
        validator_pack<T> ValidatorPack = ConstraintPack,
        auto Eq = type_eq
    >
    class constrained_type
    {
    public:
        using optimized_constraints   = ConstraintPack::template then<constraints::pack::optimize<Eq>>;
        using optimized_fail_handlers = FailHandlerPack::template then<fail_handlers::pack::optimize<Eq>>;
        using optimized_validators    = ValidatorPack::template then<validators::pack::optimize<Eq>>;

// #pragma region default_constructor
//         constexpr constrained_type() 
//             noexcept(
//                 std::is_nothrow_default_constructible_v<T> and
//                 noexcept(check(Constraints{}))
//             )
//             requires std::is_default_constructible_v<T>
//         {
//             std::puts("default constructor");
//             check(Constraints{});
//         }
// #pragma endregion

// #pragma region default_nocheck_constructor
//         constexpr constrained_type(nocheck)
//             noexcept(std::is_nothrow_default_constructible_v<T>)
//             requires std::is_default_constructible_v<T>
//         {
//             std::puts("default nocheck constructor");
//         };
// #pragma endregion

// #pragma region forwarding_coonstructor
//         template <typename... Args>
//         constexpr constrained_type(Args&&... args)
//             noexcept(
//                 std::is_nothrow_constructible_v<T, Args...> and
//                 noexcept(check(Constraints{}))
//             )
//             requires std::is_constructible_v<T, Args...>
//             : _value(std::forward<Args>(args)...)
//         { 
//             std::puts("emplace constructor");
//             check(Constraints{});
//         }
// #pragma endregion

// #pragma region copy_constructor
//         constexpr constrained_type(constrained_type const & other)
//             : _value(other.value())
//         {
//             std::puts("copy constructor (no checks performed)");
//         }
// #pragma endregion 

// #pragma region move_constructor
//         constexpr constrained_type(constrained_type && other)
//             noexcept(std::is_nothrow_move_constructible_v<T>)
//             requires std::move_constructible<T>
//             : _value(std::move(other).value())
//         {
//             std::puts("move constructor (no checks performed)");
//         }
// #pragma endregion 

// #pragma region optimizing_copy_constructor
//         template <predicate_pack<T> RhsConstraints>
//             requires (!std::same_as<ConstraintPack, RhsConstraints>)
//         constexpr constrained_type(constrained_type<T, RhsConstraints, OnFailPack, HasValuePack> const & other)
//             // TODO: Add noexcept rules
//             : _value(other.value())
//         {
//             std::puts("create from other constructor");

//             // TODO: Move these check to separate method because it's the same in copy and move constructors
//             using constraints = Constraints::template then<constraints::construct::optimize<Eq, RhsConstraints>>;
//             if (valid()) check(constraints{});
//         }
// #pragma endregion

// #pragma region optimizing_move_constructor
//         template <predicate_pack<T> RhsConstraints>
//             requires (!std::same_as<ConstraintPack, RhsConstraints>)
//         constexpr constrained_type(constrained_type<T, RhsConstraints, OnFailPack, HasValuePack> && other)
//             // TODO: Add noexcept rules
//             : _value(std::move(other).value())
//         {
//             std::puts("create from other constructor");
//             using constraints = Constraints::template then<constraints::construct::optimize<Eq, RhsConstraints>>;
//             if (valid()) check(constraints{});
//         }
// #pragma endregion

        constexpr bool valid() const
            noexcept(noexcept(validate(optimized_validators{})))
        {
            return validate(optimized_validators{});
        }

        constexpr explicit operator bool () const
            noexcept(noexcept(valid()))
        {
            return valid();
        }

        constexpr auto value() const & noexcept -> T const &
        { return _value; }
        constexpr auto value() && noexcept -> T &&
        { return std::move(_value); }
        constexpr auto value() const && noexcept -> T const &&
        { return std::move(_value); }
    private:
        T _value{};

        template <auto... Ps>
        constexpr void check(value_pack<Ps...>)
            noexcept(
                nothrow_predicate_pack<value_pack<Ps...>, T> and
                noexcept(fail(optimized_fail_handlers{}))
            )
        {
            if (!(... && Ps(_value)))
                fail(optimized_fail_handlers{});
        }

        template <auto... Fs>
        constexpr void fail(value_pack<Fs...>)
            noexcept(nothrow_fail_handler_pack<value_pack<Fs...>, T>)
        {
            (... , Fs(_value));
        }

        template <auto... Vs>
        constexpr bool validate(value_pack<Vs...>) const
            noexcept(nothrow_validator_pack<value_pack<Vs...>, T>)
        {
            return (... && Vs(_value));
        }
    };
} // namespace ct {