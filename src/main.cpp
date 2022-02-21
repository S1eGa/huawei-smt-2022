#include "FinOrdSolver.h"
#include "AntisymmetryException.h"
#include <iostream>
#include <vector>
#include <set>

int32_t main()
{
    if (!std::freopen("input.txt", "r", stdin))
    {
        return 1;
    }
    if (!std::freopen("output.txt", "w", stdout))
    {
        return 1;
    }

    size_t n, k;
    std::cin >> n >> k;

    std::vector<std::pair<int32_t, int32_t>> queries(k);
    for (auto &it : queries)
    {
        int32_t left_arg, right_arg;
        std::cin >> left_arg >> right_arg;
        it = std::pair<int32_t, int32_t>(left_arg, right_arg);
    }

    try
    {
        FinOrdSolver solver(n, queries);
        std::cout << "SAT!" << std::endl;
        std::cout << solver.getGreatestElement() << std::endl;
        std::cout << solver.getLeastElement() << std::endl;

        for (auto &it : solver.getMaximumElements())
        {
            std::cout << it << " ";
        }
        std::cout << std::endl;

        for (auto &it : solver.getMinimumElements())
        {
            std::cout << it << " ";
        }
        std::cout << std::endl;

        std::cout << solver.isLinear() << std::endl;

        for (auto &it : solver.getTransitiveReduction())
        {
            std::cout << it.first << " " << it.second << std::endl;
        }
    }
    catch (const AntisymmetryException &e)
    {
        std::cout << "Not SAT!" << std::endl;
    }
}
