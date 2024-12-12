#pragma once
// skybox.h
#include <GL/freeglut.h>
#include <string>

class Skybox {
public:
    Skybox();
    ~Skybox();
    void draw();
private:
    typedef unsigned int uint;
    typedef const char *pcStr;
    uint textures[6];
    static uint _loadTexture(pcStr filename);
};