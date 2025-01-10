//
//  easing.cpp
//  asteroids
//
//  Created by Alexandre Jacob on 18/11/2024.
//

#include "easing.hpp"


float easingInOutQuad(float time_perc) {
    return time_perc < 0.5 ? 2 * time_perc * time_perc : time_perc * (4 - 2 * time_perc) - 1;
}
