//
//  Spaceship.cpp
//  asteroids
//
//  Created by Alexandre Jacob on 09/11/2024.
//

#include "Spaceship.hpp"
#include "materials.hpp"


Spaceship::Spaceship(GLfloat scale) {
    this->scale = scale;
    this->pos.set(0., 0., 0.);
    this->rot.set(0., 0., 0.);
    this->vel.set(0., 0., 0.);
    this->isInCollision = false;
    this->backupScale = scale;
}


void Spaceship::drawSpaceship() {
    loadMaterial(11);
    
    glPushMatrix(); {
        glTranslatef(this->pos.x, this->pos.y, this->pos.z);
        glRotatef(this->rot.z, 0., 0., 1.);
        glScalef(this->scale, this->scale, this->scale);
        Spaceship::unitSpaceship();
    } glPopMatrix();
    
    this->drawCollision();
}


void Spaceship::movement(long long timePeriod) {
    if (!this->isInCollision) {
        ofVec3f lastVel = this->vel;
        
        switch (this->command) {
            case FORWARD:
                this->pos.x = this->pos.x + this->vel.x * (timePeriod / 1000.) + ACC_INTENSITY * cos(this->rot.z * PI / 180.) * pow((timePeriod / 1000.), 2);
                this->pos.y = this->pos.y + this->vel.y * (timePeriod / 1000.) + ACC_INTENSITY * sin(this->rot.z * PI / 180.) * pow((timePeriod / 1000.), 2);
                
                this->vel.x = this->vel.x + ACC_INTENSITY * cos(this->rot.z * PI / 180.) * (timePeriod / 1000.);
                this->vel.y = this->vel.y + ACC_INTENSITY * sin(this->rot.z * PI / 180.) * (timePeriod / 1000.);
                
                break;
                
                
            case BACKWARD:
                this->pos.x = this->pos.x + this->vel.x * (timePeriod / 1000.) - ACC_INTENSITY * cos(this->rot.z * PI / 180.) * pow((timePeriod / 1000.), 2);
                this->pos.y = this->pos.y + this->vel.y * (timePeriod / 1000.) - ACC_INTENSITY * sin(this->rot.z * PI / 180.) * pow((timePeriod / 1000.), 2);
                
                this->vel.x = this->vel.x - ACC_INTENSITY * cos(this->rot.z * PI / 180.) * (timePeriod / 1000.);
                this->vel.y = this->vel.y - ACC_INTENSITY * sin(this->rot.z * PI / 180.) * (timePeriod / 1000.);
                
                break;
                
                
            case RIGHT:
                this->rot.z -= ROT_SPEED * (timePeriod / 1000.);
                break;
                
                
            case LEFT:
                this->rot.z += ROT_SPEED * (timePeriod / 1000.);
                break;
                
                
            case UP:
                this->pos.z = this->pos.z + this->vel.z * (timePeriod / 1000.) + ACC_INTENSITY * 5 * pow((timePeriod / 1000.), 2);
                
                this->vel.z = this->vel.z + ACC_INTENSITY * 5* (timePeriod / 1000.);
                break;
                
                
            case DOWN:
                this->pos.z = this->pos.z + this->vel.z * (timePeriod / 1000.) - ACC_INTENSITY * 5 * pow((timePeriod / 1000.), 2);
                
                this->vel.z = this->vel.z - ACC_INTENSITY * 5 * (timePeriod / 1000.);
                break;
                
                
            case DRIFT_2D:
                if (this->vel.x != 0) {
                    this->pos.x = this->pos.x + this->vel.x * (timePeriod / 1000.) - ACC_INTENSITY * cos(angle(lastVel)) * DEACC_FACTOR * pow((timePeriod / 1000.), 2);
                    
                    this->vel.x = this->vel.x - ACC_INTENSITY * cos(angle(lastVel)) * DEACC_FACTOR * (timePeriod / 1000.);
                }
                
                if (this->vel.y != 0) {
                    this->pos.y = this->pos.y + this->vel.y * (timePeriod / 1000.) - ACC_INTENSITY * sin(angle(lastVel)) * DEACC_FACTOR * 2 * pow((timePeriod / 1000.), 2);
                    
                    this->vel.y = this->vel.y - ACC_INTENSITY * sin(angle(lastVel)) * DEACC_FACTOR * 2 * (timePeriod / 1000.);
                }
                
                if ((this->vel.x * lastVel.x) < 0) {
                    this->vel.x = 0.;
                }
                if ((this->vel.y * lastVel.y) < 0) {
                    this->vel.y = 0.;
                }
                
                break;
                
                
            case DRIFT_Z:
                if (this->vel.z != 0) {
                    this->pos.z = this->pos.z + this->vel.z * (timePeriod / 1000.) - ACC_INTENSITY * ((this->vel.z > 0) - (this->vel.z < 0)) * DEACC_FACTOR * 5 * pow((timePeriod / 1000.), 2);
                    
                    this->vel.z = this->vel.z - ACC_INTENSITY * ((this->vel.z > 0) - (this->vel.z < 0)) * DEACC_FACTOR * 5 * (timePeriod / 1000.);
                }
                
                if ((this->vel.z * lastVel.z) < 0) {
                    this->vel.z = 0.;
                }
                
                break;
                
                
            default:
                break;
        }
        
        if (sqrt(pow(this->vel.x, 2) + pow(this->vel.y, 2) /*+ pow(this->vel.z, 2)*/) > MAX_SPEED) {
            this->vel = this->vel.getNormalized() * MAX_SPEED;
        }
    }
    
    this->updateCollision(timePeriod);
}


void Spaceship::updateCollision(long long timePeriod) {
    if (this->isInCollision) {
        if (particles.size() == 0) {
            for (int i = 0; i < N_PARTICLES; i++) {
                particles.push_back(Particle(this->pos, ofVec3f(ofRandom(-1., 1.), ofRandom(-1, 1.), ofRandom(-1, 1.)).getNormalized() * ofRandom(25., 50.), ofRandom(75., 125.), 5, CUBE));
            }
        } else {
            bool allDead = true;
            for (int i = 0; i < N_PARTICLES; i++) {
                particles[i].movement(timePeriod);
                if (particles[i].alive) allDead = false;
            }
            
            if (allDead) {
                this->isInCollision = false;
                particles.clear();
                this->pos.set(0., 0., 0.);
                this->vel.set(0., 0., 0.);
                this->rot.z = 0.;
                this->scale = this->backupScale;
            } else {
                this->scale -= 0.75 * (120. / max(static_cast<double>(ofGetFrameRate()), 1.) );
                
                if (this->scale < 0) this->scale = 0;
            }
        }
    }
}


void Spaceship::drawCollision() {
    if (this->isInCollision) {
        for (int i = 0; i < particles.size(); i++) {
            particles[i].drawParticle();
        }
    }
}


void Spaceship::unitSpaceship() {
    // normal as the normalized cross porduct of two vector from the polygon
    ofVec3f a, b, normal;
    glBegin(GL_TRIANGLES); {
        loadMaterial(15);
        
        // FIRE
        a = ofVec3f(-0.35, 0., 0.2) - ofVec3f(-0.5, 0., 0.);
        b = ofVec3f(-0.35, 0.2, 0.) - ofVec3f(-0.5, 0., 0.);
        normal = crossProduct(a, b).getNormalized();
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(-0.35, 0.2, 0.);
        loadMaterial(17);
        glVertex3f(-0.5, 0., 0.);
        loadMaterial(15);
        glVertex3f(-0.35, 0., 0.2);
        
        normal = ofVec3f(normal.x, -normal.y, normal.z).getNormalized();
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(-0.35, 0., 0.2);
        loadMaterial(17);
        glVertex3f(-0.5, 0., 0.);
        loadMaterial(15);
        glVertex3f(-0.35, -0.2, 0.);
        
        normal = -normal.getNormalized();
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(-0.35, 0.2, 0.);
        loadMaterial(17);
        glVertex3f(-0.5, 0., 0.);
        loadMaterial(15);
        glVertex3f(-0.35, 0., -0.2);
        
        normal = ofVec3f(normal.x, -normal.y, normal.z).getNormalized();
        glNormal3f(normal.x, normal.y, normal.z);
        loadMaterial(17);
        glVertex3f(-0.5, 0., 0.);
        loadMaterial(15);
        glVertex3f(-0.35, 0., -0.2);
        glVertex3f(-0.35, -0.2, 0.);
        
        loadMaterial(11);
        
        // MAIN
        a = ofVec3f(-0.35, 0.35, 0.) - ofVec3f(0.5, 0., 0.);
        b = ofVec3f(-0.35, 0., 0.35) - ofVec3f(0.5, 0., 0.);
        normal = crossProduct(a, b).getNormalized();
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(0.5, 0., 0.);
        glVertex3f(-0.35, 0.35, 0.);
        glVertex3f(-0.35, 0., 0.35);
        
        normal = ofVec3f(normal.x, -normal.y, normal.z).getNormalized();
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(0.5, 0., 0.);
        glVertex3f(-0.35, 0., 0.35);
        glVertex3f(-0.35, -0.35, 0.0);
        
        normal = -normal.getNormalized();
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(-0.35, 0.35, 0.);
        glVertex3f(0.5, 0., 0.);
        glVertex3f(-0.35, 0., -0.35);
        
        normal = ofVec3f(normal.x, -normal.y, normal.z).getNormalized();
        glNormal3f(normal.x, normal.y, normal.z);
        glVertex3f(-0.35, 0., -0.35);
        glVertex3f(0.5, 0., 0.);
        glVertex3f(-0.35, -0.35, 0.);
        
        // WINGS
        // draw same wing twice so each wing has "two fronts"
        glNormal3f(0., 0., 1.);
        glVertex3f(0., 0.5, 0.);
        glVertex3f(0.3, 0.07 / 0.85, 0.);
        glVertex3f(-0.2, 0.245 / 0.85, 0.);
        
        glNormal3f(0., 0., -1.);
        glVertex3f(0., 0.5, 0.);
        glVertex3f(-0.2, 0.245 / 0.85, 0.);
        glVertex3f(0.3, 0.07 / 0.85, 0.);
        
        glNormal3f(0., -1., 0.);
        glVertex3f(-0.2, 0., -(0.245 / 0.85));
        glVertex3f(0.3, 0., -(0.07 / 0.85));
        glVertex3f(0., 0., -0.5);
        
        glNormal3f(0., 1., 0.);
        glVertex3f(-0.2, 0., -(0.245 / 0.85));
        glVertex3f(0., 0., -0.5);
        glVertex3f(0.3, 0., -(0.07 / 0.85));
        
        glNormal3f(0., 0., 1.);
        glVertex3f(-0.2, -(0.245 / 0.85), 0.);
        glVertex3f(0.3, -(0.07 / 0.85), 0.);
        glVertex3f(0., -0.5, 0.);
        
        glNormal3f(0., 0., -1.);
        glVertex3f(-0.2, -(0.245 / 0.85), 0.);
        glVertex3f(0., -0.5, 0.);
        glVertex3f(0.3, -(0.07 / 0.85), 0.);
        
        glNormal3f(0., -1., 0.);
        glVertex3f(-0.2, 0., 0.245 / 0.85);
        glVertex3f(0., 0., 0.5);
        glVertex3f(0.3, 0., 0.07 / 0.85);
        
        glNormal3f(0., 1., 0.);
        glVertex3f(0., 0., 0.5);
        glVertex3f(-0.2, 0., 0.245 / 0.85);
        glVertex3f(0.3, 0., 0.07 / 0.85);
    } glEnd();
    
    
    glBegin(GL_QUADS); {
        // BACK COVER
        glNormal3f(-1., 0., 0.);
        glVertex3f(-0.35, 0.35, 0.);
        glVertex3f(-0.35, 0., -0.35);
        glVertex3f(-0.35, -0.35, 0.);
        glVertex3f(-0.35, 0., 0.35);
    } glEnd();
}


/*
map<SpaceshipPart, vector<ofVec3f>> Spaceship::unitVertexes = {
    {MAIN,
        { ofVec3f(0.5, 0., 0.),
            ofVec3f(-0.35, 0.35, 0.),
            ofVec3f(-0.35, 0., 0.35),
            ofVec3f(0.5, 0., 0.),
            ofVec3f(-0.35, 0., 0.35),
            ofVec3f(-0.35, -0.35, 0.0),
            ofVec3f(-0.35, 0.35, 0.),
            ofVec3f(0.5, 0., 0.),
            ofVec3f(-0.35, 0., -0.35),
            ofVec3f(-0.35, 0., -0.35),
            ofVec3f(0.5, 0., 0.),
            ofVec3f(-0.35, -0.35, 0.)}
    },
            
    {BACK_COVER,
        {ofVec3f(-0.35, 0.35, 0.),
            ofVec3f(-0.35, 0., -0.35),
            ofVec3f(-0.35, -0.35, 0.),
            ofVec3f(-0.35, 0., 0.35)}
    },
    
    {FIRE,
        {ofVec3f(-0.35, 0.2, 0.),
            ofVec3f(-0.5, 0., 0.),
            ofVec3f(-0.35, 0., 0.2),
            ofVec3f(-0.35, 0., 0.2),
            ofVec3f(-0.5, 0., 0.),
            ofVec3f(-0.35, -0.2, 0.),
            ofVec3f(-0.35, 0.2, 0.),
            ofVec3f(-0.5, 0., 0.),
            ofVec3f(-0.35, 0., -0.2),
            ofVec3f(-0.5, 0., 0.),
            ofVec3f(-0.35, 0., -0.2),
            ofVec3f(-0.35, -0.2, 0.)}
    },
    
    {WINGS,
        // draw twice since they are 2D (two front faces)
        { ofVec3f(0., 0.5, 0.),
            ofVec3f(0.3, 0.07 / 0.85, 0.),
            ofVec3f(-0.2, 0.245 / 0.85, 0.),
            ofVec3f(0., 0.5, 0.),
            ofVec3f(-0.2, 0.245 / 0.85, 0.),
            ofVec3f(0.3, 0.07 / 0.85, 0.),
            ofVec3f(-0.2, 0., -(0.245 / 0.85)),
            ofVec3f(0.3, 0., -(0.07 / 0.85)),
            ofVec3f(0., 0., -0.5),
            ofVec3f(-0.2, 0., -(0.245 / 0.85)),
            ofVec3f(0., 0., -0.5),
            ofVec3f(0.3, 0., -(0.07 / 0.85)),
            ofVec3f(-0.2, -(0.245 / 0.85), 0.),
            ofVec3f(0.3, -(0.07 / 0.85), 0.),
            ofVec3f(0., -0.5, 0.),
            ofVec3f(-0.2, -(0.245 / 0.85), 0.),
            ofVec3f(0., -0.5, 0.),
            ofVec3f(0.3, -(0.07 / 0.85), 0.),
            ofVec3f(-0.2, 0., 0.245 / 0.85),
            ofVec3f(0., 0., 0.5),
            ofVec3f(0.3, 0., 0.07 / 0.85),
            ofVec3f(0., 0., 0.5),
            ofVec3f(-0.2, 0., 0.245 / 0.85),
            ofVec3f(0.3, 0., 0.07 / 0.85)
        }
    }
};
*/


/*
 Spaceship model: https://www.geogebra.org/calculator/kud6rdch
 C: ofVec3f(0.5, 0., 0.)
 E: ofVec3f(-0.5, 0., 0.)
 F: ofVec3f(-0.35, 0., -0.35)
 G: ofVec3f(-0.35, 0., 0.35)
 H: ofVec3f(-0.35, 0., -0.2)
 I: ofVec3f(-0.35, 0., 0.2)
 J: ofVec3f(0., 0., 0.5)
 K: ofVec3f(0.3, 0., 0.07 / 0.85)
 L: ofVec3f(-0.2, 0., 0.245 / 0.85)
 M: ofVec3f(0., 0., -0.5)
 N: ofVec3f(0.3, 0., -(0.07 / 0.85))
 O: ofVec3f(-0.2, 0., -(0.245 / 0.85))
 P: ofVec3f(-0.35, 0.35, 0.)
 Q: ofVec3f(-0.35, -0.35, 0.)
 R: ofVec3f(-0.35, 0.2, 0.)
 S: ofVec3f(-0.35, -0.2, 0.)
 T: ofVec3f(0., 0.5, 0.)
 U: ofVec3f(0., -0.5, 0.)
 V: ofVec3f(-0.2, 0.245 / 0.85, 0.)
 W: ofVec3f(-0.2, -(0.245 / 0.85), 0.)
 A1: ofVec3f(0.3, 0.07 / 0.85, 0.)
 B1: ofVec3f(0.3, -(0.07 / 0.85), 0.)
 */
