#include <GL/freeglut.h>
#include <stdlib.h> // rand() 함수를 위해 추가
#include <time.h>   // time() 함수를 위해 추가
#include <math.h>   // fmod 함수를 사용하기 위해 추가
#define NUM_STARS 1000

static float Day = 0, Time = 0;
float stars[NUM_STARS][3];
float starBrightness[NUM_STARS];

void InitStars() {
    srand(time(NULL)); // 랜덤 시드 설정
    for (int i = 0; i < NUM_STARS; i++) {
        // X ~ Z 평면 범위 + Y 깊이 범위
        stars[i][0] = ((float)rand() / RAND_MAX) * 10.0f - 5.0f; // X좌표: -5.0 ~ 5.0
        stars[i][1] = -((float)rand() / RAND_MAX) * 10.0f - 1.0f; // Y좌표: -1.0 ~ -11.0
        stars[i][2] = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;  // Z좌표: -5.0 ~ 5.0

        starBrightness[i] = ((float)rand() / RAND_MAX) * 0.5f + 0.5f; // 0.5 ~ 1.0 사이 밝기
    }
}

void DrawStars() {
    glDisable(GL_LIGHTING); // 별들은 자체 발광하므로 조명 비활성화
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 알파 블렌딩 적용
    glEnable(GL_POINT_SMOOTH);
    for (int i = 0; i < NUM_STARS; i++) {
        float brightness = starBrightness[i];
        float star_size = ((float)rand() / RAND_MAX) * 1.5f + 1.0f; // 1.0 ~ 2.5 사이 크기
        glPointSize(star_size); // 별 크기 설정
        glColor4f(brightness, brightness, brightness, 1.0f); // 밝기 적용
        glBegin(GL_POINTS);
        glVertex3fv(stars[i]); // 별 위치 설정
        glEnd();
    }
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING); // 조명 다시 활성화
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 깊이 버퍼도 함께 클리어
    glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 1.5, 0.0,  // 카메라 위치 (위쪽에서 아래로)
        0.0, 0.0, 0.0,   // 바라보는 점
        0.0, 0.0, 1.0);  // 업 벡터 (Z축 방향)

    // 별 그리기
    DrawStars();

    // 조명 설정
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 광원 속성 설정 
    GLfloat light_position[] = { 0.0, 0.25, 0.0, 1.0 }; // 태양 위에 위치
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 }; // 주변광
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // 확산광
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // 반사광

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // 태양 그리기
    glPushMatrix();
    // 태양의 재질 설정
    GLfloat sun_ambient[] = { 1.0, 0.2, 0.0, 1.0 }; // 빨-주황색
    GLfloat sun_diffuse[] = { 1.0, 0.5, 0.0, 1.0 };
    GLfloat sun_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat sun_shininess[] = { 60.0 };
    GLfloat sun_emission[] = { 0.9, 0.4, 0.0, 1.0 }; // 방출 속성 추가

    glMaterialfv(GL_FRONT, GL_AMBIENT, sun_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sun_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, sun_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);

    glutSolidSphere(0.2, 40, 40); // 태양
    glPopMatrix();

    // 지구와 달 그리기
    glPushMatrix();
    glRotatef(Day, 0.0, 1.0, 0.0);    // 지구 공전 (Y축 기준으로 회전)
    glTranslatef(0.7, 0.0, 0.0);      // 지구 공전 반경 (X축 방향 이동)
    glRotatef(Time, 0.0, 1.0, 0.0);   // 지구 자전 (Y축 기준으로 회전)

    // 지구의 재질 설정
    GLfloat earth_ambient[] = { 0.0, 0.0, 1.0, 1.0 }; // 파란색
    GLfloat earth_diffuse[] = { 0.0, 0.5, 1.0, 1.0 };
    GLfloat earth_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat earth_shininess[] = { 30.0 };
    GLfloat earth_emission[] = { 0.0, 0.0, 0.0, 1.0 }; // 방출 없음

    glMaterialfv(GL_FRONT, GL_AMBIENT, earth_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, earth_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, earth_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, earth_emission);

    glutSolidSphere(0.1, 30, 30); // 지구

    // 달 그리기
    glPushMatrix();
    glRotatef(Time * 12, 0.0, 1.0, 0.0); // 달 공전 (속도 조절, Y축 기준 회전)
    glTranslatef(0.2, 0.0, 0.0);         // 달 공전 반경 (X축 방향 이동)

    // 달의 재질 설정
    GLfloat moon_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; // 회색
    GLfloat moon_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat moon_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat moon_shininess[] = { 25.0 };
    GLfloat moon_emission[] = { 0.0, 0.0, 0.0, 1.0 }; // 방출 없음

    glMaterialfv(GL_FRONT, GL_AMBIENT, moon_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, moon_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, moon_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, moon_emission);

    glutSolidSphere(0.04, 30, 30); // 달
    glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

void MyIdle() {
    Day = fmod(Day + 0.1f, 360.0f);  // 공전 속도
    Time = fmod(Time + 0.2f, 360.0f); // 자전 속도
    glutPostRedisplay(); // 화면 업데이트 요청
}

void Init() {
    glEnable(GL_DEPTH_TEST);  // 깊이 테스트 활성화
    glEnable(GL_NORMALIZE);   // 법선 벡터 정규화
    InitStars();              // 별 초기화 함수 호출
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 깊이 버퍼 사용
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL 지구와 달의 공전");
    glClearColor(0, 0, 0, 1.0);

    Init(); // 초기화 함수 호출

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 20.0); // 원근 투영 설정

    glutDisplayFunc(MyDisplay);
    glutIdleFunc(MyIdle); // Idle 함수 등록
    glutMainLoop();
    return 0;
}
