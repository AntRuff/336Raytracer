#include "vec.h"
#include <iostream>
#include <list>
#include <iterator>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <cstdint>
#include <cfloat>
#include <random>

using namespace std;

#define R 0
#define G 1
#define B 2
class shape;
class material;

class hit {
public:
    double dist;
    vec3 loc;
    vec3 norm;
    vec3 color;
    double matKey;
    hit() {

    }
    hit(double dist,  vec3 loc, vec3 norm, vec3 color, double matKey) {
        this->dist = dist;
        this->loc = loc;
        this->norm = norm;
        this->color = color;
        this->matKey = matKey;
    }
};

double rand_double() {
    uniform_real_distribution<double> dist(0., 1.);
    mt19937 gen;
    return dist(gen);
}

vec3 rand_vec3() {
    double x = rand() / (RAND_MAX + 1.0);
    double y = rand() / (RAND_MAX + 1.0);
    double z = rand() / (RAND_MAX + 1.0);
    return vec3(x, y, z);
}

vec3 rand_vec3(double min, double max) {
    return (min + (max - min)) * rand_vec3();
}

vec3 rand_unit_vec3() {
    vec3* rvec; // random vector
    while (1) {
        vec3 t = rand_vec3(-1, 1);
        rvec = &t;
        if (rvec->length() <= 1) break;
    }
    return *rvec / rvec->length(); //random unit vector
}

class shape {

public:
    vec3 color;
    double matKey;

    virtual hit getHit(vec3 rayStart, vec3 rayEnd) = 0;
};

class sphere : public shape{

public:

    vec3 origin;
    double radius;


    sphere(vec3 o, double r, vec3 c, double mk){
        origin = o;
        radius = r;
        color = c;
        matKey = mk;
    }

    hit getHit(vec3 rayStart, vec3 rayEnd) override {
        double a = pow((rayEnd.x() - rayStart.x()), 2)
                  + pow((rayEnd.y() - rayStart.y()), 2)
                  + pow((rayEnd.z() - rayStart.z()), 2);
        double b = 2 * (((rayEnd.x() - rayStart.x())*(rayStart.x()-origin.x()))
                       +((rayEnd.y() - rayStart.y())*(rayStart.y()-origin.y()))
                       +((rayEnd.z() - rayStart.z())*(rayStart.z()-origin.z())));
        double c = pow(origin.x(), 2) + pow(origin.y(), 2) + pow(origin.z(), 2)
                  + pow(rayStart.x(), 2) + pow(rayStart.y(), 2) + pow(rayStart.z(), 2)
                  - 2 * ((origin.x()*rayStart.x()) + (origin.y()*rayStart.y()) + (origin.z()*rayStart.z()))
                  - pow(radius, 2);

        double intersect = (b * b) - (4 * a * c);

        if (intersect < 0.0){
        } else if (intersect == 0.0){
            double u = (-b/(2*a));
            vec3 temp = vec3(rayStart.x() + u*(rayEnd.x() - rayStart.x()),
                             rayStart.y() + u*(rayEnd.y() - rayStart.y()),
                             rayStart.z() + u*(rayEnd.z() - rayStart.z()));
            double tempD = sqrt(pow(temp.x() - rayStart.x(), 2) +
                               pow(temp.y() - rayStart.y(), 2) +
                               pow(temp.z() - rayStart.z(), 2));
            return hit(tempD, temp, temp - origin, color, matKey);
        } else {
            double uP = (-b + sqrt(intersect))/(2*a);
            double uN = (-b - sqrt(intersect))/(2*a);
            vec3 tempP = vec3(rayStart.x() + uP*(rayEnd.x() - rayStart.x()),
                              rayStart.y() + uP*(rayEnd.y() - rayStart.y()),
                              rayStart.z() + uP*(rayEnd.z() - rayStart.z()));
            vec3 tempN = vec3(rayStart.x() + uN*(rayEnd.x() - rayStart.x()),
                              rayStart.y() + uN*(rayEnd.y() - rayStart.y()),
                              rayStart.z() + uN*(rayEnd.z() - rayStart.z()));
            double tempPD = sqrt(pow(tempP.x() - rayStart.x(), 2) +
                                pow(tempP.y() - rayStart.y(), 2) +
                                pow(tempP.z() - rayStart.z(), 2));
            double tempND = sqrt(pow(tempN.x() - rayStart.x(), 2) +
                                pow(tempN.y() - rayStart.y(), 2) +
                                pow(tempN.z() - rayStart.z(), 2));
            if (tempPD < tempND) {
                return hit(tempPD, tempP, tempP - origin, color, matKey);
            }
            return hit(tempND, tempN, tempN - origin, color, matKey);
        }
        return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), color, matKey);
    }
};

class triangle : public shape {
public:
    vec3 t0;
    vec3 t1;
    vec3 t2;

    triangle (vec3 a, vec3 b, vec3 c, vec3 color, double mk){
        t0 = a;
        t1 = b;
        t2 = c;
        this->color = color;
        matKey = mk;
    }

    hit getHit(vec3 rayStart, vec3 rayEnd) override {
        vec3 edge0 = t1-t0;
        vec3 edge1 = t2-t1;
        vec3 edge2 = t0-t2;
        vec3 normal = edge0.cross((t2-t0));
        normal = normal / normal.length();
        vec3 dir = rayEnd-rayStart;

        if (normal.dot(dir) == 0) return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), color, matKey);

        double dis = normal.dot(t0);
        double t = ((normal.dot(rayStart) + dis)/ normal.dot(dir));
        if (t < 0) return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), color, matKey);
        vec3 p = rayStart + t * dir;
        vec3 c0 = p - t0;
        vec3 c1 = p - t1;
        vec3 c2 = p - t2;

        double distance = sqrt(pow(p.x()-rayStart.x(), 2) + pow(p.y()-rayStart.y(), 2) + pow(p.z()-rayStart.z(), 2));

        if (normal.dot(edge0.cross(c0)) >= 0 && normal.dot(edge1.cross(c1)) >= 0 && normal.dot(edge2.cross(c2)) >= 0) {
            return hit(distance, p, normal, color, matKey);
        }
        else return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), color, matKey);
    }
};
hit Raycast(vec3 rayStart, vec3 rayEnd, list<shape*> shapeList);

hit Raycast(vec3 rayStart, vec3 rayEnd, list<shape*> shapeList){

    vec3 closestPoint = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    double closestD = sqrt(pow(closestPoint.x() - rayStart.x(), 2) +
                          pow(closestPoint.y() - rayStart.y(), 2) +
                          pow(closestPoint.z() - rayStart.z(), 2));
//    vec3 color = vec3(50, 50, 50);

    hit h = hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), vec3(-1, -1, -1), -1);

    for(shape* s : shapeList){
        hit tempH = s->getHit(rayStart, rayEnd);
        double d = tempH.dist;

        if (d == -1) {

        }
        else if (d < closestD){
            closestD = d;
            h = tempH ;
//            color = s->color;
        }
    }
    return h;
}

vec3 get_color(vec3 p, vec3 dir, int depth, list<shape*> shapeList) {
    double f = 0.5;
//    vec3 color = vec3(1, 1, 1);
    if (depth <= 0) return vec3(0, 0, 0);
    hit next = Raycast(p, dir, shapeList);
    if (next.dist > 0) {
        vec3 ref;
        double key = next.matKey;
        if (key == 1.0) {
            vec3 r = rand_unit_vec3();
            ref = next.norm + r + next.loc;
            printf("");
        }
        else if (key == 2.0) {
            ref = dir - 2 * dir.dot(next.norm) * next.norm;
        }
//        vec3 ref = next.mptr->reflection(next.loc, next.norm);
        return f * next.color + (1 - f) * get_color(next.loc, ref, depth - 1, shapeList);
    }
    return vec3(0.53, 0.81, 0.92);
}

//vec3 getFirstColor(vec3 rayStart, vec3 rayEnd, list<shape*> shapeList) {
//
//}

int main(int argc, char *agrv[]) {

    const int xBound = 512;
    const int yBound = 384;
    int samples = 64;
    int depth = 64;

    vec3 eye = vec3(xBound/2, yBound/2, 0);
    vec3 imagePlane = vec3(0, 0, 100);


    uint8_t raster[yBound][xBound][3];

    list <shape*> shapeList;

    shape* s1 = new sphere(vec3(xBound/2.0, yBound-55, 125), 25, vec3(0.5, 0.5, 1), 1);
    shape* s2 = new sphere(vec3(xBound/2.0, yBound-130, 250), 100, vec3(1, 0.5, 0.5), 1);
    shape* s3 = new sphere(vec3(xBound/2.0 + 200, yBound-130, 250), 100, vec3(1, 1, 1), 2);
    shape* t1 = new triangle(vec3(xBound/2, yBound - 30, 0), vec3(xBound/2-10000, yBound - 30, 1000), vec3(xBound/2+10000, yBound - 30, 1000), vec3(0.5, 1, 0.5), 1);
    shape* t2 = new triangle(vec3(xBound/2, 30, 300), vec3(xBound/2-200, yBound-50, 300), vec3(xBound/2+200, yBound-50, 300), vec3(0.5, 1, 0.5), 1);

//    s1;
//    s2;
//    t1;
    shapeList.push_front(s1);
    shapeList.push_front(s2);
    shapeList.push_front(s3);
    shapeList.push_front(t1);
    shapeList.push_front(t2);

    int x, y, s;

    FILE *f;

    for (y = 0; y < yBound; y++){
        for (x = 0; x < xBound; x++){
            vec3 color = vec3(0, 0, 0);
            if (x == 360 && y == 260) {
                printf("");
            }
            for (s = 0; s < samples; s++) {
                double randX = rand() / (RAND_MAX + 1.0);
                double randY = rand() / (RAND_MAX + 1.0);
//                hit h = Raycast(eye, vec3((double) x + randX, (double) y + randY, imagePlane.z()), shapeList);
//                material *m = h.mptr;
                vec3 tempcolor = get_color(eye, vec3((double) x + randX, (double) y + randY, imagePlane.z()), depth, shapeList) * 255;
                color += tempcolor;
//                color += h.s->mat->reflection(h.loc, h.norm, depth, shapeList);
            }
            color /= samples;
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
