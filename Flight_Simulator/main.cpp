#include <stdio.h>
#include <math.h>
#include <vector>
#include "terrain.h"
#include "skybox.h"
#include <GL/freeglut.h>
#include "stb_image.h"
#include "missile.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")

std::vector<Missile> missiles;

// 객체 선언
Terrain *terrain;   // 지형
Skybox *skybox;     // 스카이 박스
Camera cam;         // 카메라

bool keyPressed[256]; // 키보드 입력 상태

// 함수 선언
void fireMissile();
void myKeyboard(unsigned char key, int x, int y);
void myKeyboardUp(unsigned char key, int x, int y);
void dispose();
void Display();
void Idle();
void Reshape(GLint w, GLint h);
void fog();

void fireMissile() {
    Vector3 direction = cam.n;
    direction.flip(); // 카메라의 방향을 따라 미사일 발사
    missiles.emplace_back(cam.eye, direction, 10.0f);
}

void updateMissiles(float deltaTime) {
    for (auto &missile : missiles) {
        missile.updatePosition(deltaTime, terrain); // 충돌 처리 포함
    }
}


void drawMissiles() {
    for (auto &missile : missiles) {
        missile.draw();
    }
}

void myKeyboard(unsigned char key, int x, int y) {
    keyPressed[key] = true;
    if (key == 'f') { // 'f' 키로 미사일 발사
        fireMissile();
    }
}

void myKeyboardUp(unsigned char key, int x, int y) {
    keyPressed[key] = false;
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
    skybox->draw();
    glPopMatrix();

    terrain->RenderTerrain(cam.eye.x, cam.eye.z);
    drawMissiles(); // 미사일 그리기
    fog();
    glFlush();
    glutSwapBuffers();
}

void Idle() {
    if (keyPressed['1']) glPolygonMode(GL_FRONT, GL_LINE);
    if (keyPressed['2']) glPolygonMode(GL_FRONT, GL_FILL);
    if (keyPressed['d']) { cam.slide(0.2, 0, 0); }
    if (keyPressed['a']) { cam.slide(-0.2, 0, 0); }
    if (keyPressed['s']) { cam.slide(0, 0, 1.0); }
    if (keyPressed['w']) { cam.slide(0, 0, -1.0); }
    if (cam.eye.y < terrain->getHeight(cam.eye.x, cam.eye.z)) {
        cam.slide(0, 1.0, 0);
    }
    if (keyPressed['k']) { cam.pitch(-0.5); }
    if (keyPressed['i']) { cam.pitch(0.5); }
    if (keyPressed['q']) { cam.yaw(-0.5); }
    if (keyPressed['e']) { cam.yaw(0.5); }
    if (keyPressed['j']) { cam.roll(0.5); }
    if (keyPressed['l']) { cam.roll(-0.5); }

    float deltaTime = 0.016f; // 프레임 시간
    updateMissiles(deltaTime);
    glutPostRedisplay();
}

void Reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, static_cast<GLdouble>(w / h), 1.0, 3000.0);
}

void dispose() {
    delete terrain;
    delete skybox;
}

void fog() {
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    GLfloat waterFogColor[4] = { 0.0, 0.6, 0.6, 5.0 };
    GLfloat fogColor[4] = { 0.75, 0.75, 0.75, 0.0 };
    if (cam.eye.y < (terrain->waterLevel - 75)) {
        glFogfv(GL_FOG_COLOR, waterFogColor);
        glFogf(GL_FOG_DENSITY, 0.075);
    }
    else {
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.001);
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(400, 30);
    glutCreateWindow("CGproject_final");
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutKeyboardFunc(myKeyboard);
    glutKeyboardUpFunc(myKeyboardUp);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    try {
        terrain = new Terrain("space/heightmap.png", "space/terrain.bmp", 257, 257);
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    skybox = new Skybox();
    cam.set(4, 500, 4, 0, 0, 0, 0, 1, 0);
    cam.setShape(60.0f, 64.0f / 48.0f, 0.5f, 1000.0f);
    cam.slide(0, 100, 0);
    cam.roll(0);
    cam.yaw(180);
    cam.pitch(45);

    atexit(dispose);
    printf("W: forward S: backward A: left D: right\n");
    printf("JL: roll\n");
    printf("IK: pitch\n");
    printf("QE: yaw\n\n");
    printf("1.2: solid, wire rendering\n");
    glutMainLoop();
    return 0;
}
