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
    double refractIndex;
    hit() {

    }
    hit(double dist,  vec3 loc, vec3 norm, vec3 color, double matKey, double refractIndex) {
        this->dist = dist;
        this->loc = loc;
        this->norm = norm;
        this->color = color;
        this->matKey = matKey;
        this->refractIndex = refractIndex;
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
    double x = rand() / (RAND_MAX + 1.0);
    double y = rand() / (RAND_MAX + 1.0);
    double z = rand() / (RAND_MAX + 1.0);
    x = min + x * (max - min);
    y = min + y * (max - min);
    z = min + z * (max - min);

    return vec3(x, y, z);
}

vec3 rand_unit_vec3() {
    vec3 rvec;
    while (1) {
        rvec = rand_vec3(-1, 1);
        if (rvec.length() <= 1) break;
    }
    return rvec / rvec.length(); //random unit vector
}

class shape {

public:
    vec3 color;
    double matKey;
    double refractIndex;

    virtual hit getHit(vec3 rayStart, vec3 rayEnd) = 0;
};

class sphere : public shape{

public:

    vec3 origin;
    double radius;


    sphere(vec3 o, double r, vec3 c, double mk, double rI){
        origin = o;
        radius = r;
        color = c;
        matKey = mk;
        refractIndex = rI;
    }

    hit getHit(vec3 rayStart, vec3 rayEnd) override {
        vec3 dir = rayEnd - rayStart;
        vec3 startToCenter = rayStart - origin;
        double a = dir.dot(dir);
        double b = 2 * dir.dot(startToCenter);
        double c = startToCenter.dot(startToCenter) - (radius * radius);

        double disc = (b * b) - (4 * a * c);

        if (disc < 0.0){
        } else {

            double t = (-b - sqrt(disc)) / (2. * a);
            vec3 r = rayStart + (t * dir);
            vec3 n = r - origin;
            n = n / n.length();

            return hit(t, r, n, color, matKey, refractIndex);
        }
        return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), color, matKey, refractIndex);
    }
};

class triangle : public shape {
public:
    vec3 t0;
    vec3 t1;
    vec3 t2;

    triangle (vec3 a, vec3 b, vec3 c, vec3 color, double mk, double rI){
        t0 = a;
        t1 = b;
        t2 = c;
        this->color = color;
        matKey = mk;
        refractIndex = rI;
    }

    hit getHit(vec3 rayStart, vec3 rayEnd) override {
        vec3 edge0 = t1-t0;
        vec3 edge1 = t2-t1;
        vec3 edge2 = t0-t2;
        vec3 normal = edge0.cross((t2-t0));
        normal = normal / normal.length();
        vec3 dir = rayEnd-rayStart;
        dir = dir / dir.length();

        double denom = normal.dot(dir);
        if (abs(denom) < 0.000001) return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), color, matKey, 0);


        double t = ((normal.dot((t0 - rayStart))) / denom);
        if (t < 0) return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), color, matKey, 0);
        vec3 p = rayStart + t * dir;
        vec3 c0 = p - t0;
        vec3 c1 = p - t1;
        vec3 c2 = p - t2;

        if (normal.dot(edge0.cross(c0)) >= 0 && normal.dot(edge1.cross(c1)) >= 0 && normal.dot(edge2.cross(c2)) >= 0) {
            return hit(t, p, normal, color, matKey, refractIndex);
        }
        else return hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), color, matKey, refractIndex);
    }
};

hit Raycast(vec3 rayStart, vec3 rayEnd, list<shape*> shapeList){

    vec3 closestPoint = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    double closestD = sqrt(pow(closestPoint.x() - rayStart.x(), 2) +
                           pow(closestPoint.y() - rayStart.y(), 2) +
                           pow(closestPoint.z() - rayStart.z(), 2));

    hit h = hit(-1, vec3(-1, -1, -1), vec3(-1, -1, -1), vec3(-1, -1, -1), -1, 0);

    for(shape* s : shapeList){
        hit tempH = s->getHit(rayStart, rayEnd);
        double d = tempH.dist;

        if (d <= 0.0000001) {

        }
        else if ((rayStart.x() - rayEnd.x() >= 0 && rayStart.x() - tempH.loc.x() < 0) ||
                 (rayStart.x() - rayEnd.x() <= 0 && rayStart.x() - tempH.loc.x() > 0) ||
                 (rayStart.y() - rayEnd.y() >= 0 && rayStart.y() - tempH.loc.y() < 0) ||
                 (rayStart.y() - rayEnd.y() <= 0 && rayStart.y() - tempH.loc.y() > 0) ||
                 (rayStart.z() - rayEnd.z() >= 0 && rayStart.z() - tempH.loc.z() < 0) ||
                 (rayStart.z() - rayEnd.z() <= 0 && rayStart.z() - tempH.loc.z() > 0)) {

        }
        else if (d < closestD){
            closestD = d;
            h = tempH ;
        }
    }
    return h;
}

vec3 refract(vec3 ray, vec3 norm, double rI){
    double a = fmin((-ray).dot(norm), 1.0);
    vec3 perpendicular = rI * (ray + a*norm);
    vec3 parallel = -sqrt(fabs(1.0 - perpendicular.dot(perpendicular)))*norm;
    return perpendicular + parallel;
}


vec3 get_color(vec3 p, vec3 nxt, int depth, list<shape*> shapeList) {
    double f = 0.5;
    if (depth <= 0) return vec3(0, 0, 0);
    hit next = Raycast(p, nxt, shapeList);
    if (next.dist > 0) {
        if (next.color.r() == 0 && next.color.g() == 0 && next.color.b() == 0) return next.color;
        vec3 ref;
        double key = next.matKey;
        if (key == 1.0) { //lambertian
            vec3 r = rand_unit_vec3();
            //ref = next.norm + r + next.loc;
            ref = next.norm + r;
        }
        else if (key == 2.0) { //specular
            ref = next.loc - 2 * next.norm * next.loc.dot(next.norm);
        }
        else if (key == 3.0) { //Dialectrics
            double refractRatio;
            if (nxt.dot(next.norm) > 0.0) {
                refractRatio = next.refractIndex;
                next.norm = -next.norm;
            }
            else {
                refractRatio = 1.0/next.refractIndex;
            }

            vec3 unit = nxt/nxt.length();

            double cos = fmin((-unit).dot(next.norm), 1.0);
            double sin = sqrt(1.0 - cos*cos);
            double r0 = pow((1-refractRatio)/(1+refractRatio), 2);

            double reflectance = r0 + (1-r0)*pow((1-cos), 5); 

            if ((refractRatio * sin > 1.0) || reflectance > rand_double()){
                ref = next.loc - 2 * next.norm * next.loc.dot(next.norm);
            } else {
                ref = refract(unit, next.norm, refractRatio);
            }
        }
        return next.color * get_color(next.loc, ref, depth - 1, shapeList);
    }
    return vec3(0.5, 1, 1); //sky
}

int main(int argc, char *agrv[]) {

    const int xBound = 1600;
    const int yBound = 1200;
    int samples = 16;
    int depth = 16;

    vec3 eye = vec3(0, 0, 0);
    vec3 imagePlane = vec3(2, 1.5, 1);
    vec3 topleft = vec3(-1, -0.75, 1);


    uint8_t raster[yBound][xBound][3];

    list <shape*> shapeList;

    shape* s1 = new sphere(vec3(-0.75, 0.25, 2), 0.5, vec3(0., 0., 1), 1, 1);
    shape* s2 = new sphere(vec3(0.75, 0.25, 2), 0.5, vec3(1, 0., 0.), 1, 1);
    shape* s3 = new sphere(vec3(0, -1, 2), 0.5, vec3(0, 1 , 0), 1, 1);
    shape* s4 = new sphere(vec3(1.5, -0.5, 4), 1, vec3(.75, .75, .75), 2, 1);
    shape* s5 = new sphere(vec3(0, -0.25, 2.5), .5, vec3(1, 1, 1), 3, 4);
    shape* t1 = new triangle(vec3(0, 0.75, 0), vec3(10, 0.75, 10), vec3(-10, 0.75, 10), vec3(0.5, 0.5, 0.5), 1, 1);

    shapeList.push_front(s1);
    shapeList.push_front(s2);
    shapeList.push_front(s3);
    shapeList.push_front(s4);
    shapeList.push_front(s5);
    shapeList.push_front(t1);

    int x, y, s;

    FILE *f;

    for (y = 0; y < yBound; y++){
        for (x = 0; x < xBound; x++){
            vec3 color = vec3(0, 0, 0);
            if (x == 150 && y == 280) {
                printf("");
            }
            for (s = 0; s < samples; s++) {
                double randX = rand() / (RAND_MAX + 1.0);
                double randY = rand() / (RAND_MAX + 1.0);
                vec3 tempdir = vec3((imagePlane.x() / xBound) * ((double) x + randX) + topleft.x(), (imagePlane.y() / yBound) * ((double) y + randY) + topleft.y(), imagePlane.z());
                vec3 tempcolor = get_color(eye, tempdir, depth, shapeList) * 255;
                color += tempcolor;
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
