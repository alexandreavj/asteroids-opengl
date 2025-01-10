//
//  Field.cpp
//  asteroids
//
//  Created by Alexandre Jacob on 12/11/2024.
//

#include "Field.hpp"
#include "materials.hpp"


Field::Field(ofVec3f dim, int rows, int cols) {
    assert((dim.x > 0 && dim.y > 0 && dim.z > 0 && rows > 0 && cols > 0) && "Field dimensions/rows/column must be positive.");
    
    this->dim = dim;
    this->rows = rows;
    this->cols = cols;
}


void Field::drawField(GLint penSize) {
    loadMaterial(16);
    
    assert(penSize > 0 && "Pen size must be postive.");
    // get previous line width
    GLint prevLineWidth;
    glGetIntegerv(GL_LINE_WIDTH, &prevLineWidth);
    glLineWidth(penSize);
    
    // calculate mesh specs
    GLfloat rowHeight = this->dim.y / this->rows;
    GLfloat colWidth = this->dim.x / this->cols;
        
    glBegin(GL_LINES); {
        // back
        glNormal3f(0., 0., 1.);
        for (int i = 0; i <= rows; i++) {
            glVertex3d(-dim.x * 0.5, dim.y * 0.5 - i * rowHeight, -dim.z * 0.5);
            glVertex3d(dim.x * 0.5, dim.y * 0.5 - i * rowHeight, -dim.z * 0.5);
        }
        for (int i = 0; i <= cols; i++) {
            glVertex3d(dim.x * 0.5 - i * colWidth, -dim.y * 0.5, -dim.z * 0.5);
            glVertex3d(dim.x * 0.5 - i * colWidth, dim.y * 0.5, -dim.z * 0.5);
        }
        
        // front
        glNormal3f(0., 0., -1.);
        for (int i = 0; i <= rows; i++) {
            glVertex3d(-dim.x * 0.5, dim.y * 0.5 - i * rowHeight, dim.z * 0.5);
            glVertex3d(dim.x * 0.5, dim.y * 0.5 - i * rowHeight, dim.z * 0.5);
        }
        for (int i = 0; i <= cols; i++) {
            glVertex3d(dim.x * 0.5 - i * colWidth, -dim.y * 0.5, dim.z * 0.5);
            glVertex3d(dim.x * 0.5 - i * colWidth, dim.y * 0.5, dim.z * 0.5);
        }
        
        
        GLfloat currentCoord = 0.;
        
        // left
        glNormal3f(1., 0., 0.);
        currentCoord = dim.y * 0.5;
        while (currentCoord >= -dim.y * 0.5) {
            glVertex3d(-dim.x * 0.5, currentCoord, -dim.z * 0.5);
            glVertex3d(-dim.x * 0.5, currentCoord, dim.z * 0.5);
            currentCoord -= colWidth;
        }
        currentCoord = dim.z * 0.5;
        while (currentCoord >= -dim.z * 0.5) {
            glVertex3d(-dim.x * 0.5, -dim.y * 0.5, currentCoord);
            glVertex3d(-dim.x * 0.5, dim.y * 0.5, currentCoord);
            currentCoord -= rowHeight;
        }
        
        // right
        glNormal3f(-1., 0., 0.);
        currentCoord = dim.y * 0.5;
        while (currentCoord >= -dim.y * 0.5) {
            glVertex3d(dim.x * 0.5, currentCoord, -dim.z * 0.5);
            glVertex3d(dim.x * 0.5, currentCoord, dim.z * 0.5);
            currentCoord -= colWidth;
        }
        currentCoord = dim.z * 0.5;
        while (currentCoord >= -dim.z * 0.5) {
            glVertex3d(dim.x * 0.5, -dim.y * 0.5, currentCoord);
            glVertex3d(dim.x * 0.5, dim.y * 0.5, currentCoord);
            currentCoord -= rowHeight;
        }
        
        // top
        glNormal3f(0., -1., 0.);
        currentCoord = dim.z * 0.5;
        while (currentCoord >= -dim.z * 0.5) {
            glVertex3d(dim.x * 0.5, dim.y * 0.5, currentCoord);
            glVertex3d(-dim.x * 0.5, dim.y * 0.5, currentCoord);
            currentCoord -= rowHeight;
        }
        currentCoord = dim.x * 0.5;
        while (currentCoord >= -dim.x * 0.5) {
            glVertex3d(currentCoord, dim.y * 0.5, -dim.z * 0.5);
            glVertex3d(currentCoord, dim.y * 0.5, dim.z * 0.5);
            currentCoord -= colWidth;
        }
        
        // bottom
        glNormal3f(0., 1., 0.);
        currentCoord = dim.z * 0.5;
        while (currentCoord >= -dim.z * 0.5) {
            glVertex3d(dim.x * 0.5, -dim.y * 0.5, currentCoord);
            glVertex3d(-dim.x * 0.5, -dim.y * 0.5, currentCoord);
            currentCoord -= rowHeight;
        }
        currentCoord = dim.x * 0.5;
        while (currentCoord >= -dim.x * 0.5) {
            glVertex3d(currentCoord, -dim.y * 0.5, -dim.z * 0.5);
            glVertex3d(currentCoord, -dim.y * 0.5, dim.z * 0.5);
            currentCoord -= colWidth;
        }
    } glEnd();
    
    
    // reset line width
    glLineWidth(prevLineWidth);
}
