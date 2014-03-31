#ifndef UTIL_H
#define UTIL_H

struct Orientation {

public:
    Orientation(float x_theta=0, float y_theta=0, float z_theta=0) :
        x_theta(x_theta), y_theta(y_theta), z_theta(z_theta) {}

    float x_theta;
	float y_theta;
    float z_theta;
};

#endif