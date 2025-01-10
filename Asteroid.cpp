//
//  asteroid.cpp
//  asteroids
//
//  Created by Alexandre Jacob on 04/11/2024.
//

#include "Asteroid.hpp"
#include "materials.hpp"


Asteroid::Asteroid(int nbrVertices, GLfloat scale) {
    TEXTURE.load("asteroid.png");
    
    // initialize random number generator
    std::random_device seed;
    std::mt19937 gen(seed());
    
    // set range for number generation
    std::uniform_real_distribution<float> range(-0.5, 0.5);
    
    this->scale = scale;
    this->backupScale = scale;
    this->alive = true;
    this->isInCollision = false;
    this->divide = false;
    
    assert((nbrVertices >= 0) && "The number of extra vertices for the equator vertices of the asteroid must be non negative.");
    
    // generate random vertices
    for (int i = 0; i < nbrVertices; i++) {
        ofVec2f vertex = ofVec2f(0, 0);
        
        // generate a new vertex until it is inside the given constraints
        while (dist(vertex, ofVec2f(0, 0)) < INNER_BORDER ) {
            // random vertex out of bounds, regenerate
            vertex = ofVec2f(range(gen), range(gen));
        }
        
        // add vertex to list
        this->equatorVertices.push_back(vertex);
    }
    
    this->equatorVertices.push_back(ofVec3f(0.5, 0., 0.));
    this->equatorVertices.push_back(ofVec3f(0., 0.5, 0.));
    this->equatorVertices.push_back(ofVec3f(-0.5, 0. ,0.));
    this->equatorVertices.push_back(ofVec3f(0., -0.5, 0.));
    
    // sort vertexes to make sure that right-hand rule is respected
    sort(this->equatorVertices.begin(), this->equatorVertices.end(), [](ofVec2f a, ofVec2f b) {
        return angle(a) < angle(b);
    });
}


Asteroid::~Asteroid() {
    TEXTURE.clear();
}


void Asteroid::drawAsteroid() {
    loadMaterial(19);
    
    this->drawCollision();
    
    glPushMatrix(); {
        glTranslatef(this->pos.x, this->pos.y, this->pos.z);
        glRotatef(this->rot.z, 0., 0., 1.);
        glRotatef(this->rot.y, 0., 1., 0.);
        glRotatef(this->rot.x, 1., 0., 0.);
        glScalef(scale, scale, scale);
        Asteroid::unitAsteroid(this->equatorVertices, false);
    } glPopMatrix();
}


void Asteroid::movement(long long timePeriod) {
    this->updateCollision(timePeriod);
    if (!isInCollision) {
        this->pos += this->vel * (timePeriod / 1000.);
        this->rot += ofVec3f(0.5, 0.5, 1.) * (120. / max(static_cast<double>(ofGetFrameRate()), 1.) );
    }
}


void Asteroid::updateCollision(long long timePeriod) {
    if (this->isInCollision) {
        if (particles.size() == 0) {
            for (int i = 0; i < this->scale * 5; i++) {
                particles.push_back(Particle(this->pos, ofVec3f(ofRandom(-1., 1.), ofRandom(-1, 1.), ofRandom(-1, 1.)).getNormalized() * ofRandom(25., 50.), this->scale * 3, this->scale / 25., CUBE));
            }
        } else {
            bool allDead = true;
            for (int i = 0; i < particles.size(); i++) {
                particles[i].movement(timePeriod);
                if (particles[i].alive) allDead = false;
            }
            
            if (allDead) {
                this->alive = false;
                particles.clear();
                this->pos.set(0., 0., 0.);
                this->vel.set(0., 0., 0.);
            } else {
                this->scale -= 1.5 * (120. / max(static_cast<double>(ofGetFrameRate()), 1.));
                
                if (this->scale <= this->backupScale * 0.5) this->divide = true;
                
                if (this->scale < 0) this->scale = 0;
            }
        }
    }
}


void Asteroid::drawCollision() {
    if (this->isInCollision) {
        for (int i = 0; i < particles.size(); i++) {
            particles[i].drawParticle();
        }
    }
}


void Asteroid::unitAsteroid(vector<ofVec2f> equatorVertices, bool outline) {
    // texture
    glEnable(GL_TEXTURE_2D);

    TEXTURE.bind();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    
    int nbrVertices = equatorVertices.size();
    
    float poleHeight = 0.5 * sqrt(1 - pow(POLES_SCALE, 2));
    
    for (int j = 0; j < 2; j++) {
        glBegin(GL_TRIANGLE_FAN); {
            if (j == 0) {
                // top
                glNormal3f(0., 0., 1.);
            } else {
                // bottom
                glNormal3f(0., 0., -1.);
            }
            
            glTexCoord2d(0.5, 0.5);
            glVertex3d(0, 0, poleHeight * pow(-1, j));
            
            for (int i = 0; i <= nbrVertices; i++) {
                int idx = i % nbrVertices;
                
                glTexCoord2d(equatorVertices[idx].x + 0.5, equatorVertices[idx].y + 0.5);
                glVertex3d(equatorVertices[idx].x * 0.5, equatorVertices[idx].y * 0.5, poleHeight * pow(-1, j));
            }
        } glEnd();
    }
    
    // vertices are sorted in ascending order, according to their angle
    int textQuadCounter = -1;
    float textRow, textCol;
    float rowHeigth = 1.0 / TEXTURE_ROWS;
    float colWidth = 1.0 / TEXTURE_COLS;
    glBegin(GL_QUADS); {
        // faces top
        for (int i = 0; i < nbrVertices; i++) {
            textQuadCounter += 1;
            textQuadCounter = textQuadCounter % (TEXTURE_ROWS * TEXTURE_COLS);
            
            textCol = (textQuadCounter / TEXTURE_COLS) * rowHeigth;
            textRow = (textQuadCounter % TEXTURE_COLS) * colWidth;
            
            // for normal, execute cross product between the vertical edge with greater angle and bottom edge
            ofVec3f a = ofVec3f(equatorVertices[(i + 1) % nbrVertices].x * 0.5, equatorVertices[(i + 1) % nbrVertices].y * 0.5, poleHeight) - ofVec3f(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
            ofVec3f b = ofVec3f(equatorVertices[i].x, equatorVertices[i].y, 0.) - ofVec3f(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
            ofVec3f normal = crossProduct(a, b).getNormalized();
            glNormal3f(normal.x, normal.y, normal.z);
            
            glTexCoord2d(textRow + rowHeigth, textCol);
            glVertex3d(equatorVertices[(i + 1) % nbrVertices].x * 0.5, equatorVertices[(i + 1) % nbrVertices].y * 0.5, poleHeight);
            
            glTexCoord2d(textRow, textCol);
            glVertex3d(equatorVertices[i].x * 0.5, equatorVertices[i].y * 0.5, poleHeight);
            
            glTexCoord2d(textRow, textCol + colWidth);
            glVertex3d(equatorVertices[i].x, equatorVertices[i].y, 0.);
            
            glTexCoord2d(textRow + rowHeigth, textCol + colWidth);
            glVertex3d(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
        }
        
        // faces bottom
        for (int i = 0; i < nbrVertices; i++) {
            textQuadCounter += 1;
            textQuadCounter = textQuadCounter % (TEXTURE_ROWS * TEXTURE_COLS);
            
            textCol= (textQuadCounter / TEXTURE_COLS) * rowHeigth;
            textRow = (textQuadCounter % TEXTURE_COLS) * colWidth;
            
            // for normal, execute cross product between the top edge and the vertical edge with greater angle
            ofVec3f a = ofVec3f(equatorVertices[i].x, equatorVertices[i].y, 0.) - ofVec3f(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
            ofVec3f b = ofVec3f(equatorVertices[(i + 1) % nbrVertices].x * 0.5, equatorVertices[(i + 1) % nbrVertices].y * 0.5, -poleHeight) - ofVec3f(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
            ofVec3f normal = crossProduct(a, b).getNormalized();
            glNormal3f(normal.x, normal.y, normal.z);
            
            glTexCoord2d(textRow + rowHeigth, textCol);
            glVertex3d(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
            
            glTexCoord2d(textRow, textCol);
            glVertex3d(equatorVertices[i].x, equatorVertices[i].y, 0.);
            
            glTexCoord2d(textRow, textCol + colWidth);
            glVertex3d(equatorVertices[i].x * 0.5, equatorVertices[i].y * 0.5, -poleHeight);
            
            glTexCoord2d(textRow + rowHeigth, textCol + colWidth);
            glVertex3d(equatorVertices[(i + 1) % nbrVertices].x * 0.5, equatorVertices[(i + 1) % nbrVertices].y * 0.5, -poleHeight);
        }
    } glEnd();
    
    TEXTURE.unbind();
    glDisable(GL_TEXTURE);
    
    if (outline) {        
        for (int j = 0; j < 2; j++) {
            glBegin(GL_LINE_LOOP); {
                if (j == 0) {
                    // top
                    glNormal3f(0., 0., 1.);
                } else {
                    // bottom
                    glNormal3f(0., 0., -1.);
                }
                for (int i = 0; i <= nbrVertices; i++) {
                    int idx = i % nbrVertices;
                    glVertex3d(equatorVertices[idx].x * 0.5, equatorVertices[idx].y * 0.5, poleHeight * pow(-1, j));
                }
            } glEnd();
        }
        
        // faces top
        for (int i = 0; i < nbrVertices; i++) {
            glBegin(GL_LINE_LOOP); {
                ofVec3f a = ofVec3f(equatorVertices[(i + 1) % nbrVertices].x * 0.5, equatorVertices[(i + 1) % nbrVertices].y * 0.5, poleHeight) - ofVec3f(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
                ofVec3f b = ofVec3f(equatorVertices[i].x, equatorVertices[i].y, 0.) - ofVec3f(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
                ofVec3f normal = crossProduct(a, b).getNormalized();
                glVertex3d(equatorVertices[(i + 1) % nbrVertices].x * 0.5, equatorVertices[(i + 1) % nbrVertices].y * 0.5, poleHeight);
                glVertex3d(equatorVertices[i].x * 0.5, equatorVertices[i].y * 0.5, poleHeight);
                glVertex3d(equatorVertices[i].x, equatorVertices[i].y, 0.);
                glVertex3d(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
            } glEnd();
        }
        
        // faces bottom
        for (int i = 0; i < nbrVertices; i++) {
            glBegin(GL_LINE_LOOP); {
                ofVec3f a = ofVec3f(equatorVertices[i].x, equatorVertices[i].y, 0.) - ofVec3f(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
                ofVec3f b = ofVec3f(equatorVertices[(i + 1) % nbrVertices].x * 0.5, equatorVertices[(i + 1) % nbrVertices].y * 0.5, -poleHeight) - ofVec3f(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
                ofVec3f normal = crossProduct(a, b).getNormalized();
                glVertex3d(equatorVertices[(i + 1) % nbrVertices].x, equatorVertices[(i + 1) % nbrVertices].y, 0.);
                glVertex3d(equatorVertices[i].x, equatorVertices[i].y, 0.);
                glVertex3d(equatorVertices[i].x * 0.5, equatorVertices[i].y * 0.5, -poleHeight);
                glVertex3d(equatorVertices[(i + 1) % nbrVertices].x * 0.5, equatorVertices[(i + 1) % nbrVertices].y * 0.5, -poleHeight);
            } glEnd();
        }
    }
}


void Asteroid::setBackupScale(GLfloat backupScale) { this->backupScale = backupScale; }

void Asteroid::setDivide(bool divide) { this->divide = divide; }

bool Asteroid::getDivide() { return this->divide; }
