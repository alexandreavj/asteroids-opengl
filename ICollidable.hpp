//
//  ICollidable.hpp
//  asteroids
//
//  Created by Alexandre Jacob on 13/11/2024.
//

#ifndef ICollidable_hpp
#define ICollidable_hpp

#include <iostream>
#include <map>
#include "Particle.hpp"
#include "ofMain.h"


enum Border {
    LEFTBORDER,
    RIGHTBORDER,
    FRONTBORDER,
    BACKBORDER,
    TOPBORDER,
    BOTTOMBORDER
};


template <typename Element>
class ICollidable {
public:
    map<Border, GLfloat> isIn(ofVec3f center, ofVec3f dimensions) {
        const Element* elem = static_cast<const Element*>(this);
        
        map<Border, GLfloat> distanceFromBorders;
        
        if (elem->pos.x + elem->scale * 0.5 < center.x - dimensions.x * 0.5) {
            distanceFromBorders[LEFTBORDER] = abs((center.x - dimensions.x * 0.5) - (elem->pos.x + elem->scale * 0.5));
        } else if (elem->pos.x - elem->scale * 0.5 > center.x + dimensions.x * 0.5) {
            distanceFromBorders[RIGHTBORDER] = abs((center.x + dimensions.x * 0.5) - (elem->pos.x - elem->scale * 0.5));
        }
        
        if (elem->pos.y + elem->scale * 0.5 < center.y - dimensions.y * 0.5) {
            distanceFromBorders[BOTTOMBORDER] = abs((center.y - dimensions.y * 0.5) - (elem->pos.y + elem->scale * 0.5));
        } else if (elem->pos.y - elem->scale * 0.5 > center.y + dimensions.y * 0.5) {
            distanceFromBorders[TOPBORDER] = abs((center.y + dimensions.y * 0.5) - (elem->pos.y - elem->scale * 0.5));
        }
        
        if (elem->pos.z + elem->scale * 0.5 < center.z - dimensions.z * 0.5) {
            distanceFromBorders[BACKBORDER] = abs((center.z - dimensions.z * 0.5) - (elem->pos.z + elem->scale * 0.5));
        } else if (elem->pos.z - elem->scale * 0.5 > center.z + dimensions.z * 0.5) {
            distanceFromBorders[FRONTBORDER] = abs((center.z + dimensions.z * 0.5) - (elem->pos.z - elem->scale * 0.5));
        }
        
        return distanceFromBorders;
    }
    
    bool isCollision(ofVec3f center, float radius, bool is2D) {
        const Element* elem = static_cast<const Element*>(this);
        
        if (is2D) {
            return (ofVec2f(elem->pos.x, elem->pos.y).distance(ofVec2f(center.x, center.y)) <= (elem->scale * 0.5 + radius)) ? true : false;
        } else {
            return (elem->pos.distance(center) <= (elem->scale * 0.5 + radius)) ? true : false;
        }
    }
    
    virtual void updateCollision(long long timePeriod) = 0;
    
    virtual void drawCollision() = 0;
        
    virtual ~ICollidable() { }
    
    
    // GETTERS
    GLfloat getScale() { return scale; }
    GLfloat getBackupScale() { return backupScale; }
    ofVec3f getVel() { return vel; }
    GLfloat getVelX() { return vel.x; }
    GLfloat getVelY() { return vel.y; }
    GLfloat getVelZ() { return vel.z; }
    ofVec3f getPos() { return pos; }
    GLfloat getPosX() { return pos.x; }
    GLfloat getPosY() { return pos.y; }
    GLfloat getPosZ() { return pos.z; }
    ofVec3f getRot() { return rot; }
    GLfloat getRotX() { return rot.x; }
    GLfloat getRotY() { return rot.y; }
    GLfloat getRotZ() { return rot.z; }
    bool isAlive() { return alive; }
    bool getIsInCollision() { return isInCollision; }
    
    // SETTERS
    void setScale(GLfloat s) { scale = s; }
    void setVel(ofVec3f v) { vel = v; }
    void setVelX(GLfloat vx) { vel.x = vx; }
    void setVelY(GLfloat vy) { vel.y = vy; }
    void setVelZ(GLfloat vz) { vel.z = vz; }
    void setPos(ofVec3f p) { pos = p; }
    void setPosX(GLfloat px) { pos.x = px; }
    void setPosY(GLfloat py) { pos.y = py; }
    void setPosZ(GLfloat pz) { pos.z = pz; }
    void setRot(ofVec3f r) { rot = r; }
    void setRotX(GLfloat rx) { rot.x = rx; }
    void setRotY(GLfloat ry) { rot.y = ry; }
    void setRotZ(GLfloat rz) { rot.z = rz; }
    void setAlive(bool a) { alive = a; }
    void setIsInCollision(bool c) { isInCollision = c; }
    

protected:
    GLfloat scale;
    GLfloat backupScale;
    ofVec3f vel;
    ofVec3f pos;
    ofVec3f rot;
    
    bool alive;
    bool isInCollision;
    
    vector<Particle> particles;

};


#endif /* ICollidable_hpp */
