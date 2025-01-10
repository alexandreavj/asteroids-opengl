//
//  elements.hpp
//  asteroids
//
//  Created by Alexandre Jacob on 23/11/2024.
//

#ifndef elements_hpp
#define elements_hpp

#include <stdio.h>
#include <map>
#include "ofMain.h"


extern map<char, vector<vector<int>>> pixelArt;


void unitSquare();

void unitCube();

void unitSphere(int m, int n);

void unitSector(float startAng, float ang, int segments);

void unitString(string text, bool texture);

void axis();


#endif /* elements_hpp */
