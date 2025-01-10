//
//  tools.hpp
//  asteroids
//
//  Created by Alexandre Jacob on 04/11/2024.
//

#ifndef tools_hpp
#define tools_hpp

#include <stdio.h>
#include <cmath>
#include "ofMain.h"


float dist(ofVec2f a, ofVec2f b);

float angle(ofVec2f a);

long long getCurrentTimestamp();

ofVec3f crossProduct(ofVec3f a, ofVec3f b);


#endif /* tools_hpp */
