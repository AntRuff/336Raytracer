#include "vec.h"
#include <iostream>
#include <list>
#include <iterator>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <cstdint>
#include <cfloat>

using namespace std;

#define R 0
#define G 1
#define B 2
class shape;
class material;

class hit {
public:
    float dist;
    vec3 loc;
    vec3 norm;
    material* mptr;
    hit() {

    }
    hit(float dist,  vec3 loc, vec3 norm, material* mptr) {
        this->dist = dist;
        this->loc = loc;
        this->norm = norm;
        this->mptr = mptr;
    }
};

vec3 rand_vec3() {
    int x = rand() / (RAND_MAX + 1.0);
    int y = rand() / (RAND_MAX + 1.0);
    int z = rand() / (RAND_MAX + 1.0);
    return vec3(x, y, z);
}

vec3 rand_vec3(float min, float max) {
    return (min + (max - min)) * rand_vec3();
}

vec3 rand_unit_vec3() {
    vec3 rvec; // random vector
    while (1) {
        rvec = rand_vec3(-1, 1);
        if (rvec.length() <= 1) break;
    }
    return rvec / rvec.length(); //random unit vector
}

class shape {

public:
    material* mat;

    virtual hit getHit(vec3 rayStart, vec3 rayEnd) = 0;
};

class sphere : public shape{

public:

    vec3 origin;
    float radius;


    sphere(vec3 o, float r, material* m){
        origin = o;
        radius = r;
        mat = m;
    }

    hit getHit(vec3 rayStart, vec3 rayEnd) override {
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
            return hit(tempD, temp, temp - origin, mat);
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
            if (tempPD < tempND) {
                return hit(tempPD, tempP, tempP - origin, mat);
            }
            return hit(tempND, tempN, tempN - origin, mat);
        }
        return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), mat);
    }
};

class triangle : public shape {
public:
    vec3 t0;
    vec3 t1;
    vec3 t2;

    triangle (vec3 a, vec3 b, vec3 c, material* m){
        t0 = a;
        t1 = b;
        t2 = c;
        mat = m;
    }

    hit getHit(vec3 rayStart, vec3 rayEnd) override {
        vec3 edge0 = t1-t0;
        vec3 edge1 = t2-t1;
        vec3 edge2 = t0-t2;
        vec3 normal = edge0.cross((t2-t0));
        normal = normal / normal.length();
        vec3 dir = rayEnd-rayStart;

        if (normal.dot(dir) == 0) return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), mat);

        float dis = normal.dot(t0);
        float t = ((normal.dot(rayStart) + dis)/ normal.dot(dir));
        if (t < 0) return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), mat);
        vec3 p = rayStart + t * dir;
        vec3 c0 = p - t0;
        vec3 c1 = p - t1;
        vec3 c2 = p - t2;

        float distance = sqrt(pow(p.x()-rayStart.x(), 2) + pow(p.y()-rayStart.y(), 2) + pow(p.z()-rayStart.z(), 2));

        if (normal.dot(edge0.cross(c0)) >= 0 && normal.dot(edge1.cross(c1)) >= 0 && normal.dot(edge2.cross(c2)) >= 0) {
            return hit(distance, p, normal, mat);
        }
        else return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), mat);
    }
};
hit Raycast(vec3 rayStart, vec3 rayEnd, list<shape*> shapeList);

class material {
public:
    vec3 color;
    int key;
    virtual vec3 reflection(vec3 p, vec3 normal) = 0;
};

class lambertian : public material {
public:
    vec3 color;
    int key = 1;
    lambertian(vec3 color) {
        this->color = color;
    }
    vec3 reflection(vec3 p, vec3 normal) override {
        return normal + rand_unit_vec3();
    }
};
//class specular : public material {
//public:
//    vec3 reflection(vec3 p, vec3 normal, int depth, list<shape*> shapeList) override {
//
//    }
//};

hit Raycast(vec3 rayStart, vec3 rayEnd, list<shape*> shapeList){

    vec3 closestPoint = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    float closestD = sqrt(pow(closestPoint.x() - rayStart.x(), 2) +
                          pow(closestPoint.y() - rayStart.y(), 2) +
                          pow(closestPoint.z() - rayStart.z(), 2));
//    vec3 color = vec3(50, 50, 50);

    hit h = hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), nullptr);

    for(shape* s : shapeList){
        hit tempH = s->getHit(rayStart, rayEnd);
        float d = tempH.dist;

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
    float f = 0.1;
//    vec3 color = vec3(1, 1, 1);
    if (depth <= 0) return vec3(0, 0, 0);
    hit next = Raycast(p, dir, shapeList);
    if (next.dist > 0) {
        vec3 ref;
        if (next.mptr->key == 1) {
            vec3 ref = next.norm + rand_unit_vec3();
        }
//        vec3 ref = next.mptr->reflection(next.loc, next.norm);
        return f * next.mptr->color + (1 - f) * get_color(next.loc, ref, depth - 1, shapeList);
    }
    return vec3(0.3, 0.3, 0.3);
}

//vec3 getFirstColor(vec3 rayStart, vec3 rayEnd, list<shape*> shapeList) {
//
//}

int main(int argc, char *agrv[]) {

    const int xBound = 512;
    const int yBound = 384;
    int samples = 16;
    int depth = 16;

    vec3 eye = vec3(xBound/2, yBound/2, 0);
    vec3 imagePlane = vec3(0, 0, 100);


    uint8_t raster[yBound][xBound][3];

    list <shape*> shapeList;

    material* lamBlue = new lambertian(vec3(0.5, 0.5, 1));
    material* lamGreen = new lambertian(vec3(0.5, 1, 0.5));
    material* lamRed = new lambertian(vec3(1, 0.5, 0.5));

    shape* s1 = new sphere(vec3(xBound/2, yBound-100, 200), 100, lamBlue);
    shape* s2 = new sphere(vec3(xBound/2+100, yBound-150, 300), 150, lamRed);
    shape* t1 = new triangle(vec3(xBound/2, yBound, 0), vec3(xBound/2-10000, yBound, 1000), vec3(xBound/2+10000, yBound, 1000), lamGreen);

//    s1;
//    s2;
//    t1;
    shapeList.push_front(s1);
    shapeList.push_front(s2);
    shapeList.push_front(t1);

    int x, y, s;

    FILE *f;

    for (y = 0; y < yBound; y++){
        for (x = 0; x < xBound; x++){
            vec3 color = vec3(0, 0, 0);
            if (x == 269 && y == 200) {
                printf("");
            }
            for (s = 0; s < samples; s++) {
                float randX = rand() / (RAND_MAX + 1.0);
                float randY = rand() / (RAND_MAX + 1.0);
//                hit h = Raycast(eye, vec3((float) x + randX, (float) y + randY, imagePlane.z()), shapeList);
//                material *m = h.mptr;
                vec3 tempcolor = get_color(eye, vec3((float) x + randX, (float) y + randY, imagePlane.z()), depth, shapeList) * 255;
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
