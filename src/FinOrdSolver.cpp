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
            Term reflection = solver.mkTuple({elementSort, elementSort}, 
                                             {elements[i], elements[i]});
            solver.assertFormula(solver.mkTerm(SET_MEMBER, reflection, binaryRelation));
        }

        for (auto &it : queries) {
            Term existsRelation = solver.mkTuple({elementSort, elementSort},
                                                {elements[it.first], elements[it.second]});
            Term forbiddenRelation = solver.mkTuple({elementSort, elementSort},
                                            {elements[it.second], elements[it.first]});

            Term existMember = solver.mkTerm(SET_MEMBER, existsRelation, binaryRelation);

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
    std::vector<int32_t> result = getMaximumElements();
    return result.size() == 1 ? result[0] : -1;
}

int32_t FinOrdSolver::getLeastElement() const
{
    std::vector<int32_t> result = getMinimumElements();
    return result.size() == 1 ? result[0] : -1;
}

std::vector<int32_t> FinOrdSolver::getMaximumElements() const
{
    std::vector<int32_t> result;

    for (auto &assumedElement : elements) {
        std::vector<Term> assertions;

        for (auto &element : elements) {
            if (element == assumedElement) {
                continue;
            }

            Term relation = solver.mkTuple({elementSort, elementSort}, {assumedElement, element});
            assertions.push_back(solver.mkTerm(NOT, solver.mkTerm(SET_MEMBER, relation, binaryRelation)));
        }

        if (solver.checkSatAssuming(assertions).isSat()) {
            result.push_back(std::stoi(assumedElement.getSymbol()));
        }
    }

    return result;
}

std::vector<int32_t> FinOrdSolver::getMinimumElements() const
{
    std::vector<int32_t> result;

    for (auto &assumedElement : elements) {
        std::vector<Term> assertions;

        for (auto &element : elements) {
            if (element == assumedElement) {
                continue;
            }
            Term relation = solver.mkTuple({elementSort, elementSort}, {element, assumedElement});
            assertions.push_back(solver.mkTerm(NOT, solver.mkTerm(SET_MEMBER, relation, binaryRelation)));
        }

        if (solver.checkSatAssuming(assertions).isSat()) {
            result.push_back(std::stoi(assumedElement.getSymbol()));
        }
    }

    return result;
}

bool FinOrdSolver::isLinear() const
{
    return false;
}