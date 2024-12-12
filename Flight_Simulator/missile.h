#pragma once
#include <vector>
#include <GL/freeglut.h>
#include "terrain.h"
#include "camera.h"

// 파티클 구조체 정의
struct Particle {
    Point3 position;    // 파티클 위치
    Vector3 velocity;   // 파티클 속도
    float lifetime;     // 파티클 수명
    float size;         // 파티클 크기

    Particle(Point3 pos, Vector3 vel, float life, float sz)
        : position(pos), velocity(vel), lifetime(life), size(sz) {
    }

    void update(float deltaTime) {
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        position.z += velocity.z * deltaTime;

        velocity.y -= 9.8f * deltaTime; // 중력 효과
        lifetime -= deltaTime;         // 수명 감소
    }

    void draw() const {
        if (lifetime <= 0) return;

        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glColor4f(1.0f, 0.5f, 0.0f, lifetime / 3.0f); // 수명에 따라 투명도 감소
        glutSolidSphere(size, 8, 8);                 // 구체로 렌더링
        glPopMatrix();
    }
};

// 미사일 클래스 정의
class Missile {
public:
    Point3 position;               // 미사일 위치
    Vector3 direction;             // 미사일 방향
    float speed;                   // 미사일 속도
    bool isActive;                 // 활성화 상태
    std::vector<Particle> particles; // 파티클 리스트

    Missile(Point3 startPos, Vector3 dir, float spd)
        : position(startPos), direction(dir), speed(spd), isActive(true) {
        direction.normalize(); // 방향 벡터 정규화
    }

    void updatePosition(float deltaTime, Terrain *terrain) {
        if (!isActive) {
            updateParticles(deltaTime); // 충돌 후 파티클만 업데이트
            return;
        }

        // 미사일 이동
        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
        position.z += direction.z * speed * deltaTime;

        // 충돌 감지
        if (checkCollision(terrain)) {
            isActive = false;
            createParticles(); // 충돌 시 파티클 생성
        }
    }

    void draw() const {
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

private:
    bool checkCollision(Terrain *terrain) {
        float groundHeight = terrain->getHeight(position.x, position.z);
        return position.y <= groundHeight;
    }

    void createParticles() {
        for (int i = 0; i < 50; ++i) {
            Vector3 randomVel(
                (rand() % 200 - 100) / 100.0f, // x축 속도
                (rand() % 200) / 100.0f,      // y축 속도
                (rand() % 200 - 100) / 100.0f // z축 속도
            );
            particles.emplace_back(position, randomVel, 1.5f, 0.2f);
        }
    }

    void updateParticles(float deltaTime) {
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
};
