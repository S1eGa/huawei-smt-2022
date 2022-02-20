#include "FinOrdSolver.h"
#include "AntisymmetryException.h"
#include <vector>
#include <iostream>

using namespace cvc5::api;

FinOrdSolver::FinOrdSolver(size_t elementsSize, const std::vector<std::pair<int32_t, int32_t>> &queries)
    : elements(elementsSize)
{
    solver.setLogic("QF_ALL");
    solver.setOption("produce-unsat-cores", "true");

    elementSort = solver.mkUninterpretedSort("FinitOrd");
    Sort binaryRelationSort = solver.mkSetSort(solver.mkTupleSort({elementSort, elementSort}));
    binaryRelation = solver.mkConst(binaryRelationSort);
    
    for (size_t i = 0; i < elementsSize; ++i)
    {
        elements[i] = solver.mkConst(elementSort, std::to_string(i));
        Term reflection = solver.mkTuple({elementSort, elementSort}, 
                                            {elements[i], elements[i]});
        solver.assertFormula(solver.mkTerm(SET_MEMBER, reflection, binaryRelation));
    }

    for (auto &elem1 : elements) {
        for (auto &elem2 : elements) {
            if (elem1 == elem2) {
                continue;
            } 

            Term firstTuple = solver.mkTuple({elementSort, elementSort}, {elem1, elem2});
            Term firstReversedTuple = solver.mkTuple({elementSort, elementSort}, {elem2, elem1});

            Term isMemberFirst = solver.mkTerm(SET_MEMBER, firstTuple, binaryRelation);
            Term isNotMemberFirstReverese = solver.mkTerm(NOT, solver.mkTerm(SET_MEMBER, firstReversedTuple, binaryRelation));
            
            solver.assertFormula(solver.mkTerm(IMPLIES, isMemberFirst, isNotMemberFirstReverese));

            for (auto &elem3 : elements) {
                Term secondTuple = solver.mkTuple({elementSort, elementSort}, {elem2, elem3});
                Term isMemberSecond = solver.mkTerm(SET_MEMBER, secondTuple, binaryRelation);
                
                Term thirdTuple = solver.mkTuple({elementSort, elementSort}, {elem1, elem3});
                Term isMemberThird = solver.mkTerm(SET_MEMBER, thirdTuple, binaryRelation);

                solver.assertFormula(solver.mkTerm(IMPLIES, solver.mkTerm(AND, isMemberFirst, isMemberSecond), isMemberThird));
            }
        }
    }

    for (auto &it : queries) {
        Term existsRelation = solver.mkTuple({elementSort, elementSort},
                                            {elements[it.first], elements[it.second]});
        Term existMember = solver.mkTerm(SET_MEMBER, existsRelation, binaryRelation);
        solver.assertFormula(existMember);
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
    for (auto &elem1 : elements) {
        for (auto &elem2 : elements) {
            if (elem1 == elem2) {
                continue;
            }
            
            Term relation = solver.mkTuple({elementSort, elementSort}, {elem1, elem2});
            Term relationReversed = solver.mkTuple({elementSort, elementSort}, {elem2, elem1});

            Term forbidRelation = solver.mkTerm(NOT, solver.mkTerm(SET_MEMBER, relation, binaryRelation));
            Term forbidRelationReversed = solver.mkTerm(NOT, solver.mkTerm(SET_MEMBER, relationReversed, binaryRelation));

            Term check = solver.mkTerm(AND, forbidRelation, forbidRelationReversed);
            if (solver.checkSatAssuming(check).isSat()) {
                return false;
            }
        }    
    }
    return true;
}

