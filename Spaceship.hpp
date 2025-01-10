//
//  Spaceship.hpp
//  asteroids
//
//  Created by Alexandre Jacob on 09/11/2024.
//

#ifndef Spaceship_hpp
#define Spaceship_hpp

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include "tools.hpp"
#include "ofMain.h"
#include "Particle.hpp"
#include "ICollidable.hpp"


/*
enum SpaceshipPart {
    MAIN,
    BACK_COVER,
    FIRE,
    WINGS
};
 */


enum SpaceshipCommand {
    DRIFT_2D,
    DRIFT_Z,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SHOOT
};


#define ACC_INTENSITY 150
#define DEACC_FACTOR 1.125
#define ROT_SPEED 150
#define MAX_SPEED 250
#define N_PARTICLES 200


class Spaceship : public ICollidable<Spaceship> {
public:
    Spaceship(GLfloat scale);
    void drawSpaceship();
    void movement(long long timePeriod);
    void updateCollision(long long timePeriod) override;
    void drawCollision() override;
    static void unitSpaceship();
    SpaceshipCommand command = DRIFT_2D;

private:
    // static map<SpaceshipPart, vector<ofVec3f>> unitVertexes;
};


#endif /* Spaceship_hpp */
