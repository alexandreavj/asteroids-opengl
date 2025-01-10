//
//  Particle.hpp
//  asteroids
//
//  Created by Alexandre Jacob on 23/11/2024.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "ofMain.h"
#include "elements.hpp"


enum Shape {
    CUBE,
    SPHERE
};


class Particle {
public:
    Particle(ofVec3f pos, ofVec3f vel, GLfloat radius, GLfloat scale, Shape shape);
    void drawParticle();
    void movement(long long elapsedTime);
    static void unitParticle(Shape shape);
    bool alive;
    
private:
    ofVec3f startPos;
    ofVec3f pos;
    ofVec3f vel;
    ofVec3f rot;
    GLfloat radius;
    GLfloat scale;
    Shape shape;
};


#endif /* Particle_hpp */
