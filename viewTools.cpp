#include "viewTools.hpp"
#include "tools.hpp"


void perspective(GLfloat theta, GLfloat alpha, GLfloat beta) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat tan = tanf(theta*0.5 * PI / 180.0);
    GLfloat d = (ofGetHeight() / 2.0) / tan;
    GLfloat nearClip = d / alpha;
    GLfloat farClip = d * beta;
    GLfloat ymax = nearClip * tan;
    GLfloat xmax = (ofGetWidth() / ofGetHeight()) * ymax;
    glFrustum(-xmax, xmax, -ymax, ymax, nearClip, farClip);
}


void lookAt(
    GLfloat camX,
    GLfloat camY,
    GLfloat camZ,
    GLfloat targetX,
    GLfloat targetY,
    GLfloat targetZ,
    GLfloat upX,
    GLfloat upY,
    GLfloat upZ) {
    
    ofVec3f cam = ofVec3f(camX, camY, camZ);
    ofVec3f target = ofVec3f(targetX, targetY, targetZ);
    ofVec3f up = ofVec3f(upX, upY, upZ);

    ofVec3f N = cam - target;
    N = N.getNormalized();
    ofVec3f U = crossProduct(up, N);
    U = U.getNormalized();
    ofVec3f V = crossProduct(N, U);
    V = V.getNormalized();

    GLfloat camTransformMatrix[4][4] = {
        {U.x, V.x, N.x, 0},
        {U.y, V.y, N.y, 0},
        {U.z, V.z, N.z, 0},
        {-U.dot(cam), -V.dot(cam), -N.dot(cam), 1}
    };

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(&camTransformMatrix[0][0]);
}
