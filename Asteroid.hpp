//
//  asteroid.hpp
//  asteroids
//
//  Created by Alexandre Jacob on 04/11/2024.
//

#ifndef Asteroid_hpp
#define Asteroid_hpp

#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <cstdlib>
#include <ctime>
#include "ofMain.h"
#include "tools.hpp"
#include "Field.hpp"
#include "Particle.hpp"
#include "ICollidable.hpp"

#define INNER_BORDER 0.375
#define POLES_SCALE 0.5

#define TEXTURE_ROWS 3
#define TEXTURE_COLS 3


class Asteroid : public ICollidable<Asteroid> {
public:
    Asteroid(int nbrVertices, GLfloat scale);
    ~Asteroid();
    void drawAsteroid();
    void movement(long long timePeriod);
    void updateCollision(long long timePeriod) override;
    void drawCollision() override;
    void unitAsteroid(vector<ofVec2f> equatorVertices, bool outline);
    
    void setBackupScale(GLfloat backupScale);
    void setDivide(bool divide);
    
    bool getDivide();
    
private:
    bool divide;
    vector<ofVec2f> equatorVertices;
    ofImage TEXTURE;
};


#endif /* asteroid_hpp */
