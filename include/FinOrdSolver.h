#ifndef FIN_ORD_SOLVER_
#define FIN_ORD_SOLVER_

#include <cvc5/cvc5.h>
#include <vector>
#include <utility>

class FinOrdSolver {
private:
    cvc5::api::Solver solver; 
    cvc5::api::Sort elementSort;
    cvc5::api::Sort binaryRelationSort;
    cvc5::api::Term binaryRelation;

    std::vector<cvc5::api::Term> elements;
public:
    FinOrdSolver(size_t elementsCount, const std::vector<std::pair<int32_t, int32_t>> &queries);

    int32_t getGreatestElement() const;

    int32_t getLeastElement() const;

    int32_t getMaximumElement() const;

    int32_t getMinimumElement() const;

    bool isLinear() const;
};

#endif // FIN_ORD_SOLVER_