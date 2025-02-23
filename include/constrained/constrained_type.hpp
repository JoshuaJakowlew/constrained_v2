#pragma once

#include <concepts>
#include <utility>
#include <optional>

#include <constrained/optimizer/constraints/optimize.hpp>
#include <constrained/optimizer/fail_handlers/optimize.hpp>
#include <constrained/optimizer/validators/optimize.hpp>

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


    template <typename T>
    concept member_dereferenceable = requires (T x) { x.operator*(); };

    template <typename T>
    concept nothrow_member_dereferenceable =
        member_dereferenceable<T>
        and noexcept(std::declval<T>().operator*());

    template <typename T>
    concept member_accessible = requires (T x) { x.operator->(); };

    template <typename T>
    concept nothrow_member_accessible = 
        member_accessible<T>
        and noexcept(std::declval<T>().operator->());

#pragma endregion

    struct nocheck {};

    // TODO: Optimize self predicates, fail handlers and validators
    template <
        typename T,
        predicate_pack<T> ConstraintPack,
        fail_handler_pack<T> FailHandlerPack,
        validator_pack<T> ValidatorPack = ConstraintPack,
        auto Eq = type_eq,
        bool disable_forwarding = false
    >
    class constrained_type
    {
    public:
        using value_t = T;
        using optimized_constraints   = ConstraintPack::template then<constraints::pack::optimize<Eq>>;
        using optimized_fail_handlers = FailHandlerPack::template then<fail_handlers::pack::optimize<Eq>>;
        using optimized_validators    = ValidatorPack::template then<validators::pack::optimize<Eq>>;

#pragma region default_constructor
        constexpr constrained_type() 
            noexcept(
                std::is_nothrow_default_constructible_v<T> and
                noexcept(check(optimized_constraints{}))
            )
            requires std::is_default_constructible_v<T>
        {
            check(optimized_constraints{});
        }
#pragma endregion

#pragma region default_nocheck_constructor
        constexpr constrained_type(nocheck)
            noexcept(std::is_nothrow_default_constructible_v<T>)
            requires std::is_default_constructible_v<T>
        {
        };
#pragma endregion

#pragma region forwarding_constructor
        template <typename... Args>
        constexpr constrained_type(Args&&... args)
            noexcept(
                std::is_nothrow_constructible_v<T, Args...> and
                noexcept(check(optimized_constraints{}))
            )
            requires std::is_constructible_v<T, Args...>
            : _value(std::forward<Args>(args)...)
        {
            check(optimized_constraints{});
        }
#pragma endregion

#pragma region copy_constructor
        constexpr constrained_type(constrained_type const & other)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
            requires std::copy_constructible<T>
            : _value(other.value())
        {
        }
#pragma endregion 

#pragma region move_constructor
        constexpr constrained_type(constrained_type && other)
            noexcept(std::is_nothrow_move_constructible_v<T>)
            requires std::move_constructible<T>
            : _value(std::move(other).value())
        {
        }
#pragma endregion 

#pragma region optimizing_copy_constructor
        template <predicate_pack<T> RhsConstraintPack>
        constexpr constrained_type(constrained_type<T, RhsConstraintPack, FailHandlerPack, ValidatorPack, Eq> const & other)
            noexcept(
                std::is_nothrow_copy_constructible_v<T> and
                noexcept(check_from_to<RhsConstraintPack>())
            )
            requires std::copy_constructible<T>
            : _value(other.value())
        {
            check_from_to<RhsConstraintPack>();
        }
#pragma endregion

#pragma region optimizing_move_constructor
        template <predicate_pack<T> RhsConstraintPack>
        constexpr constrained_type(constrained_type<T, RhsConstraintPack, FailHandlerPack, ValidatorPack, Eq> && other)
            noexcept(
                std::is_nothrow_move_constructible_v<T> and
                noexcept(check_from_to<RhsConstraintPack>())
            )
            requires std::move_constructible<T>
            : _value(std::move(other).value())
        {
            check_from_to<RhsConstraintPack>();
        }
#pragma endregion

#pragma region copy_assignment
        constexpr auto operator=(constrained_type const & rhs)
            noexcept(std::is_nothrow_copy_assignable_v<T>)
            -> constrained_type &
            requires std::is_copy_assignable_v<T>
        {
            _value = rhs._value;
            return *this;
        }
#pragma endregion

#pragma region move_assignment
        constexpr auto operator=(constrained_type && rhs)
            noexcept(std::is_nothrow_move_assignable_v<T>)
            -> constrained_type &
            requires std::is_move_assignable_v<T>
        {
            _value = std::move(rhs).value();
            return *this;
        }
#pragma endregion

#pragma region wrapped_value_assignment
        template <typename U>
        constexpr auto operator=(U const & rhs)
            noexcept(
                std::is_nothrow_copy_assignable_v<T> and
                noexcept(check(optimized_constraints{}))
            )
            -> constrained_type &
            requires std::assignable_from<T, U const &>
        {
            _value = rhs;
            check(optimized_constraints{});
            return *this;
        }

        template <typename U>
        constexpr auto operator=(U && rhs)
            noexcept(
                std::is_nothrow_move_assignable_v<T> and 
                noexcept(check(optimized_constraints{}))
            )
            -> constrained_type &
            requires std::assignable_from<T, U &&>
                and (!std::is_reference_v<U>)
        {
            _value = std::move(rhs);
            check(optimized_constraints{});
            return *this;
        }
#pragma endregion

#pragma region optimizing_copy_assignment
        template <predicate_pack<T> RhsConstraintPack>
        constexpr auto operator=(constrained_type<T, RhsConstraintPack, FailHandlerPack, ValidatorPack, Eq> const & other)
            noexcept(
                std::is_nothrow_copy_assignable_v<T> and
                noexcept(check_from_to<RhsConstraintPack>())
            )
            -> constrained_type &
        {
            _value = other.value();
            check_from_to<RhsConstraintPack>();
            return *this;
        }
#pragma endregion

#pragma region optimizing_move_assignment
        template <predicate_pack<T> RhsConstraintPack>
        constexpr auto operator=(constrained_type<T, RhsConstraintPack, FailHandlerPack, ValidatorPack, Eq> && other)
            noexcept(
                std::is_nothrow_move_assignable_v<T> and
                noexcept(check_from_to<RhsConstraintPack>())
            )
            -> constrained_type &
        {
            _value = std::move(other).value();
            check_from_to<RhsConstraintPack>(_value);
            return *this;
        }
#pragma endregion

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

#pragma region value
        constexpr auto value() const & noexcept -> T const &
        { return _value; }
        constexpr auto value() && noexcept -> T &&
        { return std::move(_value); }
        constexpr auto value() const && noexcept -> T const &&
        { return std::move(_value); }
#pragma endregion value

#pragma region dereference_operators
        static constexpr bool forwarding_member_dereference = member_dereferenceable<T> && !disable_forwarding;
        static constexpr bool non_forwarding_dereference = !forwarding_member_dereference;

        constexpr decltype(auto) operator*() const &
            noexcept(nothrow_member_dereferenceable<T>)
            requires forwarding_member_dereference
        { return _value.operator*(); }

        constexpr decltype(auto) operator*() &&
            noexcept(nothrow_member_dereferenceable<T>)
            requires forwarding_member_dereference
        { return std::move(_value).operator*(); }

        constexpr decltype(auto) operator*() const &&
            noexcept(nothrow_member_dereferenceable<T>)
            requires forwarding_member_dereference
        { return std::move(_value).operator*(); }

        constexpr auto operator*() const & noexcept -> T const &
            requires non_forwarding_dereference
        { return _value; }
        constexpr auto operator*() const && noexcept -> T const &&
            requires non_forwarding_dereference
        { return std::move(_value); }
        constexpr auto operator*() && noexcept -> T &&
            requires non_forwarding_dereference
        { return std::move(_value); }
#pragma endregion

#pragma region access_operators
    static constexpr bool forwarding_member_access = member_accessible<T> && !disable_forwarding;
    static constexpr bool non_forwarding_access = !forwarding_member_access;

    constexpr decltype(auto) operator->() const &
        noexcept(nothrow_member_accessible<T>)
        requires forwarding_member_access
    { return _value.operator->(); }
    constexpr decltype(auto) operator->() &&
        noexcept(nothrow_member_accessible<T>)
        requires forwarding_member_access
    { return std::move(_value).operator->(); }
    constexpr decltype(auto) operator->() const &&
        noexcept(nothrow_member_accessible<T>)
        requires forwarding_member_access
    { return std::move(_value).operator->(); }

    [[nodiscard]] constexpr auto operator->() const & noexcept -> const T*
        requires non_forwarding_access
    { return &_value; }

    [[nodiscard]] constexpr auto operator->() && noexcept -> T*
        requires non_forwarding_access
    { return &_value; }

    [[nodiscard]] constexpr auto operator->() const && noexcept -> const T*
        requires non_forwarding_access
    { return &_value; }

#pragma endregion

#pragma region value_or
        template <std::convertible_to<T> U>
        constexpr auto value_or(U && default_value) const &
            noexcept(noexcept(valid()) and std::is_nothrow_constructible_v<T, U>)
            -> T
        {
            if (valid()) return _value;
            return static_cast<T>(std::forward<U>(default_value));
        }

        template <std::convertible_to<T> U>
        constexpr auto value_or(U && default_value) &&
            noexcept(noexcept(valid()) and std::is_nothrow_constructible_v<T, U>)
            -> T
        {
            if (valid()) return std::move(_value);
            return static_cast<T>(std::forward<U>(default_value));
        }
#pragma endregion

#pragma region to_optional
        template <template <typename> typename Optional = std::optional>
        constexpr auto to_optional() const &
            noexcept(
                noexcept(valid()) and
                std::is_nothrow_constructible_v<Optional<T>, T const &>
            )
            -> Optional<T>
            requires std::constructible_from<Optional<T>, T const &>
        {
            if (!valid()) return std::nullopt;
            return Optional(_value);
        }

        template <template <typename> typename Optional = std::optional>
        constexpr auto to_optional() &&
            noexcept(
                noexcept(valid()) and
                std::is_nothrow_constructible_v<Optional<T>, T &&>
            )
            -> Optional<T>
            requires std::constructible_from<Optional<T>, T &&>
        {
            if (!valid()) return std::nullopt;
            return Optional(std::move(_value));
        }

        template <template <typename> typename Optional = std::optional>
        constexpr auto to_optional() const &&
            noexcept(
                noexcept(valid()) and
                std::is_nothrow_constructible_v<Optional<T>, T const &&>
            )
            -> Optional<T>
            requires std::constructible_from<Optional<T>, T const &&>
        {
            if (!valid()) return std::nullopt;
            return Optional(std::move(_value));
        }
#pragma endregion

        template <auto Convert, typename U>
        static constexpr auto from(U && rhs)
            noexcept(noexcept(constrained_type{Convert(std::forward<U>(rhs))}))
            -> constrained_type
        {
            return constrained_type{Convert(std::forward<U>(rhs))};
        }
    private:
        T _value{};

#pragma region check_from_to
        template <predicate_pack<T> RhsConstraintPack>
        using optimized_from_to_constraints = ConstraintPack
            ::template then<constraints::construct::optimize<Eq, RhsConstraintPack>>;

        template <predicate_pack<T> RhsConstraintPack>
        constexpr void check_from_to()
            noexcept(
                noexcept(valid()) and
                noexcept(check(optimized_from_to_constraints<RhsConstraintPack>{}))
            )
        {
            if (valid()) // if invalid - no need for checks as validators are the same
                check(optimized_from_to_constraints<RhsConstraintPack>{});
        }
#pragma endregion

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

    template <typename From, typename To, auto Convert>
    struct mapper
    {
        mapper(From && from) 
            : to{Convert(std::forward<From>(from))}
        {}
        operator To() { return to; }

        To to;
    };
} // namespace ct