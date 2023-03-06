#pragma once

// STL
#include <array>
// My includes
#include "Vector2.h"
#include "util.h"

/**
 * \brief Namespace of MyGAL
 */
namespace mygal
{

template<typename T>
class Diagram;

template<typename T>
class FortuneAlgorithm;

/**
 * \brief Class representing a box
 *
 * Be careful, the y-axis is oriented to the top like in math.
 * This `bottom` must be lower to `top`.
 *
 * \author Pierre Vigier
 */
template<typename T>
class CustomShape
{
    public:
        CustomShape(Vector2 *vertices){
            this->vertices = vertices;
        }
        bool contains(const Vector2<T>& point) const{
            return almostBetween(point.x, left, right) && almostBetween(point.y, bottom, top);
        }
    
    private:
        Vector2 *vertices;
};
}