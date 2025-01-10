#include "ofApp.h"


std::random_device SEED;
std::mt19937 GEN;


//--------------------------------------------------------------
void ofApp::setup() {
    ofDisableArbTex();
    
    
    std::srand(std::time(nullptr));
    
    // initialize random number generator
    GEN = std::mt19937(SEED());
    std::uniform_real_distribution<float> range;
    
    ofBackground(0, 0, 0);
    
    ofSetWindowTitle("asteroids");
    
    glEnable(GL_DEPTH_TEST);
    
    FIELD = new Field(ofVec3f(ofGetWidth(), ofGetHeight(), 1000.), 6, 9);
    
    SPACESHIP = new Spaceship(35.);
    SS_PREV_Z = 0.;
    
    LAST_FRAME = getCurrentTimestamp();
    
    for (int i = 0; i < MIN_ASTEROIDS; i++) {
        ASTEROIDS.push_back(Asteroid((std::rand() % 17) + 3., (std::rand() % 75) + 25.));
        ASTEROIDS_PREV_Z.push_back(0.);
        ASTEROIDS[i].setPos(ofVec3f(-ofGetWidth(), -ofGetHeight(), 0.));
        ASTEROIDS[i].setRot(ofVec3f(std::rand() % 360, std::rand() % 360, std::rand() % 360));
    }
    
    FROM_2D = false;
    
    DRAW_AXIS = false;
    
    LIVES = INITIAL_LIVES;
    SCORE = 0;
    STATE = WELCOME;
    AMBIENT_ON = true;
    DIRECTIONAL_ON[0] = DIRECTIONAL_ON[1] = DIRECTIONAL_ON[2] = true;
    HEADLIGHT_ON[0] = HEADLIGHT_ON[1] = HEADLIGHT_ON[2] = true;
    FIRE_ON[0] = FIRE_ON[1] = FIRE_ON[2] = true;
    DIRECTIONAL_ON_GLOBAL = true;
    HEADLIGHT_ON_GLOBAL = true;
    FIRE_ON_GLOBAL = true;
}


//--------------------------------------------------------------
void ofApp::updateSpaceship(long long elapsedTime) {
    // GIVE SPACESHIP THE MOVEMENT COMMANDS
    if (ACTIVATED_KEYS[LSHIFT] && !ACTIVATED_KEYS[SPACE]) {                  // GO LOWER
        SPACESHIP->command = DOWN;
        SPACESHIP->movement(elapsedTime);
    } else if (ACTIVATED_KEYS[SPACE] && !ACTIVATED_KEYS[LSHIFT]) {           // GO HIGHER
        SPACESHIP->command = UP;
        SPACESHIP->movement(elapsedTime);
    } else {
        SPACESHIP->command = DRIFT_Z;
        SPACESHIP->movement(elapsedTime);
    }
    
    if (ACTIVATED_KEYS[A] && !ACTIVATED_KEYS[D]) {              // LEFT
        if (ACTIVATED_KEYS[W] && !ACTIVATED_KEYS[S]) {          // LEFT & FORWARD
            SPACESHIP->command = LEFT;
            SPACESHIP->movement(elapsedTime);
            SPACESHIP->command = FORWARD;
            SPACESHIP->movement(elapsedTime);
        } else if (ACTIVATED_KEYS[S] && !ACTIVATED_KEYS[W]) {   // LEFT & BACKWARD
            SPACESHIP->command = LEFT;
            SPACESHIP->movement(elapsedTime);
            SPACESHIP->command = BACKWARD;
            SPACESHIP->movement(elapsedTime);
        } else {                                                // LEFT & NONE
            SPACESHIP->command = LEFT;
            SPACESHIP->movement(elapsedTime);
            SPACESHIP->command = DRIFT_2D;
            SPACESHIP->movement(elapsedTime);
        }
    } else if (ACTIVATED_KEYS[D] && !ACTIVATED_KEYS[A]) {       // RIGHT
        if (ACTIVATED_KEYS[W] && !ACTIVATED_KEYS[S]) {          // RIGHT & FORWARD
            SPACESHIP->command = RIGHT;
            SPACESHIP->movement(elapsedTime);
            SPACESHIP->command = FORWARD;
            SPACESHIP->movement(elapsedTime);
        } else if (ACTIVATED_KEYS[S] && !ACTIVATED_KEYS[W]) {   // RIGHT & BACKWARD
            SPACESHIP->command = RIGHT;
            SPACESHIP->movement(elapsedTime);
            SPACESHIP->command = BACKWARD;
            SPACESHIP->movement(elapsedTime);
        } else {                                                // RIGHT & NONE
            SPACESHIP->command = RIGHT;
            SPACESHIP->movement(elapsedTime);
            SPACESHIP->command = DRIFT_2D;
            SPACESHIP->movement(elapsedTime);
        }
    } else {                                                    // NO ROTATION
        if (ACTIVATED_KEYS[W] && !ACTIVATED_KEYS[S]) {          // FORWARD
            SPACESHIP->command = FORWARD;
            SPACESHIP->movement(elapsedTime);
        } else if (ACTIVATED_KEYS[S] && !ACTIVATED_KEYS[W]) {   // BACKWARD
            SPACESHIP->command = BACKWARD;
            SPACESHIP->movement(elapsedTime);
        } else {                                                // NONE
            SPACESHIP->command = DRIFT_2D;
            SPACESHIP->movement(elapsedTime);
        }
    }
    
    
    // CHECK IF SPACESHIP IS IN THE GAME FIELD
    map<Border, GLfloat> ssDistBorders = SPACESHIP->isIn(ofVec3f(0., 0., 0.), FIELD->dim);
    
    if (ssDistBorders.find(LEFTBORDER) != ssDistBorders.end()) {
        SPACESHIP->setPosX(FIELD->dim.x * 0.5 - 1 + SPACESHIP->getScale() * 0.5);
    } else if (ssDistBorders.find(RIGHTBORDER) != ssDistBorders.end()) {
        SPACESHIP->setPosX(-(FIELD->dim.x * 0.5 - 1 + SPACESHIP->getScale() * 0.5));
    }
    
    if (ssDistBorders.find(BOTTOMBORDER) != ssDistBorders.end()) {
        SPACESHIP->setPosY(FIELD->dim.y * 0.5 - 1 + SPACESHIP->getScale() * 0.5);
    } else if (ssDistBorders.find(TOPBORDER) != ssDistBorders.end()) {
        SPACESHIP->setPosY(-(FIELD->dim.y * 0.5 - 1 + SPACESHIP->getScale() * 0.5));
    }
    
    if (ssDistBorders.find(BACKBORDER) != ssDistBorders.end()) {
        SPACESHIP->setPosZ(FIELD->dim.z * 0.5 - 1 + SPACESHIP->getScale() * 0.5);
    } else if (ssDistBorders.find(FRONTBORDER) != ssDistBorders.end()) {
        SPACESHIP->setPosZ(-(FIELD->dim.z * 0.5 - 1 + SPACESHIP->getScale() * 0.5));
    }
    
    
    // CHECK isInCollision WITH ASTEROIDS
    if (!SPACESHIP->getIsInCollision()) {
        for (int i = 0; i < ASTEROIDS.size(); i++) {
            if (ASTEROIDS[i].isAlive() && !ASTEROIDS[i].getIsInCollision() && SPACESHIP->isCollision(ASTEROIDS[i].getPos(), ASTEROIDS[i].getScale() * 0.5, VIEW_MODE == TWO_D)) {
                SPACESHIP->setIsInCollision(true);
                LIVES -= 1;
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::updatePewPewPews(long long elapsedTime) {
    for (int i = 0; i < PEWPEWPEWS.size(); i++) {
        PEWPEWPEWS[i].movement(elapsedTime);
        
        for (int j = 0; j < ASTEROIDS.size(); j++) {
            if (PEWPEWPEWS[i].isCollision(ASTEROIDS[j].getPos(), ASTEROIDS[j].getScale() * 0.5, VIEW_MODE == TWO_D)) {
                PEWPEWPEWS[i].setIsInCollision(true);
                ASTEROIDS[j].setIsInCollision(true);
                SCORE += ASTEROIDS[j].getScale();
            }
        }
        
        if (!PEWPEWPEWS[i].isIn(ofVec3f(0., 0., 0.), FIELD->dim).empty() ||
            (PEWPEWPEWS[i].getIsInCollision() && PEWPEWPEWS[i].getScale() == 0)) {
            PEWPEWPEWS.erase(PEWPEWPEWS.begin() + i);
        }
    }
}


//--------------------------------------------------------------
void ofApp::updateAsteroids(long long elapsedTime) {
    std::uniform_real_distribution<float> range;

    // CHECK FOR DEAD ASTEROIDS AND DIVIDE THEM
    for (int i = 0; i < ASTEROIDS.size(); i++) {
        if (ASTEROIDS[i].getDivide()) {
            if (ASTEROIDS[i].getScale() >= 25.) {
                // new asteroid 1
                ASTEROIDS.push_back(Asteroid((std::rand() % 17) + 3., ASTEROIDS[i].getScale()));
                ASTEROIDS_PREV_Z.push_back(ASTEROIDS[i].getPosZ());

                if (VIEW_MODE == TWO_D) {
                    ASTEROIDS.back().setPos(ofVec3f(ASTEROIDS[i].getPosX(), ASTEROIDS[i].getPosY(), 0.));
                    ASTEROIDS.back().setVel(ASTEROIDS[i].getVel());
                    ASTEROIDS.back().setVelZ(0);
                } else {
                    ASTEROIDS.back().setPos(ASTEROIDS[i].getPos());
                    ASTEROIDS.back().setVel(ASTEROIDS[i].getVel());
                }
                ASTEROIDS.back().setRot(ofVec3f(std::rand() % 360, std::rand() % 360, std::rand() % 360));

                // new asteroid 2
                ASTEROIDS.push_back(Asteroid((std::rand() % 17) + 3., ASTEROIDS[i].getScale()));
                ASTEROIDS_PREV_Z.push_back(ASTEROIDS[i].getPosZ());

                if (VIEW_MODE == TWO_D) {
                    ASTEROIDS.back().setPos(ofVec3f(ASTEROIDS[i].getPosX(), ASTEROIDS[i].getPosY(), 0.));
                    ASTEROIDS.back().setVel(-ASTEROIDS[i].getVel());
                    ASTEROIDS.back().setVelZ(0);
                } else {
                    ASTEROIDS.back().setPos(ASTEROIDS[i].getPos());
                    ASTEROIDS.back().setVel(-ASTEROIDS[i].getVel());
                }
                ASTEROIDS.back().setRot(ofVec3f(std::rand() % 360, std::rand() % 360, std::rand() % 360));
            }

            ASTEROIDS[i].setBackupScale(0.);
            ASTEROIDS[i].setDivide(false);
        }

        if (!ASTEROIDS[i].isAlive()) {
            ASTEROIDS.erase(ASTEROIDS.begin() + i);
        }
    }

    // CHECK IF THERE ARE MIN_ASTEROIDS
    for (int i = ASTEROIDS.size(); i < MIN_ASTEROIDS; i++) {
        ASTEROIDS.push_back(Asteroid((std::rand() % 17) + 3., (std::rand() % 75) + 25.));
        ASTEROIDS_PREV_Z.push_back(0.);
        ASTEROIDS[i].setPos(ofVec3f(-ofGetWidth(), -ofGetHeight(), 0.));
        ASTEROIDS[i].setRot(ofVec3f(std::rand() % 360, std::rand() % 360, std::rand() % 360));

        if (VIEW_MODE == TWO_D) {
            ASTEROIDS_PREV_Z[i] = ASTEROIDS[i].getPosZ();
            ASTEROIDS[i].setPosZ(0.);
        }
    }

    // CHECK IF ASTEROIDS ARE IN THE GAME FIELD
    for (int i = 0; i < ASTEROIDS.size(); i++) {
        ASTEROIDS[i].movement(elapsedTime);

        if (!ASTEROIDS[i].isIn(ofVec3f(0., 0., 0.), FIELD->dim).empty()) {
            switch (std::rand() % 2) {
                case 0:
                    range = std::uniform_real_distribution<float>(-(FIELD->dim.x * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1), FIELD->dim.x * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1);
                    ASTEROIDS[i].setPosX(range(GEN));
                    ASTEROIDS[i].setPosY((std::rand() % 2 == 0) ? -(FIELD->dim.y * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1) : FIELD->dim.y * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1);
                    range = std::uniform_real_distribution<float>(-(FIELD->dim.z * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1), FIELD->dim.z * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1);
                    ASTEROIDS[i].setPosZ(range(GEN));
                    break;

                case 1:
                    ASTEROIDS[i].setPosX((std::rand() % 2 == 0) ? -(FIELD->dim.x * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1) : FIELD->dim.x * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1);
                    range = std::uniform_real_distribution<float>(-(FIELD->dim.y * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1), FIELD->dim.y * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1);
                    ASTEROIDS[i].setPosY(range(GEN));
                    range = std::uniform_real_distribution<float>(-(FIELD->dim.z * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1), FIELD->dim.z * 0.5 + ASTEROIDS[i].getScale() * 0.5 - 1);
                    ASTEROIDS[i].setPosZ(range(GEN));
                    break;
            }

            range = std::uniform_real_distribution<float>(-FIELD->dim.x * 0.5, FIELD->dim.x * 0.5);
            ASTEROIDS[i].setVelX(range(GEN));
            range = std::uniform_real_distribution<float>(-FIELD->dim.y * 0.5, FIELD->dim.y * 0.5);
            ASTEROIDS[i].setVelY(range(GEN));
            range = std::uniform_real_distribution<float>(-FIELD->dim.z * 0.5, FIELD->dim.z * 0.5);
            ASTEROIDS[i].setVelZ(range(GEN));

            ASTEROIDS[i].setVel(ASTEROIDS[i].getVel().getNormalized() * (std::rand() % 100 + 25));
        }
    }

    // CHECK COLLISIONS BETWEEN ASTEROIDS
    for (int i = 0; i < ASTEROIDS.size(); i++) {
        if (ASTEROIDS[i].isAlive() && !ASTEROIDS[i].getIsInCollision()) {
            float magnitude = ASTEROIDS[i].getVel().length();
            bool alreadyDetected = false;

            for (int j = 0; j < ASTEROIDS.size(); j++) {
                if (i != j && ASTEROIDS[j].isAlive() && !ASTEROIDS[j].getIsInCollision()) {
                    if (ASTEROIDS[i].isCollision(ASTEROIDS[j].getPos(), ASTEROIDS[j].getScale() * 0.5, VIEW_MODE == TWO_D)) {
                        if (!alreadyDetected) {
                            ASTEROIDS[i].setVel(ofVec3f(0., 0., 0.));
                            alreadyDetected = true;
                        }
                        ASTEROIDS[i].setVel(ASTEROIDS[i].getVel() + -(ASTEROIDS[j].getPos() - ASTEROIDS[i].getPos()));
                    }
                }
            }
            if (alreadyDetected) ASTEROIDS[i].setVel(ASTEROIDS[i].getVel().getNormalized() * magnitude);
        }
    }
}


//--------------------------------------------------------------
void ofApp::updateLighting() {
    if (STATE == PLAYING) {
        if (MODE == HARD) AMBIENT_ON = false;
        
        HEADLIGHT_POS[0] = SPACESHIP->getPos().x + cos(SPACESHIP->getRotZ() * PI / 180.) * SPACESHIP->getScale() * 0.5;
        HEADLIGHT_POS[1] = SPACESHIP->getPos().y + sin(SPACESHIP->getRotZ() * PI / 180.) * SPACESHIP->getScale() * 0.5;
        HEADLIGHT_POS[2] = SPACESHIP->getPos().z;
        HEADLIGHT_POS[3] = 1.;
        
        HEADLIGHT_DIR[0] = cos(SPACESHIP->getRotZ() * PI / 180.);
        HEADLIGHT_DIR[1] = sin(SPACESHIP->getRotZ() * PI / 180.);
        HEADLIGHT_DIR[2] = 0.;
        HEADLIGHT_DIR[3] = 0.;
        
        FIRE_POS[0] = SPACESHIP->getPos().x - cos(SPACESHIP->getRotZ() * PI / 180.) * SPACESHIP->getScale();
        FIRE_POS[1] = SPACESHIP->getPos().y - sin(SPACESHIP->getRotZ() * PI / 180.) * SPACESHIP->getScale();
        FIRE_POS[2] = SPACESHIP->getPos().z;
        FIRE_POS[3] = 1.;
    } else if (STATE == WELCOME) {
        angWelcomeLight += 0.01;
        HEADLIGHT_POS[0] = (cos(angWelcomeLight) * 500);
        HEADLIGHT_POS[1] = (sin(angWelcomeLight) * 200) + 250;
        HEADLIGHT_POS[2] = 350;
        HEADLIGHT_POS[3] = 1.;
        
        HEADLIGHT_DIR[0] = 0;
        HEADLIGHT_DIR[1] = 0;
        HEADLIGHT_DIR[2] = -1.;
        HEADLIGHT_DIR[3] = 0.;
    }
}


//--------------------------------------------------------------
void ofApp::update() {
    ofSetWindowTitle("asteroids [" + ofToString(ofGetFrameRate()) + "]");
        
    long long elapsedTime = (getCurrentTimestamp() - LAST_FRAME);
    
    if (STATE == PLAYING) {
        this->updateSpaceship(elapsedTime);
        this->updatePewPewPews(elapsedTime);
    }
    
    if (UPDATE_ASTEROIDS) this->updateAsteroids(elapsedTime);
    
    this->LAST_FRAME = getCurrentTimestamp();
    
    this->updateLighting();
}


//--------------------------------------------------------------
void ofApp::drawLighting() {
    GLfloat on[4] = {1., 1., 1., 1.};
    GLfloat mid[4] = {0.4, 0.4, 0.4, 1.};
    GLfloat off[4] = {0., 0., 0., 1.};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    
    if (STATE == PLAYING) {
        if (AMBIENT_ON) {
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AMBIENT_LIGHT);
        } else {
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, off);
        }
        
        glLightfv(GL_LIGHT0, GL_POSITION, DIRECTIONAL_DIR);
        
        if (DIRECTIONAL_ON[0]) {
            glLightfv(GL_LIGHT0, GL_AMBIENT, DIRECTIONAL_AMB);
        } else {
            glLightfv(GL_LIGHT0, GL_AMBIENT, off);
        }
        if (DIRECTIONAL_ON[1]) {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, DIRECTIONAL_DIF);
        } else {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, off);
        }
        if (DIRECTIONAL_ON[2]) {
            glLightfv(GL_LIGHT0, GL_SPECULAR, DIRECTIONAL_SPE);
        } else {
            glLightfv(GL_LIGHT0, GL_SPECULAR, off);
        }
        
        if (DIRECTIONAL_ON[0] || DIRECTIONAL_ON[1] || DIRECTIONAL_ON[2]) {
            glEnable(GL_LIGHT0);
        } else {
            glDisable(GL_LIGHT0);
        }
        
        
        glLightfv(GL_LIGHT1, GL_POSITION, HEADLIGHT_POS);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, HEADLIGHT_DIR);
        
        if (HEADLIGHT_ON[0]) {
            glLightfv(GL_LIGHT1, GL_AMBIENT, HEADLIGHT_AMB);
        } else {
            glLightfv(GL_LIGHT1, GL_AMBIENT, off);
        }
        if (HEADLIGHT_ON[1]) {
            glLightfv(GL_LIGHT1, GL_DIFFUSE, HEADLIGHT_DIF);
        } else {
            glLightfv(GL_LIGHT1, GL_DIFFUSE, off);
        }
        if (HEADLIGHT_ON[2]) {
            glLightfv(GL_LIGHT1, GL_SPECULAR, HEADLIGHT_SPE);
        } else {
            glLightfv(GL_LIGHT1, GL_SPECULAR, off);
        }
        
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, HEADLIGHT_EXP);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, HEADLIGHT_CUTOFF);
        
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, HEADLIGHT_ATEN_C);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, HEADLIGHT_ATEN_L);
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, HEADLIGHT_ATEN_Q);
        
        if (HEADLIGHT_ON[0] || HEADLIGHT_ON[1] || HEADLIGHT_ON[2]) {
            glEnable(GL_LIGHT1);
        } else {
            glDisable(GL_LIGHT1);
        }
        
        
        glLightfv(GL_LIGHT2, GL_POSITION, FIRE_POS);
        
        if (FIRE_ON[0]) {
            glLightfv(GL_LIGHT2, GL_AMBIENT, FIRE_AMB);
        } else {
            glLightfv(GL_LIGHT2, GL_AMBIENT, off);
        }
        if (FIRE_ON[1]) {
            glLightfv(GL_LIGHT2, GL_DIFFUSE, FIRE_DIF);
        } else {
            glLightfv(GL_LIGHT2, GL_DIFFUSE, off);
        }
        if (FIRE_ON[2]) {
            glLightfv(GL_LIGHT2, GL_SPECULAR, FIRE_SPE);
        } else {
            glLightfv(GL_LIGHT2, GL_SPECULAR, off);
        }
        
        glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, FIRE_ATEN_C);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, FIRE_ATEN_L);
        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, FIRE_ATEN_Q);
        
        if (FIRE_ON[0] || FIRE_ON[1] || FIRE_ON[2]) {
            glEnable(GL_LIGHT2);
        } else {
            glDisable(GL_LIGHT2);
        }
    } else if (STATE == WELCOME) {
        if (AMBIENT_ON) {
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mid);
        } else {
           glLightModelfv(GL_LIGHT_MODEL_AMBIENT, off);
        }
        
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT2);
        
        glLightfv(GL_LIGHT1, GL_POSITION, HEADLIGHT_POS);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, HEADLIGHT_DIR);
        
        glLightfv(GL_LIGHT1, GL_AMBIENT, off);
        GLfloat dif[4] = {1., 1., 1., 1.};
        if (HEADLIGHT_ON[1]) {
            glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
        } else {
            glLightfv(GL_LIGHT1, GL_DIFFUSE, off);
        }
        GLfloat spec[4] = {1., 1., 1., 1.};
        if (HEADLIGHT_ON[2]) {
            glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
        } else {
            glLightfv(GL_LIGHT1, GL_SPECULAR, off);
        }
        
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 5);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60);
        
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.000000000001);
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.00000000000000000001);
            
        glEnable(GL_LIGHT1);        
    } else {
        if (AMBIENT_ON) {
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, on);
        } else {
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, off);
        }
        
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
    }
}


//--------------------------------------------------------------
void ofApp::draw() {
    if (STATE == PLAYING) {
        if (VIEW_MODE == TWO_D) {
            if (!FROM_2D) SS_PREV_Z = SPACESHIP->getPosZ();
            SPACESHIP->setPosZ(0.);
            for (int i = 0; i < PEWPEWPEWS.size(); i++) PEWPEWPEWS[i].setPosZ(0);
            
            for (int i = 0; i < ASTEROIDS.size(); i++) {
                if (!FROM_2D) ASTEROIDS_PREV_Z[i] = ASTEROIDS[i].getPosZ();
                ASTEROIDS[i].setVelZ(0.);
                ASTEROIDS[i].setPosZ(0.);
            }
            
            if (!FROM_2D) FROM_2D = true;
            
            DRAW_FIELD = false;
            
            glViewport(0., 0., ofGetWidth(), ofGetHeight());
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-ofGetWidth() * 0.5, ofGetWidth() * 0.5, -ofGetHeight() * 0.5, ofGetHeight() * 0.5, -FIELD->dim.z * 0.5, FIELD->dim.z * 0.5);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        } else if (VIEW_MODE == TWO_FIVE_D) {
            if (FROM_2D) {
                SPACESHIP->setPosZ(SS_PREV_Z);
                for (int i = 0; i < PEWPEWPEWS.size(); i++) PEWPEWPEWS[i].setPosZ(SS_PREV_Z);
                
                for (int i = 0; i < ASTEROIDS.size(); i++) {
                    ASTEROIDS[i].setPosZ(ASTEROIDS_PREV_Z[i]);
                    ASTEROIDS[i].setVelZ(((std::rand() % 25) + 10.) * (pow(-1, std::rand() % 2)));
                }
                
                FROM_2D = false;
            }
            
            DRAW_FIELD = true;
            
            glViewport(0., 0., ofGetWidth(), ofGetHeight());
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            perspective(105, 25, 100);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            ofVec2f lastDir = CAM_25D_DIR;
            if (SPACESHIP->command != DRIFT_2D  && SPACESHIP->command != DRIFT_Z) {
                CAM_25D_DIR.set(SPACESHIP->getVel().getNormalized().x, SPACESHIP->getVel().getNormalized().y);
            } else {
                CAM_25D_DIR = ofVec2f(cos(SPACESHIP->getRotZ() * PI / 180.), sin(SPACESHIP->getRotZ() * PI / 180.));
            }
            
            float angleDirs = lastDir.angle(CAM_25D_DIR);
            if (abs(angleDirs) > 0.1) {
                CAM_25D_DIR = lastDir.rotate(angleDirs * EASING_FACTOR);
            }
            
            lookAt(SPACESHIP->getPosX() - FACTOR_25D * CAM_25D_DIR.x, SPACESHIP->getPosY() - FACTOR_25D * CAM_25D_DIR.y, SPACESHIP->getPosZ() + FACTOR_25D, SPACESHIP->getPosX(), SPACESHIP->getPosY(), SPACESHIP->getPosZ(), 0., 0., 1.);
            
        } else {
            if (FROM_2D) {
                SPACESHIP->setPosZ(SS_PREV_Z);
                for (int i = 0; i < PEWPEWPEWS.size(); i++) PEWPEWPEWS[i].setPosZ(SS_PREV_Z);
                
                for (int i = 0; i < ASTEROIDS.size(); i++) {
                    ASTEROIDS[i].setPosZ(ASTEROIDS_PREV_Z[i]);
                    ASTEROIDS[i].setVelZ(((std::rand() % 25) + 10.) * (pow(-1, std::rand() % 2)));
                }
                
                FROM_2D = false;
            }
            
            DRAW_FIELD = true;
            
            glViewport(0., 0., ofGetWidth(), ofGetHeight());
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            perspective(120, 10, 1000);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            CAM_25D_DIR = ofVec2f(cos(SPACESHIP->getRotZ() * PI / 180.), sin(SPACESHIP->getRotZ() * PI / 180.));
            lookAt(SPACESHIP->getPosX(), SPACESHIP->getPosY(), SPACESHIP->getPosZ(), SPACESHIP->getPosX() + CAM_25D_DIR.x, SPACESHIP->getPosY() +  CAM_25D_DIR.y, SPACESHIP->getPosZ(), 0., 0., 1);
            
        }
        
        
        // LIGHTING
        this->drawLighting();
        
        
        // DRAW SPACESHIP
        SPACESHIP->drawSpaceship();
        
        
        // DRAW PewPewPews
        for (int i = 0; i < PEWPEWPEWS.size(); i++) PEWPEWPEWS[i].drawPewPewPew();
        
        
        // DRAW ASTEROIDS
        for (int i = 0; i < ASTEROIDS.size(); i++) {
            glPushMatrix(); {
                ASTEROIDS[i].drawAsteroid();
            } glPopMatrix();
        }
        
        
        if (LIVES == 0 && !SPACESHIP->getIsInCollision()) {
            STATE = GAME_OVER;
            STATE_START = getCurrentTimestamp();
            AMBIENT_ON = true;
            DIRECTIONAL_ON[0] = DIRECTIONAL_ON[1] = DIRECTIONAL_ON[2] = true;
            HEADLIGHT_ON[0] = HEADLIGHT_ON[1] = HEADLIGHT_ON[2] = true;
            FIRE_ON[0] = FIRE_ON[1] = FIRE_ON[2] = true;
            DIRECTIONAL_ON_GLOBAL = true;
            HEADLIGHT_ON_GLOBAL = true;
            FIRE_ON_GLOBAL = true;
        }
        
        
        
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        GLfloat intensity[4] = {0.3, 0.3, 0.3, 1.};
        
        if (!AMBIENT_ON) {
            intensity[0] = intensity[1] = intensity[2] = 0.1;
        }
        
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, intensity);
        
        // DRAW FIELD
        if (DRAW_FIELD) {
            glPushMatrix(); {
                FIELD->drawField();
            } glPopMatrix();
        }
        
        // DRAW AXIS
        if (DRAW_AXIS) {
            glPushMatrix(); {
                glScalef(35., 35., 35.);
                axis();
            } glPopMatrix();
        }
        
        
        intensity[0] = 1;
        intensity[1] = intensity[2] = 0.;
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, intensity);
        
        glViewport(0., ofGetHeight() * 0.85, ofGetWidth() * 0.15, ofGetHeight() * 0.15);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ofGetWidth() * 0.5, ofGetWidth() * 0.5, -ofGetHeight() * 0.5, ofGetHeight() * 0.5, -FIELD->dim.z * 0.5, FIELD->dim.z * 0.5);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        if (!SPACESHIP->getIsInCollision()) {
            if (LIVES == 3) {
                loadMaterial(20);
            } else {
                loadMaterial(15);
            }
            glPushMatrix(); {
                glTranslatef(350, 0, 0);
                glScalef(50, 50, 50);
                unitString("<", false);
            } glPopMatrix();
            
            if (LIVES >= 2) {
                loadMaterial(20);
            } else {
                loadMaterial(15);
            }
            glPushMatrix(); {
                glScalef(50, 50, 50);
                unitString("<", false);
            } glPopMatrix();
            
            if (LIVES >= 1) {
                loadMaterial(20);
            } else {
                loadMaterial(15);
            }
            glPushMatrix(); {
                glTranslatef(-350, 0, 0);
                glScalef(50, 50, 50);
                unitString("<", false);
            } glPopMatrix();
        }
        
        
    } else if (STATE == GAME_OVER) {
        glViewport(0., 0., ofGetWidth(), ofGetHeight());
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ofGetWidth() * 0.5, ofGetWidth() * 0.5, -ofGetHeight() * 0.5, ofGetHeight() * 0.5, -FIELD->dim.z * 0.5, FIELD->dim.z * 0.5);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        
        // LIGHTING
        this->drawLighting();
        
        
        float stateDuration = (getCurrentTimestamp() - STATE_START) / 1000.;
        
        glPushMatrix(); {
            glTranslatef(0., 0., 150.);
            glScalef(max(ofGetWidth(), ofGetHeight()) * 2, max(ofGetWidth(), ofGetHeight()) * 2, max(ofGetWidth(), ofGetHeight()) * 2);
            float angle = 360 * (stateDuration / GAME_OVER_CLEAR_DUR);
            loadMaterial(3);
            unitSector(90., angle, static_cast<int>(angle));
        } glPopMatrix();
        
        
        if ((stateDuration > GAME_OVER_CLEAR_DUR) && (stateDuration < (SCORE_DUR + GAME_OVER_CLEAR_DUR))) {
            glPushMatrix(); {
                glTranslatef(0., 0., 151.);
                glScalef(SCORE_SIZE, SCORE_SIZE, SCORE_SIZE);
                loadMaterial(16);
                unitString("SCORE: " + std::to_string(SCORE), true);
            } glPopMatrix();
        } else {
            // DRAW ASTEROIDS
            for (int i = 0; i < ASTEROIDS.size(); i++) {
                ASTEROIDS[i].setVelZ(0.);
                ASTEROIDS[i].setPosZ(0.);
            }
            
            for (int i = 0; i < ASTEROIDS.size(); i++) {
                glPushMatrix(); {
                    ASTEROIDS[i].drawAsteroid();
                } glPopMatrix();
            }
        }
        
        if (stateDuration >= SCORE_DUR + GAME_OVER_CLEAR_DUR) {
            VIEW_MODE = TWO_D;
            ASTEROIDS.clear();
            ASTEROIDS_PREV_Z.clear();
            PEWPEWPEWS.clear();
            this->setup();
        }
    } else {
        // WELCOME
        glViewport(0., 0., ofGetWidth(), ofGetHeight());
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ofGetWidth() * 0.5, ofGetWidth() * 0.5, -ofGetHeight() * 0.5, ofGetHeight() * 0.5, -FIELD->dim.z * 0.5, FIELD->dim.z * 0.5);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        
        // LIGHTING
        this->drawLighting();
        
        
        loadMaterial(21);
        glPushMatrix(); {
            glTranslatef(0., 250., 0);
            glScalef(SCORE_SIZE * 1.5, SCORE_SIZE * 1.5, SCORE_SIZE * 1.5);
            
            unitString("ASTEROIDS", false);
        } glPopMatrix();
        
        GLfloat on[4] = {1., 1., 1., 1.};
        GLfloat off[4] = {0.4, 0.4, 0.4, 1.};
        if (HOVER_CLASSIC) {
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, on);
        } else {
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, off);
        }
        
        if (!HOVER_CLASSIC) BUTTON_CLASSIC.load("classic.png"); else BUTTON_CLASSIC.load("classic_hover.png");
        glEnable(GL_TEXTURE_2D);
        BUTTON_CLASSIC.bind();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        GLfloat p = 0.5;
        glPushMatrix(); {
            glTranslatef(0., ofGetHeight() * 0.05, SCORE_SIZE * 0.75);
            glScalef(ofGetWidth() * 0.4, ofGetHeight() * 0.2, 0.);
            glBegin(GL_QUADS); {
                glNormal3f(0, 0, 1);
                glTexCoord2f(0, 0); glVertex3f(-p, p, 0);
                glTexCoord2f(0, 1); glVertex3f(-p, -p, 0);
                glTexCoord2f(1, 1); glVertex3f(p, -p, 0);
                glTexCoord2f(1, 0); glVertex3f(p, p, 0);
            } glEnd();
        } glPopMatrix();
        
        BUTTON_CLASSIC.unbind();
        glDisable(GL_TEXTURE_2D);
        BUTTON_CLASSIC.clear();
        
        if (HOVER_HARD) {
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, on);
        } else {
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, off);
        }
        
        if (!HOVER_HARD) BUTTON_HARD.load("hard.png"); else BUTTON_HARD.load("hard_hover.png");
        glEnable(GL_TEXTURE_2D);
        BUTTON_HARD.bind();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glPushMatrix(); {
            glTranslatef(0., -ofGetHeight() * 0.2, SCORE_SIZE * 0.75);
            glScalef(ofGetWidth() * 0.4, ofGetHeight() * 0.2, 0.);
            glBegin(GL_QUADS); {
                glNormal3f(0, 0, 1);
                glTexCoord2f(0, 0); glVertex3f(-p, p, 0);
                glTexCoord2f(0, 1); glVertex3f(-p, -p, 0);
                glTexCoord2f(1, 1); glVertex3f(p, -p, 0);
                glTexCoord2f(1, 0); glVertex3f(p, p, 0);
            } glEnd();
        } glPopMatrix();
        
        BUTTON_HARD.unbind();
        glDisable(GL_TEXTURE_2D);
        BUTTON_HARD.clear();
    }
}


//--------------------------------------------------------------
void ofApp::exit() {
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
        case 'l':
            // GL_LINE <-> GL_FILL
            GLint currentPolygonMode[2];
            glGetIntegerv(GL_POLYGON_MODE, currentPolygonMode);
            if (currentPolygonMode[0] == GL_FILL) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            
            break;
            
            
        case 'v':
            VIEW_MODE = static_cast<ViewMode>((VIEW_MODE + 1) % 3);
            break;
            
            
        case 'w':
            ACTIVATED_KEYS[W] = true;
            break;
            
        case 'a':
            ACTIVATED_KEYS[A] = true;
            break;
            
        case 's':
            ACTIVATED_KEYS[S] = true;
            break;
            
        case 'd':
            ACTIVATED_KEYS[D] = true;
            break;
            
        case OF_KEY_LEFT_SHIFT:
            ACTIVATED_KEYS[LSHIFT] = true;
            break;
            
        case ' ':
            ACTIVATED_KEYS[SPACE] = true;
            break;
            
        case OF_KEY_RETURN:
            if (STATE == PLAYING && !SPACESHIP->getIsInCollision()) {
                PEWPEWPEWS.push_back(PewPewPew(SPACESHIP->getPos(), 500, SPACESHIP->getRot(), 3.));
                
                if (SOUND.load("pew.mp3")) {
                    SOUND.play();
                } else {
                    cout << "'pew' sound could not be loaded :(" << endl;
                }
            }
            break;
            
        case 'c':
            CULLING += 1;
            if (CULLING > 3) CULLING = 0;
            
            // CULLING
            switch (CULLING) {
                case 0:
                    if (glIsEnabled(GL_CULL_FACE)) glDisable(GL_CULL_FACE);
                    cout << "NO CULLING" << endl;
                    break;
                    
                case 1:
                    if (!glIsEnabled(GL_CULL_FACE)) glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    cout << "CULL - BACK" << endl;
                    break;
                    
                case 2:
                    if (!glIsEnabled(GL_CULL_FACE)) glEnable(GL_CULL_FACE);
                    glCullFace(GL_FRONT);
                    cout << "CULL - FRONT" << endl;
                    break;
                    
                case 3:
                    if (!glIsEnabled(GL_CULL_FACE)) glEnable(GL_CULL_FACE);
                    glCullFace(GL_FRONT_AND_BACK);
                    cout << "CULL - FRONT & BACK" << endl;
                    break;
            }
            
            break;
            
        case 'x':
            DRAW_AXIS = !DRAW_AXIS;
            break;
            
        case '\\':
            UPDATE_ASTEROIDS = !UPDATE_ASTEROIDS;
            break;
            
            
        // LIGHTING
        case '0':
            AMBIENT_ON = !AMBIENT_ON;
            break;
            
        case '1':
        case '2':
        case '3':
            DIRECTIONAL_ON[key - '1'] = !DIRECTIONAL_ON[key - '1'];
            if (DIRECTIONAL_ON[0] == DIRECTIONAL_ON[1] == DIRECTIONAL_ON[2] == false) {
                DIRECTIONAL_ON_GLOBAL = false;
            } else if (DIRECTIONAL_ON[0] == DIRECTIONAL_ON[1] == DIRECTIONAL_ON[2] == true) {
                DIRECTIONAL_ON_GLOBAL = true;
            }
            
            break;
            
        case '4':
        case '5':
        case '6':
            HEADLIGHT_ON[key - '4'] = !HEADLIGHT_ON[key - '4'];
            if (HEADLIGHT_ON[0] == HEADLIGHT_ON[1] == HEADLIGHT_ON[2] == false) {
                HEADLIGHT_ON_GLOBAL = false;
            } else if (HEADLIGHT_ON[0] == HEADLIGHT_ON[1] == HEADLIGHT_ON[2] == true) {
                HEADLIGHT_ON_GLOBAL = true;
            }
            break;
            
        case '7':
        case '8':
        case '9':
            FIRE_ON[key - '7'] = !FIRE_ON[key - '7'];
            if (FIRE_ON[0] == FIRE_ON[1] == FIRE_ON[2] == false) {
                FIRE_ON_GLOBAL = false;
            } else if (FIRE_ON[0] == FIRE_ON[1] == FIRE_ON[2] == true) {
                FIRE_ON_GLOBAL = true;
            }
            
            break;
            
        case 'i':
            DIRECTIONAL_ON_GLOBAL = !DIRECTIONAL_ON_GLOBAL;
            DIRECTIONAL_ON[0] = DIRECTIONAL_ON[1] = DIRECTIONAL_ON[2] = DIRECTIONAL_ON_GLOBAL;
            break;
            
        case 'o':
            HEADLIGHT_ON_GLOBAL = !HEADLIGHT_ON_GLOBAL;
            HEADLIGHT_ON[0] = HEADLIGHT_ON[1] = HEADLIGHT_ON[2] = HEADLIGHT_ON_GLOBAL;
            break;
            
        case 'p':
            FIRE_ON_GLOBAL = !FIRE_ON_GLOBAL;
            FIRE_ON[0] = FIRE_ON[1] = FIRE_ON[2] = FIRE_ON_GLOBAL;
            break;
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
        case 'w':
            ACTIVATED_KEYS[W] = false;
            break;
            
        case 'a':
            ACTIVATED_KEYS[A] = false;
            break;
            
        case 's':
            ACTIVATED_KEYS[S] = false;
            break;
            
        case 'd':
            ACTIVATED_KEYS[D] = false;
            break;
            
        case OF_KEY_LEFT_SHIFT:
            ACTIVATED_KEYS[LSHIFT] = false;
            break;
            
        case ' ':
            ACTIVATED_KEYS[SPACE] = false;
            break;
    }
}


void ofApp::mouseMoved(int x, int y) {
    if (STATE == WELCOME) {
        float centerX = ofGetWidth() / 2.0;
        float centerY = ofGetHeight() / 2.0;

        float rectWidth = ofGetWidth() * 0.4;
        float rectHeight = ofGetHeight() * 0.2;

        float classicXMin = centerX - rectWidth / 2;
        float classicXMax = centerX + rectWidth / 2;
        float classicYMin = centerY - ofGetHeight() * 0.05 - rectHeight / 2;
        float classicYMax = centerY - ofGetHeight() * 0.05 + rectHeight / 2;

        float hardXMin = centerX - rectWidth / 2;
        float hardXMax = centerX + rectWidth / 2;
        float hardYMin = centerY + ofGetHeight() * 0.2 - rectHeight / 2;
        float hardYMax = centerY + ofGetHeight() * 0.2 + rectHeight / 2;

        if (x > classicXMin && x < classicXMax && y > classicYMin && y < classicYMax) {
            HOVER_CLASSIC = true;
            HOVER_HARD = false;
        } else if (x > hardXMin && x < hardXMax && y > hardYMin && y < hardYMax) {
            HOVER_HARD = true;
            HOVER_CLASSIC = false;
        } else {
            HOVER_CLASSIC = HOVER_HARD = false;
        }
    }
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    if (STATE == WELCOME) {
        float centerX = ofGetWidth() / 2.0;
        float centerY = ofGetHeight() / 2.0;

        float rectWidth = ofGetWidth() * 0.4;
        float rectHeight = ofGetHeight() * 0.2;

        float classicXMin = centerX - rectWidth / 2;
        float classicXMax = centerX + rectWidth / 2;
        float classicYMin = centerY - ofGetHeight() * 0.05 - rectHeight / 2;
        float classicYMax = centerY - ofGetHeight() * 0.05 + rectHeight / 2;

        float hardXMin = centerX - rectWidth / 2;
        float hardXMax = centerX + rectWidth / 2;
        float hardYMin = centerY + ofGetHeight() * 0.2 - rectHeight / 2;
        float hardYMax = centerY + ofGetHeight() * 0.2 + rectHeight / 2;

        if (x > classicXMin && x < classicXMax && y > classicYMin && y < classicYMax) {
            STATE = PLAYING;
            MODE = CLASSIC;
            cout << "CLASSIC" << endl;
        } else if (x > hardXMin && x < hardXMax && y > hardYMin && y < hardYMax) {
            STATE = PLAYING;
            MODE = HARD;
            cout << "HARD" << endl;
        }

        if (STATE == PLAYING) {
            if (SOUND.load("start.mp3")) {
                SOUND.play();
            } else {
                cout << "'start' sound could not be loaded :(" << endl;
            }

            MIN_ASTEROIDS = (MODE == CLASSIC) ? 15 : 35;
        }
    }
}



//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}


//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {

}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}


//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}


//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
