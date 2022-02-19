#include <cvc5/cvc5.h>

#include <iostream>
#include <vector>

using namespace cvc5::api;

int32_t main()
{
    Solver solver;
    solver.setLogic("QF_ALL");
    solver.setOption("produce-unsat-cores", "true");

    Sort finite_ord_element = solver.mkUninterpretedSort("FiniteOrd1");

    int32_t n, k;
    std::cin >> n >> k;

    std::vector<Term> element_terms(n);
    for (int32_t i = 0; i < n; ++i)
    {
        element_terms[i] = solver.mkConst(finite_ord_element, std::to_string(i));
    }

    Term binary_relation = solver.mkConst(solver.mkSetSort(
        solver.mkTupleSort(
            {finite_ord_element, finite_ord_element}
        )
    ), "relation");

    for (int32_t i = 0; i < k; ++i)
    {
        int32_t left_arg, right_arg;
        std::cin >> left_arg >> right_arg;
        
        Term exists_relation = solver.mkTuple({finite_ord_element, finite_ord_element},
                                        {element_terms[left_arg], element_terms[right_arg]});
        Term forbidden_relation = solver.mkTuple({finite_ord_element, finite_ord_element},
                                        {element_terms[right_arg], element_terms[left_arg]});

        Term binary_relation_closure = solver.mkTerm(RELATION_TCLOSURE, binary_relation);
        Term exist_member = solver.mkTerm(SET_MEMBER, exists_relation, binary_relation_closure);
        Term forbidden_member = solver.mkTerm(SET_MEMBER, forbidden_relation, binary_relation_closure);
        Term not_forbidden_member = solver.mkTerm(NOT, forbidden_member);

        solver.assertFormula(exist_member);
        solver.assertFormula(not_forbidden_member);
    }

    std::cout << solver.checkSat() << std::endl;
    // Result result = solver.checkSat();
    // if (result.isUnsat()) {
    //     std::vector<Term> unsat_core = solver.getUnsatCore();
    //     for (auto &it : unsat_core) {
    //         std::cout << it[0].getSymbol() << it[0].getSymbol() << "\n";
    //     }
    // }
}
