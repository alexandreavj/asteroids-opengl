//
//  Particle.cpp
//  asteroids
//
//  Created by Alexandre Jacob on 23/11/2024.
//

#include "Particle.hpp"


Particle::Particle(ofVec3f pos, ofVec3f vel, GLfloat radius, GLfloat scale, Shape shape) : startPos(pos), pos(pos), vel(vel), rot(ofVec3f(0., 0., 0.)), radius(radius), scale(scale), shape(shape), alive(true) { }


void Particle::drawParticle() {
    glPushMatrix(); {
        glTranslatef(this->pos.x, this->pos.y, this->pos.z);
        glRotatef(this->rot.z, 0., 0., 1.);
        glRotatef(this->rot.y, 0., 1., 0.);
        glRotatef(this->rot.x, 1., 0., 0.);
        glScalef(this->scale, this->scale, this->scale);
        Particle::unitParticle(shape);
    } glPopMatrix();
}


void Particle::movement(long long elapsedTime) {
    if (alive) {
        this->rot += ofVec3f(0.5, 0.5, 1);
        this->pos += this->vel * (elapsedTime / 1000.);
        this->scale -= 0.01 * (120. / max(static_cast<double>(ofGetFrameRate()), 1.) );
        
        if (this->scale <= 0) {
            this->scale = 0;
            this->alive = false;
        }
        
        if ((this->startPos.distance(this->pos) / radius) > 0.95) this->alive = false;
    }
}


void Particle::unitParticle(Shape shape) {
    switch (shape) {
        case CUBE:
            unitCube();
            break;
            
        case SPHERE:
            unitSphere(10, 10);
            break;
    }
}
