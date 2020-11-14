#include <catch2/catch.hpp>
#include <iostream>

#include <visualizer/box.h>
#include <core/ball.h>

using idealgas::visualizer::Box;
using idealgas::Particle;

vector<double> some_color{0, 1, 0};

/**
 * In this test I will simulate the behavior of the update() method which is to update positions using Tick()
 * and then update velocities
 * by checking for collisions using the two collision checking methods.
 */
TEST_CASE("Try colliding two particles with one another") {
    Box test_box;
    test_box = Box(glm::vec2(0, 0), 30, 30);

    // spawn 2 particles heading towards one another
    // this is actually the scenario in the pdf file
    test_box.AddParticleToBox(Particle(vec2(19.9, 20), vec2(0.1, 0), 1, 1, some_color));
    test_box.AddParticleToBox(Particle(vec2(21.5, 21.4), vec2(-0.1, 0), 1, 1, some_color));
    REQUIRE(test_box.GetParticles().size() == 2);

    // add one random particle somewhere else to make sure its not being affected
    test_box.AddParticleToBox(Particle(vec2(1, 1), vec2(1, 0), 1, 1, some_color));

    test_box.ExecuteTimeStep();
    vector<Particle> box_after_one_tick = test_box.GetParticles();
    REQUIRE(box_after_one_tick[0].GetPosition() == vec2(20, 20));
    REQUIRE(box_after_one_tick[0].GetVelocity() == vec2(0.1, 0));
    REQUIRE(box_after_one_tick[1].GetPosition() == vec2(21.4, 21.4));
    REQUIRE(box_after_one_tick[1].GetVelocity() == vec2(-0.1, 0));
    REQUIRE(box_after_one_tick[2].GetPosition() == vec2(2, 1));
    REQUIRE(box_after_one_tick[2].GetVelocity() == vec2(1, 0));

    // they should be detected as intersecting with each other here
    test_box.ExecuteParticleCollisions();

    test_box.ExecuteParticleWallCollisions();

    test_box.ExecuteTimeStep();
    vector<Particle> box_after_two_tick = test_box.GetParticles();
    REQUIRE(box_after_two_tick[0].GetPosition() == vec2(20, 19.9));

    // for some reason I got -0.0 != 0.0 at first when trying this test
    // Did some research to realize that this is a thing- so I will just use approx here to show they are essentially equal
    // Just for certainty I did it for the others as well

    // the reason being that we go from double to float in CheckParticleCollisions and hence lose some precision
    REQUIRE(box_after_two_tick[0].GetVelocity().x == Approx(0.0).margin(1e-7));
    REQUIRE(box_after_two_tick[0].GetVelocity().y == Approx(-0.1).margin(1e-7));

    REQUIRE(box_after_two_tick[1].GetPosition() == vec2(21.4, 21.5));
    REQUIRE(box_after_two_tick[1].GetVelocity().x == Approx(0.0).margin(1e-7));
    REQUIRE(box_after_two_tick[1].GetVelocity().y == Approx(0.1).margin(1e-7));

    REQUIRE(box_after_two_tick[2].GetPosition() == vec2(3, 1));
    REQUIRE(box_after_two_tick[2].GetVelocity() == vec2(1, 0));

    // run one more tick
    test_box.ExecuteParticleCollisions();
    test_box.ExecuteParticleWallCollisions();

    test_box.ExecuteTimeStep();
    vector<Particle> box_after_three_tick = test_box.GetParticles();
    REQUIRE(box_after_three_tick[0].GetPosition() == vec2(20, 19.8));
    REQUIRE(box_after_three_tick[0].GetVelocity().x == Approx(0.0).margin(1e-7));
    REQUIRE(box_after_three_tick[0].GetVelocity().y == Approx(-0.1).margin(1e-7));

    REQUIRE(box_after_three_tick[1].GetPosition() == vec2(21.4, 21.6));
    REQUIRE(box_after_three_tick[1].GetVelocity().x == Approx(0.0).margin(1e-7));
    REQUIRE(box_after_three_tick[1].GetVelocity().y == Approx(0.1).margin(1e-7));
    REQUIRE(box_after_three_tick[2].GetPosition() == vec2(4, 1));
    REQUIRE(box_after_three_tick[2].GetVelocity() == vec2(1, 0));
}

TEST_CASE("Try bouncing a particle off a wall (left)") {
    Box test_box;
    test_box = Box(glm::vec2(0, 0), 5, 5);

    test_box.AddParticleToBox(Particle(vec2(2, 2), vec2(-1, 0), 1, 1, some_color));


    test_box.ExecuteTimeStep();
    vector<Particle> box_after_one_tick = test_box.GetParticles();
    REQUIRE(box_after_one_tick[0].GetPosition() == vec2(1, 2));
    REQUIRE(box_after_one_tick[0].GetVelocity() == vec2(-1, 0));

    test_box.ExecuteParticleCollisions();
    // should be detected by below method as intersecting with left wall
    test_box.ExecuteParticleWallCollisions();

    test_box.ExecuteTimeStep();
    vector<Particle> box_after_two_tick = test_box.GetParticles();

    // direction should be flipped and it should have moved right one
    REQUIRE(box_after_two_tick[0].GetPosition() == vec2(2, 2));
    REQUIRE(box_after_two_tick[0].GetVelocity() == vec2(1, 0));
}

TEST_CASE("Try bouncing a particle off a wall (top)") {
    Box test_box;
    test_box = Box(glm::vec2(0, 0), 5, 5);

    test_box.AddParticleToBox(Particle(vec2(2, 2), vec2(0, -1), 1, 1, some_color));

    test_box.ExecuteTimeStep();
    vector<Particle> box_after_one_tick = test_box.GetParticles();
    REQUIRE(box_after_one_tick[0].GetPosition() == vec2(2, 1));
    REQUIRE(box_after_one_tick[0].GetVelocity() == vec2(0, -1));

    test_box.ExecuteParticleCollisions();
    // should be detected by below method as intersecting with top wall
    test_box.ExecuteParticleWallCollisions();

    test_box.ExecuteTimeStep();
    vector<Particle> box_after_two_tick = test_box.GetParticles();

    REQUIRE(box_after_two_tick[0].GetPosition() == vec2(2, 2));
    REQUIRE(box_after_two_tick[0].GetVelocity() == vec2(0, 1));
}

TEST_CASE("Try bouncing off bottom wall") {
    Box test_box;
    test_box = Box(glm::vec2(0, 0), 5, 5);

    test_box.AddParticleToBox(Particle(vec2(1, 3.5), vec2(0, 1), 1, 1, some_color));

    test_box.ExecuteTimeStep();
    vector<Particle> box_after_one_tick = test_box.GetParticles();
    REQUIRE(box_after_one_tick[0].GetPosition() == vec2(1, 4.5));
    REQUIRE(box_after_one_tick[0].GetVelocity() == vec2(0, 1));

    test_box.ExecuteParticleCollisions();
    // should be detected by below method as intersecting with bottom wall
    test_box.ExecuteParticleWallCollisions();

    test_box.ExecuteTimeStep();
    vector<Particle> box_after_two_tick = test_box.GetParticles();

    REQUIRE(box_after_two_tick[0].GetPosition() == vec2(1, 3.5));
    REQUIRE(box_after_two_tick[0].GetVelocity() == vec2(0, -1));
}

TEST_CASE("Testing Double Generator") {
    Box test_box;
    for (size_t i = 0; i < 100; i++) {
        REQUIRE(test_box.GenerateRandomDouble(10) <= 10);
        REQUIRE(test_box.GenerateRandomDouble(10) >= -10);
    }
}

TEST_CASE("Check that the particles generated using SpawnParticles meet certain properties") {
    Box test_box;
    test_box = Box(glm::vec2(0, 0), 100, 100);
    test_box.SpawnParticles(10, 10, 1, some_color, 15, 1);

    SECTION("They should all have velocity less than 15") {
        for (auto x : test_box.GetParticles()) {
            REQUIRE(length(x.GetVelocity()) < 15);
        }
        // check that 10 particles are actually spawned
        REQUIRE(test_box.GetParticles().size() == 10);
    }
    SECTION("They should spawn in a rectangular format with gaps = 1") {
        vector<Particle> particles = test_box.GetParticles();
        REQUIRE(particles[0].GetPosition() == vec2(11, 11));
        REQUIRE(particles[1].GetPosition() == vec2(32, 11));
        REQUIRE(particles[2].GetPosition() == vec2(53, 11));
        REQUIRE(particles[3].GetPosition() == vec2(74, 11));
        REQUIRE(particles[4].GetPosition() == vec2(11, 32));
        REQUIRE(particles[5].GetPosition() == vec2(32, 32));
        REQUIRE(particles[6].GetPosition() == vec2(53, 32));
        REQUIRE(particles[7].GetPosition() == vec2(74, 32));
        REQUIRE(particles[8].GetPosition() == vec2(11, 53));
        REQUIRE(particles[9].GetPosition() == vec2(32, 53));
    }

    // now the value of y_bound should be 63 (because 53 + 10)
    REQUIRE(test_box.GetYBound() == 63);

    // try spawning new set of particles in under first set
    test_box.SpawnParticles(5, 5, 1, some_color, 15, 1);
    SECTION("They should spawn below the first set of particles in these spots") {
        vector<Particle> particles = test_box.GetParticles();
        REQUIRE(particles[10].GetPosition() == vec2(6, 69));
        REQUIRE(particles[11].GetPosition() == vec2(17, 69));
        REQUIRE(particles[12].GetPosition() == vec2(28, 69));
        REQUIRE(particles[13].GetPosition() == vec2(39, 69));
        REQUIRE(particles[14].GetPosition() == vec2(50, 69));
    }
}

// invalid stuff
TEST_CASE("Trying to create a box with negative values") {
    Box test_box;
    REQUIRE_THROWS_AS(test_box = Box(glm::vec2(-32, -12), 100, 100), std::invalid_argument);
    REQUIRE_THROWS_AS(test_box = Box(glm::vec2(0, 0), -12, 100), std::invalid_argument);
    REQUIRE_THROWS_AS(test_box = Box(glm::vec2(0, 0), 12, -100), std::invalid_argument);
}

TEST_CASE("Trying to spawn too many particles should throw an exception if we don't have enough room") {
    Box test_box;
    test_box = Box(glm::vec2(0, 0), 100, 100);
    vector<double> some_color{0,0,1};
    // the statement below should be impossible since there's no way to spawn 2000 particles in this space
    REQUIRE_THROWS_AS(test_box.SpawnParticles(2000, 10, 15, some_color, 10, 1), std::invalid_argument);
}

TEST_CASE("Trying to call SpawnParticles with incorrect parameter values") {
    Box test_box;
    test_box = Box(glm::vec2(0, 0), 100, 100);
    vector<double> some_color{0,0,1};

    // can't spawn particles with zero radius
    REQUIRE_THROWS_AS(test_box.SpawnParticles(1, 0, 3, some_color, 15, 1), std::invalid_argument);

    // can't spawn particles with negative starting velocity
    REQUIRE_THROWS_AS(test_box.SpawnParticles(1, 10, 3, some_color, -1.4, 1), std::invalid_argument);

    // can't spawn particles that have negative space apart from one another
    REQUIRE_THROWS_AS(test_box.SpawnParticles(1, 10, 3, some_color, 15, -0.3), std::invalid_argument);

}

TEST_CASE("Trying to read from invalid JSON files") {
    // idk how to test this in catch2 but if you change path to JSON file in ideal_gas_app.cc to "../../../data/wrongmargins.json"
    // it will throw exception which is correct since window is too small comapred to margin + height/length
    // so that's one way I know this error will be caught

    // do wrongmasses.json to see that it catches files with same mass error as well
}

