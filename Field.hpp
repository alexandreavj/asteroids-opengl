//
//  Field.hpp
//  asteroids
//
//  Created by Alexandre Jacob on 12/11/2024.
//

#ifndef Field_hpp
#define Field_hpp

#include <iostream>
#include <cassert>
#include "ofMain.h"


class Field {
public:
    Field(ofVec3f dim, int rows, int cols);
    void drawField(GLint penSize = 1);
    
    ofVec3f dim;
private:
    int rows, cols;
};


#endif /* Field_hpp */
