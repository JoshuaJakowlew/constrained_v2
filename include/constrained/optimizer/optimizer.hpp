#pragma once

#include <constrained/value_pack/diff.hpp>
#include <constrained/value_pack/unique.hpp>

namespace ct {
    template <auto Eq, typename From, typename To>
    struct common_elimination_pass_impl
    {
    };

    template <auto Eq, auto... Xs, auto... Ys>
    struct common_elimination_pass_impl<Eq, value_pack<Xs...>, value_pack<Ys...>>
    {
        // To \ From
        using type = value_pack<Ys...>
            ::template then<diff<Eq, Xs...>>;
    };

    template <auto Eq, typename From>
    struct common_elimination_pass
    {
        template <auto... Xs>
        using type = common_elimination_pass_impl<Eq, From, value_pack<Xs...>>::type;
    };

    template <auto Eq>
    struct remove_duplicates_pass
    {
        template <auto... Xs>
        using type = value_pack<Xs...>
            ::template then<unique<Eq>>;
    };

    template <typename OptFrom, typename OptTo, typename Result>
    struct optimize_pass_result
    {
        using from = OptFrom;
        using to = OptTo;
        using result = Result;
    };

    template <auto Eq, typename From, typename To>
    struct optimize_pass {};

    template <auto Eq, auto... Xs, auto... Ys>
    struct optimize_pass<Eq, value_pack<Xs...>, value_pack<Ys...>>
    {
        using from = value_pack<Xs...>;
        using to = value_pack<Ys...>;

        using unique_from = from::template then<remove_duplicates_pass<Eq>>;
        using unique_to   = to  ::template then<remove_duplicates_pass<Eq>>;

        using result = unique_to::template then<common_elimination_pass<Eq, unique_from>>;

        using type = optimize_pass_result<unique_from, unique_to, result>;
    };

    // template <bool Loop, auto Eq, typename From, typename To>
    // struct optimize_loop {};

    // template <auto Eq, auto... Xs, auto... Ys>
    // struct optimize_loop<true, Eq, value_pack<Xs...>, value_pack<Ys...>>
    // {
    //     using from = value_pack<Xs...>;
    //     using to = value_pack<Ys...>;

    //     using optimized = optimize_pass<Eq, from, to>::type;
    //     using opt_from = optimized::from;
    //     using opt_to = optimized::to;
    //     using opt_res = optimized::result;

    //     using type = optimize_loop<std::same_as<to, opt_res>, Eq, opt_from, opt_res>::type;

    //     // using type = std::conditional_t<
    //     //     std::same_as<to, opt_res>,
    //     //     opt_res,
    //     //     typename optimize_loop<Eq, opt_from, opt_res>::type
    //     // >;
    // };

    // template <auto Eq, auto... Xs, auto... Ys>
    // struct optimize_loop<false, Eq, value_pack<Xs...>, value_pack<Ys...>>
    // {
    //     using type = value_pack<Ys...>; 
    // };

    template <auto Eq, typename From>
    struct optimize
    {
        template <auto... Xs>
        using type = optimize_pass<Eq, From, value_pack<Xs...>>::type::result;
    };
} // namespace ct