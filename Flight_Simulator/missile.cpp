#include "missile.h"
#include "terrain.h"
#include "camera.h"
#include <vector>
#include <GL/freeglut.h>

// ���� ���� ����
extern Camera cam;       // ī�޶� ���� ���� ����
extern Terrain *terrain; // ���� ���� ���� ����

Missile::Missile(Point3 startPos, Vector3 dir, float spd)
    : position(startPos), direction(dir), speed(spd), isActive(true) {
    direction.normalize(); // ���� ���� ����ȭ
}

void Missile::updatePosition(float deltaTime) {
    if (!isActive) {
        updateParticles(deltaTime); // �浹 �� ��ƼŬ�� ������Ʈ
        return;
    }

    // �̻��� �̵�
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
    position.z += direction.z * speed * deltaTime;

    // �浹 ����
    if (checkCollision()) {
        isActive = false;
        createParticles(); // �浹 �� ��ƼŬ ����
    }
}

void Missile::draw() const {
    if (isActive) {
        // �̻��� �׸���
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glColor3f(1.0f, 0.0f, 0.0f); // ������ �̻���
        glutSolidSphere(0.5, 16, 16);
        glPopMatrix();
    }

    // ��ƼŬ �׸���
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
            (rand() % 200 - 100) / 100.0f, // x�� �ӵ�
            (rand() % 200) / 100.0f,      // y�� �ӵ�
            (rand() % 200 - 100) / 100.0f // z�� �ӵ�
        );
        particles.emplace_back(position, randomVel, 1.5f, 0.2f);
    }
}

void Missile::updateParticles(float deltaTime) {
    for (auto it = particles.begin(); it != particles.end();) {
        it->update(deltaTime);
        if (it->lifetime <= 0) {
            it = particles.erase(it); // ������ ���� ��ƼŬ ����
        }
        else {
            ++it;
        }
    }
}
