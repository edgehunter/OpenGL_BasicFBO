A program to demonstrate drawing to a FBO (framebuffer object) in OpenGL.

The program

1. Draws a red square onto a green background into a FBO. The backing store for the FBO is a 2D texture.

2. Draw a square onto a blue screen but map the texture from step 1 to the square. This maps the entire scene from the previous step onto the square. 