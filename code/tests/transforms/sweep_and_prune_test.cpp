// Sweep and Prune test
#include <catch/catch.hpp>
#include <systems/physics_engine/broadphase/sweep.hpp>
#include <math/math.hpp>
#include <core/entity_id.hpp>


TEST_CASE("Sweep and Prune removes overlapping aabbs")
{

  Physics::Broadphase::Sweep sweep;
  Physics::Broadphase::sweep_init(&sweep, 6);

  REQUIRE(Physics::Broadphase::sweep_get_size(&sweep) == 6);

  // Some tests data 
  Core::Entity_id ids[] = {Core::Entity_id{1,1},
                           Core::Entity_id{2,2},
                           Core::Entity_id{3,3},
                           Core::Entity_id{4,4},
                           Core::Entity_id{5,5},
                           Core::Entity_id{6,6}}; 

  SECTION("Sweep should pickup all entites")
  {
    constexpr uint32_t number_of_boundings = 6;

    const math::aabb bounding[number_of_boundings] = {
      math::aabb_init(math::vec3_init(-0,-0,-0), math::vec3_init(+0,+0,+0)),
      math::aabb_init(math::vec3_init(-1,-1,-1), math::vec3_init(+0,+0,+0)),
      math::aabb_init(math::vec3_init(-2,-2,-2), math::vec3_init(+0,+0,+0)),
      math::aabb_init(math::vec3_init(-3,-3,-3), math::vec3_init(+0,+0,+0)),
      math::aabb_init(math::vec3_init(-4,-4,-4), math::vec3_init(+0,+0,+0)),
      math::aabb_init(math::vec3_init(-5,-5,-5), math::vec3_init(+0,+0,+0)),
    };
    
    Physics::Broadphase::sweep_calculate(&sweep,
                                         bounding, 
                                         number_of_boundings);
   
    const Physics::Broadphase::Sweep_axis *axis[3] = {sweep.x_axis, sweep.y_axis, sweep.z_axis};

    for(uint32_t i = 0; i < 3; ++i)
    {
      const Physics::Broadphase::Sweep_axis *curr_axis = axis[i];

      for(uint32_t j = 0; j < number_of_boundings; ++j)
      {
        REQUIRE(curr_axis[j].center == (j * -0.5f));
        REQUIRE(curr_axis[j].half_extent == (j * -0.5f));
      }
    }
  }


  SECTION("Prune should prune no entities")
  {
  }


  SECTION("Prune should pull some entities")
  {
  }


  SECTION("Prune should remove all entities")
  {
  }


}
