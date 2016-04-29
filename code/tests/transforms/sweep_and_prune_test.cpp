// Sweep and Prune test
#include <catch/catch.hpp>
#include <systems/physics_engine/broadphase/sweep.hpp>
#include <systems/physics_engine/broadphase/prune.hpp>
#include <math/math.hpp>
#include <core/entity_id.hpp>


TEST_CASE("Sweep and Prune removes overlapping aabbs")
{
  constexpr uint32_t number_of_boundings = 6;

  Physics::Broadphase::Sweep sweep;
  Physics::Broadphase::sweep_init(&sweep, number_of_boundings);

  REQUIRE(Physics::Broadphase::sweep_get_size(&sweep) == number_of_boundings);

  // Some tests data 
  Core::Entity_id ids[] = {Core::Entity_id{1,1},
                           Core::Entity_id{2,2},
                           Core::Entity_id{3,3},
                           Core::Entity_id{4,4},
                           Core::Entity_id{5,5},
                           Core::Entity_id{6,6}}; 

  SECTION("Sweep should pickup all entites")
  {
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


  SECTION("All should be pruned")
  {
    const math::aabb bounding[number_of_boundings] = {
      math::aabb_init(math::vec3_init(-10,-10,-10), math::vec3_init(-9,-9,-9)),
      math::aabb_init(math::vec3_init(-20,-20,-20), math::vec3_init(-19,-19,-19)),
      math::aabb_init(math::vec3_init(-30,-30,-30), math::vec3_init(-29,-29,-29)),
      math::aabb_init(math::vec3_init(-40,-40,-40), math::vec3_init(-39,-39,-39)),
      math::aabb_init(math::vec3_init(-50,-50,-50), math::vec3_init(-49,-49,-49)),
      math::aabb_init(math::vec3_init(-60,-60,-60), math::vec3_init(-59,-59,-59)),
    };
    
    Physics::Broadphase::sweep_calculate(&sweep,
                                         bounding, 
                                         number_of_boundings);

    Physics::Broadphase::Prune prune;
    Physics::Broadphase::prune_init(&prune, number_of_boundings);

    Physics::Broadphase::prune_calculate(&prune, &sweep);
    
    REQUIRE(prune.size == number_of_boundings);
  }


  SECTION("Prune should pull some entities")
  {
  }


  SECTION("No pruning should happen entities")
  {
    const math::aabb bounding[number_of_boundings] = {
      math::aabb_init(math::vec3_init(-10,-10,-10), math::vec3_init(-8,-8,-8)),
      math::aabb_init(math::vec3_init(-11,-11,-11), math::vec3_init(-9,-9,-9)),
      math::aabb_init(math::vec3_init(-12,-12,-12), math::vec3_init(-10,-10,-10)),
      math::aabb_init(math::vec3_init(-13,-13,-13), math::vec3_init(-11,-11,-11)),
      math::aabb_init(math::vec3_init(-14,-14,-14), math::vec3_init(-12,-12,-12)),
      math::aabb_init(math::vec3_init(-15,-15,-15), math::vec3_init(-13,-13,-13)),
    };
    
    Physics::Broadphase::sweep_calculate(&sweep,
                                         bounding, 
                                         number_of_boundings);

    Physics::Broadphase::Prune prune;
    Physics::Broadphase::prune_init(&prune, number_of_boundings);

    Physics::Broadphase::prune_calculate(&prune, &sweep);
    
    REQUIRE(prune.size == 0);
  }


}
