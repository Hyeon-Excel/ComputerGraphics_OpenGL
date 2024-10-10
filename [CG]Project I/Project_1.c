#include <GL/freeglut.h>

int FlatShaded = 0;    int Wireframed = 0;
// ��ź ���̵� �� ���̾������� ��� ����

int ViewX = 0, ViewY = 0;
// ���콺 �����ӿ� ���� ���� ���� ����

void InitLight() {
    GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_shininess[] = { 15.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_position[] = { -3, 6, 3.0, 0.0 };
    glShadeModel(GL_SMOOTH); // �⺻ �������� �ָ���� ����
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // ������ GL_LIGHT0���� ����
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    // �߰��� �κ�
    glEnable(GL_COLOR_MATERIAL); // ���� ���� Ȱ��ȭ
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // ������ ���� �Ӽ� ����
    glEnable(GL_NORMALIZE); // ���� ���� �ڵ� ����ȭ Ȱ��ȭ
}

void MyMouseMove(GLint X, GLint Y) {
    // ���콺 �������� ���� ������ �Ҵ�
    ViewX = X;
    ViewY = Y;
    glutPostRedisplay();    // ����� �ǽð����� ��ȭ ����
}

void MyKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'q': case 'Q': case '\033':
        exit(0);
        break;
    case 's':
        if (FlatShaded) {
            FlatShaded = 0;
            glShadeModel(GL_SMOOTH);
        }
        else {
            FlatShaded = 1;
            glShadeModel(GL_FLAT);
        }
        glutPostRedisplay();
        break;
    case 'w':
        if (Wireframed) {
            Wireframed = 0;
        }
        else {
            Wireframed = 1;
        }
        glutPostRedisplay();
        break;
    }
}

void DrawTable() {
    // ���̺� ����
    glPushMatrix();
    glColor3f(0.5, 0.35, 0.05);
    glScalef(2.0, 0.1, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // ���̺� �ٸ� 
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            glPushMatrix();
            glColor3f(0.5, 0.35, 0.05);
            glTranslatef(i * 0.9f, -0.55f, j * 0.4f);
            glScalef(0.1f, 1.0f, 0.1f);
            glutSolidCube(1.0);
            glPopMatrix();
        }
    }
}

void DrawObjectsOnTable() {
    // �� 
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(-0.5f, 0.25f, 0.0f);
    glutSolidSphere(0.2f, 20, 20);
    glPopMatrix();

    // ������ü
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.8f, 0.2f, -0.35f);
    glutSolidCube(0.3f);
    glPopMatrix();

    // ����
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(0.0f, 0.05f, 0.3f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.2f, 0.5f, 20, 20);
    glPopMatrix();

    // �䷯�� 
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(-0.85f, 0.25f, -0.45f);
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    glutSolidTorus(0.05f, 0.15f, 20, 20);
    glPopMatrix();

    // �̽ʸ�ü 
    glPushMatrix();
    glColor3f(1.0, 0.5f, 0.0f);
    glTranslatef(0.5f, 0.25f, 0.1f);
    glScalef(0.2f, 0.2f, 0.2f);
    glutSolidIcosahedron();
    glPopMatrix();

    // ����� 
    glPushMatrix();
    glColor3f(0.5f, 0.0f, 0.5f);
    glTranslatef(-0.6f, 0.0f, 0.4f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.1f, 0.5f, 20, 20);
    glPopMatrix();

    // ������ 
    glPushMatrix();
    glColor3f(0.8f, 0.5f, 0.2f);
    glTranslatef(0.0f, 0.5f, -0.3f);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
    glutSolidTeapot(0.2f);
    glPopMatrix();
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ī�޶� ����
    gluLookAt(0.0, 3.0, 5.0, // �ʱ� ī�޶� ��ġ x, y, z
        0.0, 0.0, 0.0,       // ī�޶� �ٶ󺸴� ��ġ ����
        0.0, 1.0, 0.0);      // ī�޶��� ������ ���� y ��( ī�޶� �������� ������ ��� ������ ���Ѵ� )

    // ���콺 �����ӿ� ���� ��� ȸ��
    glRotatef((float)(ViewY % 360), 1.0, 0.0, 0.0);
    glRotatef((float)(ViewX % 360), 0.0, 1.0, 0.0);

    // ��� - �ָ���
    if (FlatShaded) {
        glShadeModel(GL_FLAT);
    }
    else {
        glShadeModel(GL_SMOOTH);
    }

    // ���̾� ������
    if (Wireframed) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // ���̺�� ��ü�� �׸���
    DrawTable();
    DrawObjectsOnTable();

    glFlush();
}

void MyReshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);   // ������ ��ü�� ����Ʈ�� ����
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();
    //glOrtho(-3.0, 3.0, -2.0, 2.0, 1.0, 20.0); (��, ��, �Ʒ�, ��, ��, ��) 
    gluPerspective(30.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0); // ���� ����
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);  // ���� ����, RGBA ���, ���� ���� ���
    glutInitWindowSize(800, 600);   // �ʱ� ������ ������
    glutInitWindowPosition(0, 0);   // �ʱ� ������ ���� ��ġ
    glutCreateWindow("OpenGL Sample Drawing");  // ������ â �̸�

    glClearColor(0.4, 0.4, 0.4, 0.0);   // ��� ���� ȸ������ �ʱ�ȭ
    InitLight();
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutMotionFunc(MyMouseMove);
    glutReshapeFunc(MyReshape);
    glutMainLoop();
    return 0;
}