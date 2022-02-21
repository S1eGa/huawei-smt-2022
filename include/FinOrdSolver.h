#ifndef FIN_ORD_SOLVER_
#define FIN_ORD_SOLVER_

#include <cvc5/cvc5.h>
#include <vector>
#include <utility>

class FinOrdSolver
{
private:
    cvc5::api::Solver solver;

    cvc5::api::Sort elementSort;
    std::vector<cvc5::api::Term> elements;

    cvc5::api::Term binaryRelation;

    std::vector<std::pair<int32_t, int32_t>> queries;

public:
    FinOrdSolver(size_t elementsCount, const std::vector<std::pair<int32_t, int32_t>> &queries);

    int32_t getGreatestElement() const;

    int32_t getLeastElement() const;

    std::vector<int32_t> getMaximumElements() const;

    std::vector<int32_t> getMinimumElements() const;

    bool isLinear() const;

    std::vector<std::pair<int32_t, int32_t>> getTransitiveReduction() const;
};

#endif // FIN_ORD_SOLVER_