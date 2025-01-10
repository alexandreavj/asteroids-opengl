#ifndef viewTools_hpp
#define viewTools_hpp

#include <stdio.h>
#include "ofMain.h"
#include "tools.hpp"


void perspective(GLfloat theta, GLfloat alpha, GLfloat beta);

void lookAt(GLfloat camX,
            GLfloat camY,
            GLfloat camZ,
            GLfloat targetX,
            GLfloat targetY,
            GLfloat targetZ,
            GLfloat upX,
            GLfloat upY,
            GLfloat upZ);

#endif
