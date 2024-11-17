#include <GL/freeglut.h>
#include <stdlib.h>

float rtri = 0.0f;

float tx = 0.0f, ty = 0.0f, tz = 0.0f;
float rx = 0.0f, ry = 0.0f, rz = 0.0f;
float scale = 1.0f;

const float SCALE_MIN = 0.1f;
const float SCALE_MAX = 5.0f;

int mouseButton = -1;
int mouseOldX, mouseOldY;

void InitGL() {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void DrawGLScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0f, 0.0f, 5.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);

    glTranslatef(tx, ty, tz);
    glRotatef(rx, 1.0f, 0.0f, 0.0f);
    glRotatef(ry, 0.0f, 1.0f, 0.0f);
    glRotatef(rz, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);

    glRotatef(rtri, 0.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    rtri += 0.2f;

    glutSwapBuffers();
}

void ReSizeGLScene(int Width, int Height) {
    if (Height == 0) Height = 1;
    glViewport(0, 0, Width, Height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MouseButton(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_RIGHT_BUTTON) {
            mouseButton = 2;
        }
    }
    else {
        if (button == GLUT_RIGHT_BUTTON) {
            mouseButton = -1;
        }
    }
    mouseOldX = x;
    mouseOldY = y;
}

void MouseMotion(int x, int y) {
    int dx = x - mouseOldX;
    int dy = y - mouseOldY;

    if (mouseButton == 2) {
        ry += dx * 0.5f;
        rx += dy * 0.5f;
    }

    mouseOldX = x;
    mouseOldY = y;

    glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': ty += 0.1f; break;
    case 's': ty -= 0.1f; break;
    case 'a': tx -= 0.1f; break;
    case 'd': tx += 0.1f; break;

    case 'q':
        scale += 0.1f;
        if (scale > SCALE_MAX) scale = SCALE_MAX;
        break;
    case 'e':
        scale -= 0.1f;
        if (scale < SCALE_MIN) scale = SCALE_MIN;
        break;

    case 27: exit(0); break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pyramid 3D Shape");

    InitGL();
    glutDisplayFunc(DrawGLScene);
    glutIdleFunc(DrawGLScene);
    glutReshapeFunc(ReSizeGLScene);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();

    return 1;
}
