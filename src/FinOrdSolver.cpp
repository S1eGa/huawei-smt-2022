#include "FinOrdSolver.h"
#include "AntisymmetryException.h"
#include <vector>
#include <iostream>

using namespace cvc5::api;

FinOrdSolver::FinOrdSolver(size_t elementsCount, const std::vector<std::pair<int32_t, int32_t>> &queries)
    : elements(elementsCount)
{
    solver.setLogic("QF_ALL");
    solver.setOption("produce-unsat-cores", "true");
    solver.setOption("mbqi", "trust");

    elementSort = solver.mkUninterpretedSort("FinitOrd");
    binaryRelationSort = solver.mkSetSort(solver.mkTupleSort({elementSort, elementSort}));
    binaryRelation = solver.mkConst(binaryRelationSort);
    Term binaryRelationClosure = solver.mkTerm(RELATION_TCLOSURE, binaryRelation);
    
    {
        for (size_t i = 0; i < elementsCount; ++i)
        {
            elements[i] = solver.mkConst(elementSort, std::to_string(i));
        }

        for (auto &it : queries) {
            Term existsRelation = solver.mkTuple({elementSort, elementSort},
                                                {elements[it.first], elements[it.second]});
            Term forbiddenRelation = solver.mkTuple({elementSort, elementSort},
                                            {elements[it.second], elements[it.first]});

            Term existMember = solver.mkTerm(SET_MEMBER, existsRelation, binaryRelationClosure);

            Term forbiddenMember = solver.mkTerm(SET_MEMBER, forbiddenRelation, binaryRelationClosure);
            Term notForbiddenMember = solver.mkTerm(NOT, forbiddenMember);

            solver.assertFormula(existMember);
            solver.assertFormula(notForbiddenMember);
        }
    }

    Result result = solver.checkSat();
    if (result.isUnsat()) {
        throw AntisymmetryException("Doesn't correspond antisymmetry properties");
    }
}


int32_t FinOrdSolver::getGreatestElement() const
{
    // EXISTS x: FORALL y : (x, y) IN binaryRelationClosure

    Term existsVar = solver.mkVar(elementSort, "x");
    Term forallVar = solver.mkVar(elementSort, "y");

    Term instancedVar = solver.mkTerm(INST_PATTERN_LIST, solver.mkTerm(INST_PATTERN, elements));

    // (x, y) in binaryRelationClosure
    Term commonElementPair = solver.mkTuple({elementSort, elementSort}, {forallVar, existsVar});
    Term binaryRelationClosure = solver.mkTerm(RELATION_TCLOSURE, binaryRelation);
    Term leastElement = solver.mkTerm(SET_MEMBER, commonElementPair, binaryRelationClosure);

    // FORALL y
    Term quantifiedForallVar = solver.mkTerm(VARIABLE_LIST, forallVar);
    Term forallElement = solver.mkTerm(FORALL, quantifiedForallVar, leastElement);//, instancedVar);

    // EXISTS x
    Term quantifiedExistsVar = solver.mkTerm(VARIABLE_LIST, existsVar);
    Term existsElement = solver.mkTerm(EXISTS, quantifiedExistsVar, forallElement, instancedVar);
    
    Result result = solver.checkSatAssuming(existsElement);
    std::cout << existsElement << std::endl;
    return result.isSat();
}

int32_t FinOrdSolver::getLeastElement() const
{
    return 0;
}

int32_t FinOrdSolver::getMaximumElement() const
{
    return 0;
}

int32_t FinOrdSolver::getMinimumElement() const
{
    return 0;
}

bool FinOrdSolver::isLinear() const
{
    return false;
}