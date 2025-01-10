//
//  elements.cpp
//  asteroids
//
//  Created by Alexandre Jacob on 23/11/2024.
//

#include "elements.hpp"
#include "materials.hpp"


void unitSquare() {
    GLfloat p = 0.5;
    glBegin(GL_QUADS); {
        // front
        glNormal3f(0, 0, 1);
        glVertex3f(-p, p, 0);
        glVertex3f(-p, -p, 0);
        glVertex3f(p, -p, 0);
        glVertex3f(p, p, 0);
    } glEnd();
}


void unitCube() {
    GLfloat p = 0.5;
    glBegin(GL_QUADS); {
        // front
        glNormal3f(0, 0, 1);
        glVertex3f(-p, p, p);
        glVertex3f(-p, -p, p);
        glVertex3f(p, -p, p);
        glVertex3f(p, p, p);

        // back
        glNormal3f(0, 0, -1);
        glVertex3f(-p, p, -p);
        glVertex3f(p, p, -p);
        glVertex3f(p, -p, -p);
        glVertex3f(-p, -p, -p);

        // top
        glNormal3f(0, 1, 0);
        glVertex3f(-p, p, -p);
        glVertex3f(-p, p, p);
        glVertex3f(p, p, p);
        glVertex3f(p, p, -p);

        // bottom
        glNormal3f(0, -1, 0);
        glVertex3f(-p, -p, p);
        glVertex3f(-p, -p, -p);
        glVertex3f(p, -p, -p);
        glVertex3f(p, -p, p);

        // left
        glNormal3f(-1, 0, 0);
        glVertex3f(-p, p, p);
        glVertex3f(-p, p, -p);
        glVertex3f(-p, -p, -p);
        glVertex3f(-p, -p, p);

        // right
        glNormal3f(1, 0, 0);
        glVertex3f(p, p, p);
        glVertex3f(p, -p, p);
        glVertex3f(p, -p, -p);
        glVertex3f(p, p, -p);
    } glEnd();
}


void unitCubeTextured(ofImage texture) {
    GLfloat p = 0.5;
    
    texture.bind();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBegin(GL_QUADS); {
        // front
        glNormal3f(0, 0, 1);
        glTexCoord2f(0.0, 2.0); glVertex3f(-p, p, p);
        glTexCoord2f(0.0, 0.0); glVertex3f(-p, -p, p);
        glTexCoord2f(2.0, 0.0); glVertex3f(p, -p, p);
        glTexCoord2f(2.0, 2.0); glVertex3f(p, p, p);

        // back
        glNormal3f(0, 0, -1);
        glTexCoord2f(0.0, 2.0); glVertex3f(-p, p, -p);
        glTexCoord2f(2.0, 2.0); glVertex3f(p, p, -p);
        glTexCoord2f(2.0, 0.0); glVertex3f(p, -p, -p);
        glTexCoord2f(0.0, 0.0); glVertex3f(-p, -p, -p);

        // top
        glNormal3f(0, 1, 0);
        glTexCoord2f(0.0, 2.0); glVertex3f(-p, p, -p);
        glTexCoord2f(0.0, 0.0); glVertex3f(-p, p, p);
        glTexCoord2f(2.0, 0.0); glVertex3f(p, p, p);
        glTexCoord2f(2.0, 2.0); glVertex3f(p, p, -p);

        // bottom
        glNormal3f(0, -1, 0);
        glTexCoord2f(0.0, 2.0); glVertex3f(-p, -p, p);
        glTexCoord2f(2.0, 2.0); glVertex3f(p, -p, p);
        glTexCoord2f(2.0, 0.0); glVertex3f(p, -p, -p);
        glTexCoord2f(0.0, 0.0); glVertex3f(-p, -p, -p);

        // left
        glNormal3f(-1, 0, 0);
        glTexCoord2f(0.0, 2.0); glVertex3f(-p, p, p);
        glTexCoord2f(2.0, 2.0); glVertex3f(-p, p, -p);
        glTexCoord2f(2.0, 0.0); glVertex3f(-p, -p, -p);
        glTexCoord2f(0.0, 0.0); glVertex3f(-p, -p, p);

        // right
        glNormal3f(1, 0, 0);
        glTexCoord2f(0.0, 2.0); glVertex3f(p, p, p);
        glTexCoord2f(0.0, 0.0); glVertex3f(p, -p, p);
        glTexCoord2f(2.0, 0.0); glVertex3f(p, -p, -p);
        glTexCoord2f(2.0, 2.0); glVertex3f(p, p, -p);
    } glEnd();
    
    texture.unbind();
    glDisable(GL_TEXTURE_2D);
    texture.clear();
}


void unitSphere(int m, int n) {
    GLfloat d_theta = 2.0f * 3.1415926 / m;  // Incremental step for theta
    GLfloat d_phi = 3.1415926 / n;           // Incremental step for phi

    glBegin(GL_QUADS); {
        for (int i = 0; i < n; ++i) {
            GLfloat phi0 = i * d_phi;
            GLfloat phi1 = (i + 1) * d_phi;
            
            for (int j = 0; j < m; ++j) {
                GLfloat theta0 = j * d_theta;
                GLfloat theta1 = (j + 1) * d_theta;
                
                // Coordinates for the four vertices of the quad
                // Vertex 0 (theta0, phi0)
                GLfloat x0 = sinf(phi0) * cosf(theta0);
                GLfloat y0 = cosf(phi0);
                GLfloat z0 = sinf(phi0) * sinf(theta0);
                
                // Vertex 1 (theta1, phi0)
                GLfloat x1 = sinf(phi0) * cosf(theta1);
                GLfloat y1 = cosf(phi0);
                GLfloat z1 = sinf(phi0) * sinf(theta1);
                
                // Vertex 2 (theta1, phi1)
                GLfloat x2 = sinf(phi1) * cosf(theta1);
                GLfloat y2 = cosf(phi1);
                GLfloat z2 = sinf(phi1) * sinf(theta1);
                
                // Vertex 3 (theta0, phi1)
                GLfloat x3 = sinf(phi1) * cosf(theta0);
                GLfloat y3 = cosf(phi1);
                GLfloat z3 = sinf(phi1) * sinf(theta0);
                
                // Specify normals and vertices for the quad
                // Vertex 0
                glNormal3f(x0, y0, z0);
                glVertex3f(x0, y0, z0);
                
                // Vertex 1
                glNormal3f(x1, y1, z1);
                glVertex3f(x1, y1, z1);
                
                // Vertex 2
                glNormal3f(x2, y2, z2);
                glVertex3f(x2, y2, z2);
                
                // Vertex 3
                glNormal3f(x3, y3, z3);
                glVertex3f(x3, y3, z3);
            }
        }
    } glEnd();
}


void unitSector(float startAng, float ang, int segments) {
    ang *= (PI / 180.);
    startAng *= (PI / 180.);
    float angStep = ang / segments;

    glBegin(GL_TRIANGLE_FAN); {
        glNormal3f(0., 0., 1.);
        
        // center vertex
        glVertex2f(0., 0.);
        
        // generate vertices along the arc
        for (int i = 0; i <= segments; ++i) {
            float angle = startAng + (i * angStep);
            glVertex2f(0.5 * cos(angle), 0.5 * sin(angle));
        }
    }

    glEnd();
}


void unitString(string text, bool texture) {
    ofImage textureImg;
    if (texture) textureImg.load("text_square.png");
    
    glPushMatrix(); {
        glTranslatef(((text.size() * 5) + (text.size() - 1)) * -0.5, 7 * -0.5, 0.);
        for (int i = 0; i < text.size(); i++) {
            char currentChar = text[i];
            if (pixelArt.find(currentChar) != pixelArt.end()) {
                vector<vector<int>> grid = pixelArt[currentChar];
                for (int r = 0; r < 7; r++) {
                    for (int c = 0; c < 5; c++) {
                        if (grid[r][c] == 1) {
                            glPushMatrix(); {
                                glTranslatef((i * 5) + (i) + (c) + 0.5, (7 - r) - 0.5, 0);
                                if (texture) {
                                    unitCubeTextured(textureImg);
                                } else {
                                    unitCube();
                                }
                            } glPopMatrix();
                        }
                    }
                }
            }
        }
    } glPopMatrix();
}


void axis() {
    glBegin(GL_LINES); {
        //glColor3f(1., 0., 0.);
        loadMaterial(15);
        glNormal3f(1., 0., 0.);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        
        //glColor3f(0., 1., 0.);
        loadMaterial(14);
        glNormal3f(0., 1., 0.);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);

        //glColor3f(0., 0., 1.);
        loadMaterial(13);
        glNormal3f(0., 0., 1.);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
    } glEnd();
}


map<char, vector<vector<int>>> pixelArt = {
    {' ',
        {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}}
    },
    {':',
        {
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0}}
    },
    {'0',
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 1, 1},
            {1, 0, 1, 0, 1},
            {1, 1, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}}
    },
    {'1',
        {
            {0, 0, 1, 0, 0},
            {0, 1, 1, 0, 0},
            {1, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {1, 1, 1, 1, 1}}
    },
    {'2',
        {
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1}}
    },
    {'3',
        {
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}}
    },
    {'4',
        {
            {1, 0, 0, 1, 0},
            {1, 0, 0, 1, 0},
            {1, 0, 0, 1, 0},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 1, 0}}
    },
    {'5',
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}}
    },
    {'6',
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1}}
    },
    {'7',
        {{1, 1, 1, 1, 1},
         {0, 0, 0, 0, 1},
         {0, 0, 0, 1, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0}}
    },
    {'8',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}}
    },
    {'9',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1},
         {0, 0, 0, 0, 1},
         {0, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}}
    },
    {'A',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1}}
    },
    {'B',
        {{1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 0}}
    },
    {'C',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1}}
    },
    {'D',
        {{1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 0}}
    },
    {'E',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1}}
    },
    {'F',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0}}
    },
    {'G',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}}
    },
    {'H',
        {{1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1}}
    },
    {'I',
        {{1, 1, 1, 1, 1},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {1, 1, 1, 1, 1}}
    },
    {'J',
        {{0, 0, 0, 1, 0},
         {0, 0, 0, 1, 0},
         {0, 0, 0, 1, 0},
         {1, 0, 0, 1, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1}}
    },
    {'K',
        {{1, 0, 0, 0, 1},
         {1, 0, 1, 0, 0},
         {1, 1, 0, 0, 0},
         {1, 0, 0, 1, 0},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1}}
    },
    {'L',
        {{1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1}}
    },
    {'M',
        {{1, 0, 0, 0, 1},
         {1, 1, 0, 1, 1},
         {1, 0, 1, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1}}
    },
    {'N',
        {{1, 0, 0, 0, 1},
         {1, 1, 0, 0, 1},
         {1, 0, 1, 0, 1},
         {1, 0, 0, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1}}
    },
    {'O',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}}
    },
    {'P',
        {{1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0}}
    },
    {'Q',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 1, 0, 1},
         {1, 0, 1, 1, 0},
         {1, 0, 0, 1, 0},
         {1, 1, 0, 0, 0}}
    },
    {'R',
        {{1, 1, 1, 1, 0},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 0},
         {1, 0, 1, 0, 0},
         {1, 0, 0, 1, 0},
         {1, 0, 0, 0, 1}}
    },
    {'S',
        {{1, 1, 1, 1, 1},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1},
         {0, 0, 0, 0, 1},
         {0, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}}
    },
    {'T',
        {{1, 1, 1, 1, 1},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0}}
    },
    {'U',
        {{1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 1, 1, 1, 1}}
    },
    {'V',
        {{1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {0, 1, 0, 1, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0}}
    },
    {'W',
        {{1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 1, 0, 1},
         {1, 0, 1, 1, 1},
         {1, 1, 0, 1, 1},
         {1, 0, 0, 0, 1},
         {1, 0, 0, 0, 1}}
    },
    {'X',
        {{1, 0, 0, 0, 1},
         {0, 1, 0, 1, 0},
         {0, 0, 1, 0, 0},
         {0, 1, 0, 1, 0},
         {1, 0, 0, 0, 1},
         {0, 1, 0, 1, 0},
         {1, 0, 0, 0, 1}}
    },
    {'Y',
        {{1, 0, 0, 0, 1},
         {0, 1, 0, 1, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 1, 0, 0}}
    },
    {'Z',
        {{1, 1, 1, 1, 1},
         {0, 0, 0, 1, 0},
         {0, 0, 1, 0, 0},
         {0, 1, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0},
         {1, 1, 1, 1, 1}}
    },
    {'<',
        {{1, 1, 0, 1, 1},
         {1, 1, 0, 1, 1},
         {1, 1, 1, 1, 1},
         {1, 1, 1, 1, 1},
         {1, 1, 1, 1, 1},
         {0, 1, 1, 1, 0},
         {0, 0, 1, 0, 0}}
    }
};
