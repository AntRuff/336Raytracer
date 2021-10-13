# 336Raytracer
Anthony Ruffalo aruffalo
Brady Solomon bsolomon

To complie the project, run the Makefile with the make command. Then run the command ./main to run the program. 
This works on pyrite and should produce an image in output.ppm.

The camera is placed at (0, 0, 0) with an image plane at (2, 1.5, 1). The scene for placing shapes is between (-1, -.75) and (1, .75) with any z value greater than 0 being visable from the camera.
To edit the scene, sampling rate, or to add objects to the scene, please navigate to the main function.
To add a sphere: use shape* name = new sphere(vec3 origin, float radius, vec3 color, float matKey); To add this to the shape list for collision detection, please call shapeList.push_front(name);
To add a triangle: use shape* name = new triangle(vec3 pointA, vec3 pointB, vec3 pointC, vec3 color, floar matKey); To add this to the shape list for collision detection, please call shapeList.push_front(name);
To adjust the material properties of an object, edit the matKey upon object creation. As of right now, 1.0 means diffuser and 2.0 means specular.
To adjust the sampling rate or recursive depth, edit the samples or the depth variables respectively.