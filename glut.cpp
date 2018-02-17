#include "glut.h"

#include <GLFW/glfw3.h>

#include <iostream>

#include <GL/glut.h>

void renderScene(void) {

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
//    glBegin(GL_TRIANGLES);
//        glVertex3f(-0.5,-0.5,0.0);
//        glVertex3f(0.5,0.0,0.0);
//        glVertex3f(0.0,0.5,0.0);
//    glEnd();

//    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;

    glutInit(&argc, argv);

    glutCreateWindow("Window");

    glutDisplayFunc(renderScene);

    glutMainLoop();

    return 0;
}

