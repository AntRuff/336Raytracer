# 336Raytracer
Anthony Ruffalo aruffalo
Brady Solomon bsolomon

To complie the project, run the Makefile with the make command. Then run the command ./main to run the program. 
This works on pyrite and should produce an image in output.ppm.

The image size and aspect ratio can be adjusted by changing the width variable in the main function to set the number of pixels wide the image should be and the aspectRatio variable to set the aspect ratio (image height will be automatically calculated based on this)
The camera location is defined by the eye variable and the camera target is defined by the target variable. The vup variable defines the direction that will be treated as 'up'. Changin it will roll the camera (i.e. changing vup to vec3(1,0,0) will tilt the image 90 degrees to the left). Make sure that this vector is not perfectly parallel to the direction the camera is viewing the scene from (i.e. setting the eye to vec3(0,-10,0) and the target to (0,-9,0) and vup to vec3(0,1,0) will result in an incorrect image).
To edit the scene, sampling rate, or to add objects to the scene, please navigate to the main function.
To add a sphere: use shape* name = new sphere(vec3 origin, float radius, vec3 color, float matKey, float refractionIndex); To add this to the shape list for collision detection, please call shapeList.push_front(name);
To add a triangle: use shape* name = new triangle(vec3 pointA, vec3 pointB, vec3 pointC, vec3 color, float matKey, float refractionIndex); To add this to the shape list for collision detection, please call shapeList.push_front(name);
  Note that because triangles are 2D shapes, only one side of the triangle will properly bounce rays.
To adjust the material properties of an object, edit the matKey upon object creation. As of right now, 1.0 means diffuser and 2.0 means specular.
To create a dialetric object, change the matKey to 3.0. Then, you can adjust the refraction index to change how the object refracts.
To adjust the sampling rate or recursive depth, edit the samples or the depth variables respectively.
