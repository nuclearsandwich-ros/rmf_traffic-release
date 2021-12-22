/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include "ShapeInternal.hpp"

#include <rmf_traffic/geometry/Circle.hpp>

#ifdef RMF_TRAFFIC__USING_FCL_0_6
#include <fcl/geometry/shape/sphere.h>
#else
#include <fcl/shape/geometric_shapes.h>
#endif

namespace rmf_traffic {
namespace geometry {

//==============================================================================
class CircleInternal : public Shape::Internal
{
public:

  CircleInternal(double radius)
  : _radius(radius)
  {
    // Do nothing
  }

  CollisionGeometries make_fcl() const final
  {
    #ifdef RMF_TRAFFIC__USING_FCL_0_6
    return {std::make_shared<fcl::Sphered>(_radius)};
    #else
    return {std::make_shared<fcl::Sphere>(_radius)};
    #endif
  }

  double _radius;
};

//==============================================================================
Circle::Circle(double radius)
: ConvexShape(std::make_unique<CircleInternal>(radius))
{
  // Do nothing
}

//==============================================================================
Circle::Circle(const Circle& other)
: ConvexShape(std::make_unique<CircleInternal>(
      static_cast<const CircleInternal&>(*other._get_internal())))
{
  // Do nothing
}

//==============================================================================
Circle& Circle::operator=(const Circle& other)
{
  static_cast<CircleInternal&>(*_get_internal()) =
    static_cast<const CircleInternal&>(*other._get_internal());

  return *this;
}

//==============================================================================
void Circle::set_radius(double r)
{
  static_cast<CircleInternal*>(_get_internal())->_radius = r;
}

//==============================================================================
double Circle::get_radius() const
{
  return static_cast<const CircleInternal*>(_get_internal())->_radius;
}

//==============================================================================
FinalShape Circle::finalize() const
{
  return FinalShape::Implementation::make_final_shape(
    rmf_utils::make_derived_impl<const Shape, const Circle>(*this),
    _get_internal()->make_fcl(), this->get_radius(),
    make_equality_comparator(*this));
}

//==============================================================================
FinalConvexShape Circle::finalize_convex() const
{
  return FinalConvexShape::Implementation::make_final_shape(
    rmf_utils::make_derived_impl<const Shape, const Circle>(*this),
    _get_internal()->make_fcl(), this->get_radius(),
    make_equality_comparator(*this));
}

//==============================================================================
bool operator==(const Circle& lhs, const Circle& rhs)
{
  return lhs.get_radius() == rhs.get_radius();
}

//==============================================================================
bool operator!=(const Circle& lhs, const Circle& rhs)
{
  return !(lhs == rhs);
}

} // namespace geometry
} // namespace rmf_traffic
