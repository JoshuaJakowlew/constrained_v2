#include <concepts>

#include <constrained/value_pack/concat.hpp>

namespace ct::test {
    using p1 = value_pack<0, 1, 2>;
    using p2 = value_pack<3, 4, 5>;
    using p3 = value_pack<0, 1, 2, 3, 4, 5>;

    static_assert(std::same_as<empty::then<concat<>>, empty>);     // 0 + 0 == 0
    static_assert(std::same_as<p1::then<concat<>>, p1>);           // x + 0 == x
    static_assert(std::same_as<empty::then<concat<0, 1, 2>>, p1>); // 0 + x == x
    static_assert(std::same_as<p1::then<concat<3, 4, 5>>, p3>);    // x + y == x + y

    static_assert(std::same_as<empty::then<concat_pack<>>, empty>);      // 0 + 0 == 0
    static_assert(std::same_as<p1::then<concat_pack<>>, p1>);            // x + 0 == 0
    static_assert(std::same_as<empty::then<concat_pack<empty>>, empty>); // 0 + 0 == 0
    static_assert(std::same_as<empty::then<concat_pack<p1>>, p1>);       // x + 0 == x
    static_assert(std::same_as<p1::then<concat_pack<empty>>, p1>);       // 0 + x == x
    static_assert(std::same_as<p1::then<concat_pack<p2>>, p3>);          // x + y == x + y
    
    static_assert(std::same_as<empty::then<concat_pack<empty, empty>>, empty>);                               // (0 + 0) + 0 == 0
    static_assert(std::same_as<empty::then<concat_pack<empty, empty, empty>>, empty>);                        // ((0 + 0) + 0) + 0 == 0
    static_assert(std::same_as<p1::then<concat_pack<p2, p1>>, value_pack<0, 1, 2, 3, 4, 5, 0, 1, 2>>);        // (x + y) + z == x + y + z
    static_assert(std::same_as<p1::then<concat_pack<p2, empty, p1>>, value_pack<0, 1, 2, 3, 4, 5, 0, 1, 2>>); // ((x + y) + 0) + z == x + y + z

    static_assert(std::same_as<concat_pack_of_packs<>::type, empty>);                                           // concat([]) == 0
    static_assert(std::same_as<concat_pack_of_packs<empty>::type, empty>);                                      // concat([0]) == 0
    static_assert(std::same_as<concat_pack_of_packs<empty, empty>::type, empty>);                               // concat([0, 0]) == 0
    static_assert(std::same_as<concat_pack_of_packs<p1, empty, p2, empty>::type, p3>);                          // concat([x, 0, y, 0]) == concat([x, y])
    static_assert(std::same_as<concat_pack_of_packs<p1, p2, p1>::type, value_pack<0, 1, 2, 3, 4, 5, 0, 1, 2>>); // concat([x, y, x]) == // concat([x, y, x]) 
} // namespace ct::test