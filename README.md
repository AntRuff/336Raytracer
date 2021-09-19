# 336Raytracer
Anthony Ruffalo aruffalo

Brady Solomon bsolomon

To run the project, run the Makefile with the make command. This works on pyrite and should produce an image in output.ppm.

This Raytracer draws rays from an eye centered in the screen to a plain 1 unit into the screen. If it intersects with a sphere, then it will paint that pixel the color of the sphere, otherwise that pixel will be painted the background color. If multiple spheres are intersected, then the closest sphere to the eye will determine the color of the pixel. The background of the image will be purple, while there are three spheres colored red, blue, and green. To determine if the ray intersects with the sphere, we calculated the distance between the ray and the center of the circle, and if that was less than the radius of the sphere then we knew we hit. It will also support any number of spheres drawn.
