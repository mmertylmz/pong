#ifndef PARTICLE_H
#define PARTICLE_H

#include "raylib.h"
#include <vector>

struct Particle {
    Vector2 position;
    Vector2 speed;
    float size;
    Color color;
    float lifeSpan;
    float alpha;
};

extern std::vector<Particle> particles;

// Functions
void CreateParticles(Vector2 position, float speed);
void UpdateParticles();
void DrawParticles();

#endif // PARTICLE_H
