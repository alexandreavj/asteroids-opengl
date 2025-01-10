#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "ofMain.h"
#include "viewTools.hpp"
#include "Field.hpp"
#include "Spaceship.hpp"
#include "Asteroid.hpp"
#include "PewPewPew.hpp"
#include "materials.hpp"


#define FACTOR_25D 350
#define INITIAL_LIVES 3
#define GAME_OVER_CLEAR_DUR 3.
#define SCORE_DUR 5.
#define SCORE_SIZE 10


enum State {
    WELCOME,
    PLAYING,
    GAME_OVER
};


enum Mode {
    CLASSIC,
    HARD
};


enum ViewMode {
    TWO_D,
    TWO_FIVE_D,
    FIRST_PERSON
};


enum Key {
    W,
    A,
    S,
    D,
    LSHIFT,
    SPACE
};


class ofApp : public ofBaseApp {
public:
    void setup() override;
    void updateSpaceship(long long elapsedTime);
    void updatePewPewPews(long long elapsedTime);
    void updateAsteroids(long long elapsedTime);
    void updateLighting();
    void update() override;
    void drawLighting();
    void draw() override;
    void exit() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y ) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    
private:
    int MIN_ASTEROIDS = 15;
    
    ofSoundPlayer SOUND;
    
    State STATE;
    long long STATE_START;
    
    ViewMode VIEW_MODE = TWO_D;
    bool DRAW_FIELD = false;
    Field *FIELD = nullptr;
    bool FROM_2D;
    ofVec2f CAM_25D_DIR = ofVec2f(1., 0.);
    float EASING_FACTOR = 0.01;
    
    Spaceship *SPACESHIP = nullptr;
    vector<PewPewPew> PEWPEWPEWS;
    GLfloat SS_PREV_Z;
    long long LAST_FRAME;
    map<Key, bool> ACTIVATED_KEYS = {{W, false}, {A, false}, {S, false}, {D, false}};
    
    vector<Asteroid> ASTEROIDS;
    vector<GLfloat> ASTEROIDS_PREV_Z;
    
    int LIVES = INITIAL_LIVES;
    
    int SCORE = 0;
    
    bool DRAW_AXIS = false;
    
    short CULLING = 0;
    
    bool UPDATE_ASTEROIDS = true;
    
    
    // LIGHTING
    bool AMBIENT_ON = true;
    GLfloat AMBIENT_LIGHT[4] = {0.9, 0.9, 0.9, 1.};

    bool DIRECTIONAL_ON_GLOBAL = true;
    bool DIRECTIONAL_ON[3] = {true, true, true};
    GLfloat DIRECTIONAL_DIR[4] = {0., 0., 1., 0.};
    GLfloat DIRECTIONAL_AMB[4] = {0.1, 0.1, 0.1, 1.};
    GLfloat DIRECTIONAL_DIF[4] = {0.4, 0.4, 0.4, 1.};
    GLfloat DIRECTIONAL_SPE[4] = {0.2, 0.2, 0.2, 1.};

    bool HEADLIGHT_ON_GLOBAL = true;
    bool HEADLIGHT_ON[3] = {true, true, true};
    GLfloat HEADLIGHT_POS[4];
    GLfloat HEADLIGHT_DIR[4];
    GLfloat HEADLIGHT_AMB[4] = {0., 0., 0., 1.};
    GLfloat HEADLIGHT_DIF[4] = {0.6, 0.6, 0.6, 1.};
    GLfloat HEADLIGHT_SPE[4] = {0.3, 0.3, 0.3, 1.};
    GLfloat HEADLIGHT_EXP = 5;
    GLfloat HEADLIGHT_CUTOFF = 90;
    GLfloat HEADLIGHT_ATEN_C = 1.;
    GLfloat HEADLIGHT_ATEN_L = 0.0000000001;
    GLfloat HEADLIGHT_ATEN_Q = 0.000000000001;
    
    bool FIRE_ON_GLOBAL = true;
    bool FIRE_ON[3] = {true, true, true};
    GLfloat FIRE_POS[4];
    GLfloat FIRE_AMB[4] = {0., 0., 0., 1.};
    GLfloat FIRE_DIF[4] = {0.4, 0.2, 0., 1.};
    GLfloat FIRE_SPE[4] = {0.25, 0.125, 0., 1.};
    GLfloat FIRE_ANG;
    GLfloat FIRE_ATEN_C = 1.;
    GLfloat FIRE_ATEN_L = 0.001;
    GLfloat FIRE_ATEN_Q = 0.0001;
    
    Mode MODE = CLASSIC;
    
    bool HOVER_CLASSIC = false, HOVER_HARD = false;
    ofImage BUTTON_CLASSIC, BUTTON_HARD;
    
    float angWelcomeLight = 0;
};
