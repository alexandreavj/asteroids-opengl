//
//  PewPewPew.cpp
//  asteroids
//
//  Created by Alexandre Jacob on 23/11/2024.
//

#include "PewPewPew.hpp"
#include "materials.hpp"


PewPewPew::PewPewPew(ofVec3f pos, GLfloat velMag, ofVec3f rot, GLfloat scale) {
    this->pos = pos;
    this->rot.set(0., 0., rot.z * (PI / 180.));
    this->scale = scale;
    this->vel = ofVec3f(cos(this->rot.z), sin(this->rot.z)).getNormalized() * velMag;
}


void PewPewPew::drawPewPewPew() {
    loadMaterial(6);
    
    glPushMatrix(); {
        glTranslatef(this->pos.x, this->pos.y, this->pos.z);
        glScalef(this->scale, this->scale, this->scale);
        PewPewPew::unitPewPewPew();
    } glPopMatrix();
}


void PewPewPew::movement(long long timePeriod) {
    this->updateCollision(timePeriod);
    
    if (!isInCollision) {
        ofVec2f lastVel = this->vel;
        
        this->pos.x = this->pos.x + this->vel.x * (timePeriod / 1000.) - DEACCEL * pow((timePeriod / 1000.), 2) * cos(this->rot.z);
        this->pos.y = this->pos.y + this->vel.y * (timePeriod / 1000.) - DEACCEL * pow((timePeriod / 1000.), 2) * sin(this->rot.z);
        
        this->vel.x = this->vel.x - DEACCEL * (timePeriod / 1000.) * cos(this->rot.z);
        this->vel.y = this->vel.y - DEACCEL * (timePeriod / 1000.) * sin(this->rot.z);
        
        if (lastVel.x * this->vel.x <= 0.) this->vel.x = 0.;
        if (lastVel.y * this->vel.y <= 0.) this->vel.y = 0.;
    }
}


void PewPewPew::updateCollision(long long timePeriod) {
    if (isInCollision) {
        this->scale -= 1 * (120. / max(static_cast<double>(ofGetFrameRate()), 1.) );
        
        if (this->scale <= 0) this->scale = 0;
    }
}


void PewPewPew::drawCollision() { ; }


void PewPewPew::unitPewPewPew() {
    unitSphere(10, 10);
}
