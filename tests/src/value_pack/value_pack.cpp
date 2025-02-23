#include <concepts>

#include <constrained/value_pack/value_pack.hpp>

namespace ct::test::value_pack_ {
    consteval void test_self() {
        static_assert(std::same_as<empty::self, empty>);
        static_assert(std::same_as<value_pack<1, 2, 3>::self, value_pack<1, 2, 3>>);
    }

    consteval void test_size() {
        static_assert(empty::size == 0);
        static_assert(value_pack<1, 2, 3, 4, 5>::size == 5);    
    }

    consteval void test_unwrap() {
        static_assert(!traits::unwrappable<empty>); // unwrap [] is bad
        static_assert(traits::unwrappable<value_pack<1>>); // unwrap [a] is ok
        static_assert(!traits::unwrappable<value_pack<1, 2>>); // unwrap [a:as] is bad

        static_assert(value_pack<1>::unwrap == 1); // unwrap [1] == 1
    }

    consteval void test_map() {
        constexpr auto add_one = [](int x) { return x + 1.f; };
        constexpr auto to_void = [](auto x) {};
        constexpr auto nullary = []() {};
        
        struct dummy {};
        constexpr auto map_dummy = [](dummy x) { return x; };
        constexpr auto map_any = [=](auto x) {
            if constexpr (std::same_as<decltype(x), dummy>)
                return map_dummy(x);
            else
                return add_one(x);
        };
        
        // check mappable concept
        
        // any callable is ok if pack is empty
        static_assert(traits::mappable<empty, add_one>); // map [] a -> b is ok
        static_assert(traits::mappable<empty, to_void>); // map [] _ -> void is ok
        static_assert(traits::mappable<empty, nullary>); // map [] void -> _ is ok

        // mapper can't return void or be nullary
        static_assert(traits::mappable<value_pack<1, 2, 3>, add_one>); // map [a] a -> b is ok
        static_assert(!traits::mappable<value_pack<1, 2, 3>, map_dummy>); // map [a] b -> _ is bad
        static_assert(traits::mappable<value_pack<1, 2, 3>, map_any>); // map [a] _ -> b is ok
        static_assert(!traits::mappable<value_pack<1, 2, 3>, to_void>); // map [a] _ -> void is bad
        static_assert(!traits::mappable<value_pack<1, 2, 3>, nullary>); // map [a] void -> _ is bad

        // can't call add_one on dummy
        static_assert(!traits::mappable<value_pack<dummy{}>, add_one>);
        static_assert(traits::mappable<value_pack<dummy{}>, map_dummy>);
        static_assert(traits::mappable<value_pack<dummy{}>, map_any>);
        static_assert(!traits::mappable<value_pack<dummy{}>, to_void>);
        static_assert(!traits::mappable<value_pack<dummy{}>, nullary>);
        
        // constraint satisfied if mapper is ok for all elements of the pack
        static_assert(!traits::mappable<value_pack<1, dummy{}, 3>, add_one>); // map [a|b] a -> c is bad
        static_assert(!traits::mappable<value_pack<1, dummy{}, 3>, map_dummy>); // map [a|b] b -> c is bad
        static_assert(traits::mappable<value_pack<1, dummy{}, 3>, map_any>); // map [a|b] _ -> c is ok
        static_assert(!traits::mappable<value_pack<1, dummy{}, 3>, to_void>); // map [a|b] _ -> void is bad
        static_assert(!traits::mappable<value_pack<1, dummy{}, 3>, nullary>); // map [a|b] void -> is bad

        // check map implementation

        // empty -> empty regardless of mapper
        static_assert(std::same_as<empty::map<add_one>, empty>); // map [] (+1.f) = []
        static_assert(std::same_as<empty::map<nullary>, empty>); // map [] nullary = []

        // map single type
        static_assert(std::same_as<value_pack<0>::map<add_one>, value_pack<1.f>>); // map [0] (+1.f) = [1.f]
        static_assert(std::same_as<value_pack<0, 1, 2, 3>::map<add_one>, value_pack<1.f, 2.f, 3.f, 4.f>>); // map [0, 1, 2, 3] (+1.f) = [1.f, 2.f, 3.f, 4.f]
        
        // map mixed types
        static_assert(std::same_as<value_pack<0, dummy{}, 2, 3>::map<map_any>, value_pack<1.f, dummy{}, 3.f, 4.f>>); // map [0, dummy, 2, 3] (a|b -> c|b) = [1.f, dummy, 3.f, 4.f]
    }

    template <auto... Xs>
    struct another_pack {};

    struct to_another_pack
    {
        template <auto... Xs>
        using type = another_pack<Xs...>;
    };

    struct to_another_pack_single
    {
        template <auto X>
        using type = another_pack<X>;
    };

    struct bad_thenable_typename
    {
        template <typename X>
        using type = X;
    };

    struct bad_thenable_no_type {};

    consteval void test_then() {
        // test thenable concept
        static_assert(!traits::thenable<value_pack<1>, bad_thenable_no_type>); // must be ::type
        static_assert(!traits::thenable<value_pack<1>, bad_thenable_typename>); // ::type

        static_assert(!traits::thenable<empty, to_another_pack_single>); // sizeof(Xs)... == 0 is is bad for X
        static_assert(traits::thenable<value_pack<1>, to_another_pack_single>); // sizeof(Xs)... == 1 is ok for X
        static_assert(!traits::thenable<value_pack<1, 2, 3>, to_another_pack_single>); // sizeof(Xs)... > 1 is bad for X

        static_assert(traits::thenable<empty, to_another_pack>); // sizeof(Xs)... == 0 is ok for Xs...
        static_assert(traits::thenable<value_pack<1>, to_another_pack>); // sizeof(Xs)... == 1 is ok for Xs...
        static_assert(traits::thenable<value_pack<1, 2, 3>, to_another_pack>); // sizeof(Xs)... > 1 is ok for Xs...

        // test then implementation

        static_assert(std::same_as<empty::then<to_another_pack>, another_pack<>>);
        static_assert(std::same_as<value_pack<1>::then<to_another_pack>, another_pack<1>>);
        static_assert(std::same_as<value_pack<0, 1, 2, 3>::then<to_another_pack>, another_pack<0, 1, 2, 3>>);
    }

    struct sum
    {
        template <auto... Xs>
        using type = value_pack<(Xs + ...)>;
    };

    consteval void test_chaining() {
        constexpr auto add_one = [](int x) { return x + 1; };

        static_assert(
            value_pack<1, 2, 3>::map<add_one>::then<sum>::self::unwrap
            == (2 + 3 + 4)
        );

        static_assert(
            value_pack<1, 2, 3>::then<sum>::size
            == 1
        );

        static_assert(std::same_as<
            value_pack<1, 2, 3>::map<add_one>::then<to_another_pack>,
            another_pack<2, 3, 4>
        >);
    }
} // namespace ct::test::value_pack_