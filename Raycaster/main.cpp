#include "vec.h"
#include <iostream>
#include <list>
#include <iterator>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <cstdint>

using namespace std;

#define R 0
#define G 1
#define B 2


class shape {
public:
    vec3 color;

    virtual float hit(vec3 rayStart, vec3 rayEnd) { return 0;}
};

class sphere : public shape{

public:

    vec3 origin;
    float radius;


    sphere(vec3 o, float r, vec3 rgb){
        origin = o;
        radius = r;
        color = rgb;
    }

    float hit(vec3 rayStart, vec3 rayEnd) override {
        float a = pow((rayEnd.x() - rayStart.x()), 2)
                  + pow((rayEnd.y() - rayStart.y()), 2)
                  + pow((rayEnd.z() - rayStart.z()), 2);
        float b = 2 * (((rayEnd.x() - rayStart.x())*(rayStart.x()-origin.x()))
                       +((rayEnd.y() - rayStart.y())*(rayStart.y()-origin.y()))
                       +((rayEnd.z() - rayStart.z())*(rayStart.z()-origin.z())));
        float c = pow(origin.x(), 2) + pow(origin.y(), 2) + pow(origin.z(), 2)
                  + pow(rayStart.x(), 2) + pow(rayStart.y(), 2) + pow(rayStart.z(), 2)
                  - 2 * ((origin.x()*rayStart.x()) + (origin.y()*rayStart.y()) + (origin.z()*rayStart.z()))
                  - pow(radius, 2);

        float intersect = (b * b) - (4 * a * c);

        if (intersect < 0.0){
        } else if (intersect == 0.0){
            float u = (-b/(2*a));
            vec3 temp = vec3(rayStart.x() + u*(rayEnd.x() - rayStart.x()),
                             rayStart.y() + u*(rayEnd.y() - rayStart.y()),
                             rayStart.z() + u*(rayEnd.z() - rayStart.z()));
            float tempD = sqrt(pow(temp.x() - rayStart.x(), 2) +
                               pow(temp.y() - rayStart.y(), 2) +
                               pow(temp.z() - rayStart.z(), 2));
            return tempD;
        } else {
            float uP = (-b + sqrt(intersect))/(2*a);
            float uN = (-b - sqrt(intersect))/(2*a);
            vec3 tempP = vec3(rayStart.x() + uP*(rayEnd.x() - rayStart.x()),
                              rayStart.y() + uP*(rayEnd.y() - rayStart.y()),
                              rayStart.z() + uP*(rayEnd.z() - rayStart.z()));
            vec3 tempN = vec3(rayStart.x() + uN*(rayEnd.x() - rayStart.x()),
                              rayStart.y() + uN*(rayEnd.y() - rayStart.y()),
                              rayStart.z() + uN*(rayEnd.z() - rayStart.z()));
            float tempPD = sqrt(pow(tempP.x() - rayStart.x(), 2) +
                                pow(tempP.y() - rayStart.y(), 2) +
                                pow(tempP.z() - rayStart.z(), 2));
            float tempND = sqrt(pow(tempN.x() - rayStart.x(), 2) +
                                pow(tempN.y() - rayStart.y(), 2) +
                                pow(tempN.z() - rayStart.z(), 2));
            if (tempPD > tempND) {
                return tempPD;
            }
            return tempND;
        }
    }
};

class triangle : public shape {
public:
    vec3 t0;
    vec3 t1;
    vec3 t2;

    triangle (vec3 a, vec3 b, vec3 c, vec3 rgb){
        t0 = a;
        t1 = b;
        t2 = c;
        color = rgb;
    }

    float hit(vec3 rayStart, vec3 rayEnd) override {
        vec3 temp = t1-t0;
        vec3 normal = temp.cross((t2-t0));
        vec3 dir = rayEnd-rayStart;
        float dis = normal.dot(t0);
        return - ((normal.dot(rayStart) + dis)/ normal.dot(dir));
    }
};

vec3 Raycast(vec3 rayStart, vec3 rayEnd, list<shape> shapeList){

    vec3 closestPoint = rayEnd;
    float closestD = sqrt(pow(closestPoint.x() - rayStart.x(), 2) +
                          pow(closestPoint.y() - rayStart.y(), 2) +
                          pow(closestPoint.z() - rayStart.z(), 2));
    vec3 color = vec3(50, 50, 50);

    for(shape s : shapeList){
        float d = s.hit(rayStart, rayEnd);
        if (d < closestD){
            closestD = d;
            color = s.color;
        }
    }
}

int main(int argc, char *agrv[]) {

    const int xBound = 512;
    const int yBound = 384;
    vec3 eye = vec3(xBound/2, yBound/2, 0);
    vec3 imagePlane = vec3(0, 0, 100);


    uint8_t raster[yBound][xBound][3];

    list <shape> shapeList;

    shapeList.push_front(sphere(vec3(xBound/2, yBound/2, 1), 0.5, vec3(0, 0, 255)));
    shapeList.push_front(sphere(vec3(xBound/2+0.3, yBound/2-0.3, 01.1), 0.4, vec3(0, 255, 0)));
    shapeList.push_front(triangle(vec3(0, 1, 1), vec3(1, 0, 1), vec3(1, 1, 0), vec3(255, 0, 0)));


    int x, y;

    FILE *f;

    for (y = 0; y < yBound; y++){
        for (x = 0; x < xBound; x++){
            vec3 color = Raycast(eye, vec3((float) x, (float) y, imagePlane.z()), shapeList);
            raster[y][x][R] = (uint8_t) color.x();
            raster[y][x][G] = (uint8_t) color.y();
            raster[y][x][B] = (uint8_t) color.z();

        }
    }

    f = fopen("output.ppm", "w");

    fprintf(f, "P6\n%d\n%d\n255\n", xBound, yBound);
    fwrite(raster, 1, sizeof(raster), f);

    fclose(f);

    return 0;

}
