#ifndef DIRECTION_CHOOSE_FUNCTIONS_HPP_
#define DIRECTION_CHOOSE_FUNCTIONS_HPP_

#include "KNeighborsFinder.hpp"
#include "PredictionDefinitions.hpp"

namespace prediction
{

DeltaRecord getClosestNeighborDirectionWithMaxOccurence(
    const BasicKNeighborFinder::NeighborsVector& neighbors);
DeltaRecord getDirectionWithMaxOccurence(
    const BasicKNeighborFinder::NeighborsVector& neighbors);

}

#endif  // DIRECTION_CHOOSE_FUNCTIONS_HPP_

