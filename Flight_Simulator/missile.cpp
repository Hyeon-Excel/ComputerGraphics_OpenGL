#include "missile.h"
#include "terrain.h"
#include "camera.h"
#include <vector>
#include <GL/freeglut.h>

// 전역 변수 참조
extern Camera cam;       // 카메라 전역 변수 참조
extern Terrain *terrain; // 지형 전역 변수 참조

Missile::Missile(Point3 startPos, Vector3 dir, float spd)
    : position(startPos), direction(dir), speed(spd), isActive(true) {
    direction.normalize(); // 방향 벡터 정규화
}

void Missile::updatePosition(float deltaTime) {
    if (!isActive) {
        updateParticles(deltaTime); // 충돌 후 파티클만 업데이트
        return;
    }

    // 미사일 이동
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
    position.z += direction.z * speed * deltaTime;

    // 충돌 감지
    if (checkCollision()) {
        isActive = false;
        createParticles(); // 충돌 시 파티클 생성
    }
}

void Missile::draw() const {
    if (isActive) {
        // 미사일 그리기
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glColor3f(1.0f, 0.0f, 0.0f); // 빨간색 미사일
        glutSolidSphere(0.5, 16, 16);
        glPopMatrix();
    }

    // 파티클 그리기
    for (const auto &particle : particles) {
        particle.draw();
    }
}

bool Missile::checkCollision() {
    float groundHeight = terrain->getHeight(position.x, position.z);
    return position.y <= groundHeight;
}

void Missile::createParticles() {
    for (int i = 0; i < 50; ++i) {
        Vector3 randomVel(
            (rand() % 200 - 100) / 100.0f, // x축 속도
            (rand() % 200) / 100.0f,      // y축 속도
            (rand() % 200 - 100) / 100.0f // z축 속도
        );
        particles.emplace_back(position, randomVel, 1.5f, 0.2f);
    }
}

void Missile::updateParticles(float deltaTime) {
    for (auto it = particles.begin(); it != particles.end();) {
        it->update(deltaTime);
        if (it->lifetime <= 0) {
            it = particles.erase(it); // 수명이 다한 파티클 제거
        }
        else {
            ++it;
        }
    }
}
