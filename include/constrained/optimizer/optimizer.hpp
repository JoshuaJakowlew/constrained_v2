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

    template <auto Eq, typename From>
    struct optimize
    {
        template <auto... Xs>
        using type = optimize_pass<Eq, From, value_pack<Xs...>>::type::result;
    };

    template <auto Eq>
    struct optimize_predicate_pack
    {
        template <auto... Xs>
        using type = remove_duplicates_pass<Eq>::template type<Xs...>;
    };
} // namespace ct