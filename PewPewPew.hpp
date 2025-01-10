//
//  PewPewPew.hpp
//  asteroids
//
//  Created by Alexandre Jacob on 23/11/2024.
//

#ifndef PewPewPew_hpp
#define PewPewPew_hpp

#include <stdio.h>
#include "elements.hpp"
#include "ofMain.h"
#include "ICollidable.hpp"


#define DEACCEL 25


class PewPewPew : public ICollidable<PewPewPew> {
public:
    PewPewPew(ofVec3f pos, GLfloat velMag, ofVec3f rot, GLfloat scale);
    void drawPewPewPew();
    void movement(long long timePeriod);
    void updateCollision(long long timePeriod) override;
    void drawCollision() override;
    static void unitPewPewPew();
    GLfloat ang;
    
};


#endif /* PewPewPew_hpp */
