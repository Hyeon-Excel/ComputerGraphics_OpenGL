#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#define TIME_INTERVAL 0.016f

float gravity = -9.8f;
float maxHeight = 5.0f;   
float ballY = 5.0f;
float ballVelocity = 0.0f;
int wireframe = 0;

float scaleX = 1.0f;
float scaleY = 1.0f;
int scalingDown = 0;
float deformationSpeed = 0.02f;  

float speedMultiplier = 1.0f;      
float speedMultiplierDecay = 0.5f; 

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.33, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawGround() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, -0.05f, 0.0f); 
    glScalef(10.0f, 0.1f, 10.0f);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawBall() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, ballY, 0.0f);
    glScalef(scaleX, scaleY, 1.0f);

    if (wireframe)
        glutWireSphere(0.5, 20, 20);
    else
        glutSolidSphere(0.5, 20, 20);

    glPopMatrix();
}

void update(int value) {
    if (speedMultiplier > 1.0f) {
        speedMultiplier -= speedMultiplierDecay * TIME_INTERVAL;
        if (speedMultiplier < 1.0f)
            speedMultiplier = 1.0f;
    }

    ballVelocity += gravity * TIME_INTERVAL * speedMultiplier;
    ballY += ballVelocity * TIME_INTERVAL * speedMultiplier;

    if (ballY <= 0.5f) {
        ballY = 0.5f;

        ballVelocity = sqrt(2 * fabs(gravity) * maxHeight);

        scalingDown = 1; 

        deformationSpeed = 0.02f * speedMultiplier;
    }

    if (scalingDown) {
        scaleX += deformationSpeed;
        scaleY -= deformationSpeed;
        if (scaleY <= 0.8f) {
            scalingDown = 0;
        }
    }
    else {
        if (scaleX > 1.0f) {
            scaleX -= deformationSpeed;
            scaleY += deformationSpeed;
            if (scaleX < 1.0f) {
                scaleX = 1.0f;
                scaleY = 1.0f;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc((int)(TIME_INTERVAL * 1000), update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        wireframe = 1;
        break;
    case 's':
        wireframe = 0;
        break;
    case '+':
        speedMultiplier += 1.0f;  
        if (speedMultiplier > 5.0f)
            speedMultiplier = 5.0f;  
        break;
    case 27:
        exit(0);
        break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    gluLookAt(0.0, 5.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawGround();
    drawBall();
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    ballVelocity = sqrt(2 * fabs(gravity) * maxHeight);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Project II : Bouncing Ball");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc((int)(TIME_INTERVAL * 1000), update, 0);

    glutMainLoop();
    return 0;
}