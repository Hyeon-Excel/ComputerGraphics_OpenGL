#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/freeglut.h>
#include "camera.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_STARS 1000

Camera camera;
GLuint earthTexture;   // ���� �ؽ�ó
GLuint moonTexture;    // �� �ؽ�ó
GLuint venusTexture;   // �ݼ� �ؽ�ó
GLuint marsTexture;    // ȭ�� �ؽ�ó
GLuint jupiterTexture; // �� �ؽ�ó

bool keyPressed[256] = { false };

// ������ ��� ���� ����
GLenum polygonMode = GL_FILL; // �⺻ �ָ���

// �������� ��� ���� ����
bool usePerspective = true;  // true�� ���� ����, false�� ���� ����

// �� �༺���� ���� ����
static float Day = 0;           // ���� ���� ����
static float EarthRotation = 0; // ���� ���� ����
static float MoonOrbit = 0;     // �� ���� ����
static float MoonRotation = 0;  // �� ���� ����
static float MercuryOrbit = 0;
static float VenusOrbit = 0;
static float MarsOrbit = 0;
static float JupiterOrbit = 0;
static float SaturnOrbit = 0;
static float UranusOrbit = 0;
static float NeptuneOrbit = 0;

float stars[NUM_STARS][3];
float starBrightness[NUM_STARS];

void InitStars() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < NUM_STARS; i++) {
        stars[i][0] = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;
        stars[i][1] = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;
        stars[i][2] = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;
        starBrightness[i] = ((float)rand() / RAND_MAX) * 0.5f + 0.5f;
    }
}

void LoadTexture(const char *filename, GLuint *textureID) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (data) {
        glGenTextures(1, textureID);
        glBindTexture(GL_TEXTURE_2D, *textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);
    }
    else {
        printf("Failed to load texture: %s\n", filename);
    }
}

void DrawStars() {
    glDisable(GL_LIGHTING);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_STARS; i++) {
        glColor3f(starBrightness[i], starBrightness[i], starBrightness[i]);
        glVertex3f(stars[i][0], stars[i][1], stars[i][2]);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void DrawScene() {
    // �¾�
    glPushMatrix();
    GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT0);

    GLfloat sun_emission[] = { 1.0, 0.5, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
    glutSolidSphere(0.2, 40, 40);
    glPopMatrix();

    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    // �˵� ���
    float merc_incl = 7.0f;
    float ven_incl = 3.39f;
    // Earth : 0��
    float mar_incl = 1.85f;
    float jup_incl = 1.3f;
    float sat_incl = 2.5f;
    float ura_incl = 0.77f;
    float nep_incl = 1.77f;

    // ����(Mercury)
    {
        GLfloat mercury_diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
        glPushMatrix();
        glRotatef(merc_incl, 1.0, 0.0, 0.0);
        glRotatef(MercuryOrbit, 0.0, 1.0, 0.0);
        glTranslatef(0.4, 0.0, 0.0);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mercury_diffuse);
        glutSolidSphere(0.03, 20, 20);
        glPopMatrix();
    }

    // �ݼ�(Venus)
    {
        glPushMatrix();
        glRotatef(ven_incl, 1.0, 0.0, 0.0);
        glRotatef(VenusOrbit, 0.0, 1.0, 0.0);
        glTranslatef(0.6, 0.0, 0.0);
        glRotatef(-90, 1.0, 0.0, 0.0);

        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, venusTexture);
        GLUquadric *venus_quad = gluNewQuadric();
        gluQuadricTexture(venus_quad, GL_TRUE);
        gluSphere(venus_quad, 0.05, 30, 30);
        gluDeleteQuadric(venus_quad);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    // ����(Earth)
    {
        glPushMatrix();
        glRotatef(Day, 0.0, 1.0, 0.0);
        glTranslatef(1.0, 0.0, 0.0);
        glRotatef(EarthRotation, 0.0, 1.0, 0.0);
        glRotatef(-90, 1.0, 0.0, 0.0);

        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        GLUquadric *earth_quad = gluNewQuadric();
        gluQuadricTexture(earth_quad, GL_TRUE);
        gluSphere(earth_quad, 0.1, 40, 40);
        gluDeleteQuadric(earth_quad);
        glDisable(GL_TEXTURE_2D);

        // ��(Moon)
        glRotatef(MoonOrbit, 0.0, 1.0, 0.0);
        glTranslatef(0.2, 0.0, 0.0);

        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        GLUquadric *moon_quad = gluNewQuadric();
        gluQuadricTexture(moon_quad, GL_TRUE);
        gluSphere(moon_quad, 0.03, 20, 20);
        gluDeleteQuadric(moon_quad);
        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }

    // ȭ��(Mars)
    {
        glPushMatrix();
        glRotatef(mar_incl, 1.0, 0.0, 0.0);
        glRotatef(MarsOrbit, 0.0, 1.0, 0.0);
        glTranslatef(1.5, 0.0, 0.0);

        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, marsTexture);
        GLUquadric *mars_quad = gluNewQuadric();
        gluQuadricTexture(mars_quad, GL_TRUE);
        glRotatef(-90, 1.0, 0.0, 0.0);
        gluSphere(mars_quad, 0.07, 30, 30);
        gluDeleteQuadric(mars_quad);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    // ��(Jupiter)
    {
        glPushMatrix();
        glRotatef(jup_incl, 1.0, 0.0, 0.0);
        glRotatef(JupiterOrbit, 0.0, 1.0, 0.0);
        glTranslatef(2.5, 0.0, 0.0);

        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, jupiterTexture);
        GLUquadric *jupiter_quad = gluNewQuadric();
        gluQuadricTexture(jupiter_quad, GL_TRUE);
        glRotatef(-90, 1.0, 0.0, 0.0);
        gluSphere(jupiter_quad, 0.15, 40, 40);
        gluDeleteQuadric(jupiter_quad);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    // �伺(Saturn)
    {
        GLfloat saturn_diffuse[] = { 0.9f, 0.8f, 0.5f, 1.0f };
        glPushMatrix();
        glRotatef(sat_incl, 1.0, 0.0, 0.0);
        glRotatef(SaturnOrbit, 0.0, 1.0, 0.0);
        glTranslatef(3.5, 0.0, 0.0);

        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, saturn_diffuse);
        glutSolidSphere(0.12, 40, 40);

        glPushMatrix();
        glRotatef(90, 1.0, 0.0, 0.0);
        glutSolidTorus(0.02, 0.2, 20, 50);
        glPopMatrix();
        glPopMatrix();
    }

    // õ�ռ�(Uranus)
    {
        GLfloat uranus_diffuse[] = { 0.5f, 0.8f, 0.9f, 1.0f };
        glPushMatrix();
        glRotatef(ura_incl, 1.0, 0.0, 0.0);
        glRotatef(UranusOrbit, 0.0, 1.0, 0.0);
        glTranslatef(4.5, 0.0, 0.0);

        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, uranus_diffuse);
        glutSolidSphere(0.1, 40, 40);
        glPopMatrix();
    }

    // �ؿռ�(Neptune)
    {
        GLfloat neptune_diffuse[] = { 0.2f, 0.2f, 0.7f, 1.0f };
        glPushMatrix();
        glRotatef(nep_incl, 1.0, 0.0, 0.0);
        glRotatef(NeptuneOrbit, 0.0, 1.0, 0.0);
        glTranslatef(5.5, 0.0, 0.0);

        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, neptune_diffuse);
        glutSolidSphere(0.1, 40, 40);
        glPopMatrix();
    }
}

void SetProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (usePerspective) {
        // ���� ����
        gluPerspective(60.0, 1.0, 1.0, 20.0);
    }
    else {
        // ���� ����
        // ��� ũ�⿡ ���߾� �ణ ũ�� ����
        glOrtho(-3.0, 3.0, -3.0, 3.0, 0.1, 20.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // ������ ������ ��� ����
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    SetProjection(); // ���� ��� ����

    camera.setModelViewMatrix();
    DrawStars();
    DrawScene();
    glutSwapBuffers();
}

void Idle() {
    float slideSpeed = 0.05f;
    float rotationSpeed = 0.2f;

    // ī�޶� �̵�/ȸ��
    if (keyPressed['d']) camera.slide(slideSpeed, 0, 0);
    if (keyPressed['a']) camera.slide(-slideSpeed, 0, 0);
    if (keyPressed['s']) camera.slide(0, 0, slideSpeed);
    if (keyPressed['w']) camera.slide(0, 0, -slideSpeed);
    if (keyPressed['q']) camera.yaw(-rotationSpeed);
    if (keyPressed['e']) camera.yaw(rotationSpeed);
    if (keyPressed['i']) camera.pitch(rotationSpeed);
    if (keyPressed['k']) camera.pitch(-rotationSpeed);
    if (keyPressed['j']) camera.roll(rotationSpeed);
    if (keyPressed['l']) camera.roll(-rotationSpeed);

    // ���� �ӵ� (����: 0.1��/frame)
    float earthOrbitSpeed = 0.1f;
    float mercurySpeed = 0.1f / 0.240846f; // �� 0.415
    float venusSpeed = 0.1f / 0.615f;    // �� 0.163
    float marsSpeed = 0.1f / 1.881f;    // �� 0.053
    float jupiterSpeed = 0.1f / 11.862f;   // �� 0.0084
    float saturnSpeed = 0.1f / 29.457f;   // �� 0.0034
    float uranusSpeed = 0.1f / 84.016f;   // �� 0.00119
    float neptuneSpeed = 0.1f / 164.8f;    // �� 0.00061

    Day += earthOrbitSpeed;
    EarthRotation += 0.5f;
    MoonOrbit += 0.5f;
    MoonRotation += 0.2f;

    MercuryOrbit += mercurySpeed;
    VenusOrbit += venusSpeed;
    MarsOrbit += marsSpeed;
    JupiterOrbit += jupiterSpeed;
    SaturnOrbit += saturnSpeed;
    UranusOrbit += uranusSpeed;
    NeptuneOrbit += neptuneSpeed;

    float *angles[] = { &Day, &EarthRotation, &MoonOrbit, &MoonRotation,
                       &MercuryOrbit, &VenusOrbit, &MarsOrbit, &JupiterOrbit,
                       &SaturnOrbit, &UranusOrbit, &NeptuneOrbit };

    int count = sizeof(angles) / sizeof(angles[0]);
    for (int i = 0; i < count; i++) {
        if (*angles[i] >= 360.0f) *angles[i] -= 360.0f;
    }

    glutPostRedisplay();
}

void KeyDown(unsigned char key, int x, int y) {
    keyPressed[key] = true;

    // 1Ű: ���̾�������
    if (key == '1') {
        polygonMode = GL_LINE;
    }
    // 2Ű: �ָ���
    else if (key == '2') {
        polygonMode = GL_FILL;
    }
    // pŰ: ���� ����
    else if (key == 'p') {
        usePerspective = true;
    }
    // oŰ: ���� ����
    else if (key == 'o') {
        usePerspective = false;
    }
}

void KeyUp(unsigned char key, int x, int y) {
    keyPressed[key] = false;
}

void Init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    InitStars();
    camera.setDefaultCamera();

    // �ؽ�ó �ε� (���ϸ��� ȯ�濡 �°�)
    LoadTexture("earth.jpg", &earthTexture);
    LoadTexture("moon.jpg", &moonTexture);
    LoadTexture("venus.jpg", &venusTexture);
    LoadTexture("mars.jpg", &marsTexture);
    LoadTexture("jupiter.jpg", &jupiterTexture);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Solar System with Moon, Projection Toggle");

    Init();

    // �ʱ� ���� ����
    SetProjection();

    glutDisplayFunc(MyDisplay);
    glutIdleFunc(Idle);
    glutKeyboardFunc(KeyDown);
    glutKeyboardUpFunc(KeyUp);
    glutMainLoop();

    return 0;
}
