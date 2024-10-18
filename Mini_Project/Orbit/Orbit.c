#include <GL/freeglut.h>
#include <stdlib.h> // rand() �Լ��� ���� �߰�
#include <time.h>   // time() �Լ��� ���� �߰�
#include <math.h>   // fmod �Լ��� ����ϱ� ���� �߰�
#define NUM_STARS 1000

static float Day = 0, Time = 0;
float stars[NUM_STARS][3];
float starBrightness[NUM_STARS];

void InitStars() {
    srand(time(NULL)); // ���� �õ� ����
    for (int i = 0; i < NUM_STARS; i++) {
        // X ~ Z ��� ���� + Y ���� ����
        stars[i][0] = ((float)rand() / RAND_MAX) * 10.0f - 5.0f; // X��ǥ: -5.0 ~ 5.0
        stars[i][1] = -((float)rand() / RAND_MAX) * 10.0f - 1.0f; // Y��ǥ: -1.0 ~ -11.0
        stars[i][2] = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;  // Z��ǥ: -5.0 ~ 5.0

        starBrightness[i] = ((float)rand() / RAND_MAX) * 0.5f + 0.5f; // 0.5 ~ 1.0 ���� ���
    }
}

void DrawStars() {
    glDisable(GL_LIGHTING); // ������ ��ü �߱��ϹǷ� ���� ��Ȱ��ȭ
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ���� ���� ����
    glEnable(GL_POINT_SMOOTH);
    for (int i = 0; i < NUM_STARS; i++) {
        float brightness = starBrightness[i];
        float star_size = ((float)rand() / RAND_MAX) * 1.5f + 1.0f; // 1.0 ~ 2.5 ���� ũ��
        glPointSize(star_size); // �� ũ�� ����
        glColor4f(brightness, brightness, brightness, 1.0f); // ��� ����
        glBegin(GL_POINTS);
        glVertex3fv(stars[i]); // �� ��ġ ����
        glEnd();
    }
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING); // ���� �ٽ� Ȱ��ȭ
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ���� ���۵� �Բ� Ŭ����
    glEnable(GL_DEPTH_TEST); // ���� �׽�Ʈ Ȱ��ȭ

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 1.5, 0.0,  // ī�޶� ��ġ (���ʿ��� �Ʒ���)
        0.0, 0.0, 0.0,   // �ٶ󺸴� ��
        0.0, 0.0, 1.0);  // �� ���� (Z�� ����)

    // �� �׸���
    DrawStars();

    // ���� ����
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // ���� �Ӽ� ���� 
    GLfloat light_position[] = { 0.0, 0.25, 0.0, 1.0 }; // �¾� ���� ��ġ
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 }; // �ֺ���
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Ȯ�걤
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // �ݻ籤

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // �¾� �׸���
    glPushMatrix();
    // �¾��� ���� ����
    GLfloat sun_ambient[] = { 1.0, 0.2, 0.0, 1.0 }; // ��-��Ȳ��
    GLfloat sun_diffuse[] = { 1.0, 0.5, 0.0, 1.0 };
    GLfloat sun_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat sun_shininess[] = { 60.0 };
    GLfloat sun_emission[] = { 0.9, 0.4, 0.0, 1.0 }; // ���� �Ӽ� �߰�

    glMaterialfv(GL_FRONT, GL_AMBIENT, sun_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sun_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, sun_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);

    glutSolidSphere(0.2, 40, 40); // �¾�
    glPopMatrix();

    // ������ �� �׸���
    glPushMatrix();
    glRotatef(Day, 0.0, 1.0, 0.0);    // ���� ���� (Y�� �������� ȸ��)
    glTranslatef(0.7, 0.0, 0.0);      // ���� ���� �ݰ� (X�� ���� �̵�)
    glRotatef(Time, 0.0, 1.0, 0.0);   // ���� ���� (Y�� �������� ȸ��)

    // ������ ���� ����
    GLfloat earth_ambient[] = { 0.0, 0.0, 1.0, 1.0 }; // �Ķ���
    GLfloat earth_diffuse[] = { 0.0, 0.5, 1.0, 1.0 };
    GLfloat earth_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat earth_shininess[] = { 30.0 };
    GLfloat earth_emission[] = { 0.0, 0.0, 0.0, 1.0 }; // ���� ����

    glMaterialfv(GL_FRONT, GL_AMBIENT, earth_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, earth_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, earth_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, earth_emission);

    glutSolidSphere(0.1, 30, 30); // ����

    // �� �׸���
    glPushMatrix();
    glRotatef(Time * 12, 0.0, 1.0, 0.0); // �� ���� (�ӵ� ����, Y�� ���� ȸ��)
    glTranslatef(0.2, 0.0, 0.0);         // �� ���� �ݰ� (X�� ���� �̵�)

    // ���� ���� ����
    GLfloat moon_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; // ȸ��
    GLfloat moon_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat moon_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat moon_shininess[] = { 25.0 };
    GLfloat moon_emission[] = { 0.0, 0.0, 0.0, 1.0 }; // ���� ����

    glMaterialfv(GL_FRONT, GL_AMBIENT, moon_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, moon_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, moon_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, moon_emission);

    glutSolidSphere(0.04, 30, 30); // ��
    glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

void MyIdle() {
    Day = fmod(Day + 0.1f, 360.0f);  // ���� �ӵ�
    Time = fmod(Time + 0.2f, 360.0f); // ���� �ӵ�
    glutPostRedisplay(); // ȭ�� ������Ʈ ��û
}

void Init() {
    glEnable(GL_DEPTH_TEST);  // ���� �׽�Ʈ Ȱ��ȭ
    glEnable(GL_NORMALIZE);   // ���� ���� ����ȭ
    InitStars();              // �� �ʱ�ȭ �Լ� ȣ��
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ���� ���� ���
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL ������ ���� ����");
    glClearColor(0, 0, 0, 1.0);

    Init(); // �ʱ�ȭ �Լ� ȣ��

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 20.0); // ���� ���� ����

    glutDisplayFunc(MyDisplay);
    glutIdleFunc(MyIdle); // Idle �Լ� ���
    glutMainLoop();
    return 0;
}
