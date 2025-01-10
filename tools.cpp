//
//  tools.cpp
//  asteroids
//
//  Created by Alexandre Jacob on 04/11/2024.
//

#include "tools.hpp"
#include "ofApp.h"


float dist(ofVec2f a, ofVec2f b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

float angle(ofVec2f a) {
    float ang = atan(a.y / a.x) * (180. / PI);

    if (a.x >= 0 and a.y >= 0) {
        return ang;
    } else if (a.x < 0) {
        return ang + 180;
    } else {
        return 360 + ang;
    }
}

long long getCurrentTimestamp() {
    return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();
}

ofVec3f crossProduct(ofVec3f a, ofVec3f b) {
    ofVec3f aux;
    aux.x = a.y * b.z - a.z * b.y;
    aux.y = a.z * b.x - a.x * b.z;
    aux.z = a.x * b.y - a.y * b.x;

    return aux;
}
