#pragma once
#include <vector>
#include <GL/freeglut.h>
#include "terrain.h"
#include "camera.h"

// ��ƼŬ ����ü ����
struct Particle {
    Point3 position;    // ��ƼŬ ��ġ
    Vector3 velocity;   // ��ƼŬ �ӵ�
    float lifetime;     // ��ƼŬ ����
    float size;         // ��ƼŬ ũ��

    Particle(Point3 pos, Vector3 vel, float life, float sz)
        : position(pos), velocity(vel), lifetime(life), size(sz) {
    }

    void update(float deltaTime) {
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        position.z += velocity.z * deltaTime;

        velocity.y -= 9.8f * deltaTime; // �߷� ȿ��
        lifetime -= deltaTime;         // ���� ����
    }

    void draw() const {
        if (lifetime <= 0) return;

        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glColor4f(1.0f, 0.5f, 0.0f, lifetime / 3.0f); // ���� ���� ���� ����
        glutSolidSphere(size, 8, 8);                 // ��ü�� ������
        glPopMatrix();
    }
};

// �̻��� Ŭ���� ����
class Missile {
public:
    Point3 position;               // �̻��� ��ġ
    Vector3 direction;             // �̻��� ����
    float speed;                   // �̻��� �ӵ�
    bool isActive;                 // Ȱ��ȭ ����
    std::vector<Particle> particles; // ��ƼŬ ����Ʈ

    Missile(Point3 startPos, Vector3 dir, float spd)
        : position(startPos), direction(dir), speed(spd), isActive(true) {
        direction.normalize(); // ���� ���� ����ȭ
    }

    void updatePosition(float deltaTime, Terrain *terrain) {
        if (!isActive) {
            updateParticles(deltaTime); // �浹 �� ��ƼŬ�� ������Ʈ
            return;
        }

        // �̻��� �̵�
        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
        position.z += direction.z * speed * deltaTime;

        // �浹 ����
        if (checkCollision(terrain)) {
            isActive = false;
            createParticles(); // �浹 �� ��ƼŬ ����
        }
    }

    void draw() const {
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

private:
    bool checkCollision(Terrain *terrain) {
        float groundHeight = terrain->getHeight(position.x, position.z);
        return position.y <= groundHeight;
    }

    void createParticles() {
        for (int i = 0; i < 50; ++i) {
            Vector3 randomVel(
                (rand() % 200 - 100) / 100.0f, // x�� �ӵ�
                (rand() % 200) / 100.0f,      // y�� �ӵ�
                (rand() % 200 - 100) / 100.0f // z�� �ӵ�
            );
            particles.emplace_back(position, randomVel, 1.5f, 0.2f);
        }
    }

    void updateParticles(float deltaTime) {
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
};
