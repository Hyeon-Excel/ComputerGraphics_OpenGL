#include <GL/freeglut.h>

int FlatShaded = 0;    int Wireframed = 0;
// 평탄 쉐이딩 및 와이어프레임 토글 변수

int ViewX = 0, ViewY = 0;
// 마우스 움직임에 따른 시점 변경 변수

void InitLight() {
    GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_shininess[] = { 15.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_position[] = { -3, 6, 3.0, 0.0 };
    glShadeModel(GL_SMOOTH); // 기본 랜더링을 솔리드로 설정
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // 조명을 GL_LIGHT0으로 설정
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    // 추가된 부분
    glEnable(GL_COLOR_MATERIAL); // 색상 추적 활성화
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // 추적할 재질 속성 지정
    glEnable(GL_NORMALIZE); // 법선 벡터 자동 정규화 활성화
}

void MyMouseMove(GLint X, GLint Y) {
    // 마우스 움직임을 전역 변수에 할당
    ViewX = X;
    ViewY = Y;
    glutPostRedisplay();    // 변경시 실시간으로 변화 적용
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
    // 테이블 상판
    glPushMatrix();
    glColor3f(0.5, 0.35, 0.05);
    glScalef(2.0, 0.1, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // 테이블 다리 
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
    // 구 
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(-0.5f, 0.25f, 0.0f);
    glutSolidSphere(0.2f, 20, 20);
    glPopMatrix();

    // 정육면체
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.8f, 0.2f, -0.35f);
    glutSolidCube(0.3f);
    glPopMatrix();

    // 원뿔
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(0.0f, 0.05f, 0.3f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.2f, 0.5f, 20, 20);
    glPopMatrix();

    // 토러스 
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(-0.85f, 0.25f, -0.45f);
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    glutSolidTorus(0.05f, 0.15f, 20, 20);
    glPopMatrix();

    // 이십면체 
    glPushMatrix();
    glColor3f(1.0, 0.5f, 0.0f);
    glTranslatef(0.5f, 0.25f, 0.1f);
    glScalef(0.2f, 0.2f, 0.2f);
    glutSolidIcosahedron();
    glPopMatrix();

    // 원기둥 
    glPushMatrix();
    glColor3f(0.5f, 0.0f, 0.5f);
    glTranslatef(-0.6f, 0.0f, 0.4f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCylinder(0.1f, 0.5f, 20, 20);
    glPopMatrix();

    // 주전자 
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

    // 카메라 설정
    gluLookAt(0.0, 3.0, 5.0, // 초기 카메라 위치 x, y, z
        0.0, 0.0, 0.0,       // 카메라가 바라보는 위치 원점
        0.0, 1.0, 0.0);      // 카메라의 업벡터 설정 y 축( 카메라 기준으로 위쪽이 어느 축인지 정한다 )

    // 마우스 움직임에 따른 장면 회전
    glRotatef((float)(ViewY % 360), 1.0, 0.0, 0.0);
    glRotatef((float)(ViewX % 360), 0.0, 1.0, 0.0);

    // 평면 - 솔리드
    if (FlatShaded) {
        glShadeModel(GL_FLAT);
    }
    else {
        glShadeModel(GL_SMOOTH);
    }

    // 와이어 프레임
    if (Wireframed) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // 테이블과 객체들 그리기
    DrawTable();
    DrawObjectsOnTable();

    glFlush();
}

void MyReshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);   // 윈도우 전체를 뷰포트로 설정
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();
    //glOrtho(-3.0, 3.0, -2.0, 2.0, 1.0, 20.0); (좌, 우, 아래, 위, 근, 원) 
    gluPerspective(30.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0); // 원근 투영
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);  // 단일 버퍼, RGBA 모드, 깊이 버퍼 사용
    glutInitWindowSize(800, 600);   // 초기 윈도우 사이즈
    glutInitWindowPosition(0, 0);   // 초기 윈도우 생성 위치
    glutCreateWindow("OpenGL Sample Drawing");  // 윈도우 창 이름

    glClearColor(0.4, 0.4, 0.4, 0.0);   // 배경 색상 회색으로 초기화
    InitLight();
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutMotionFunc(MyMouseMove);
    glutReshapeFunc(MyReshape);
    glutMainLoop();
    return 0;
}