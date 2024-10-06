#include "particle.h"
#include <cstdlib>

std::vector<Particle> particles;

void CreateParticles(Vector2 position, float speed) {
    const int numParticles = 20;

    for (int i = 0; i < numParticles; i++) {
        Particle particle;

        // Particle Colours
        Color particleColor = {
            static_cast<unsigned char>(rand() % 56 + 200),  
            static_cast<unsigned char>(rand() % 156 + 100), 
            0,                                             
            255
        };

        // Particle Size and Lifespan
        float particleSize = 1.0f + static_cast<float>(rand() % 8);
        float lifeSpan = 1.0f + static_cast<float>(rand() % 2);     

        particle.position = position;
        particle.speed = Vector2{ (float)(rand() % 200 - 100) / 100.0f * speed,
                                  (float)(rand() % 200 - 100) / 100.0f * speed };
        particle.size = particleSize;
        particle.color = particleColor;
        particle.lifeSpan = lifeSpan;
        particle.alpha = 1.0f;

        particles.push_back(particle);
    }
}

void UpdateParticles() {
    for (auto it = particles.begin(); it != particles.end();) {
        it->lifeSpan -= GetFrameTime();
        if (it->lifeSpan <= 0) {
            it = particles.erase(it);
        }
        else {
            it->alpha -= GetFrameTime() / it->lifeSpan;
            if (it->alpha < 0) it->alpha = 0;

            it->size -= GetFrameTime();
            if (it->size < 0.5f) it->size = 0.5f; // Min Size

            it->position.x += it->speed.x;
            it->position.y += it->speed.y;

            ++it;
        }
    }
}

void DrawParticles() {
    for (const auto& particle : particles) {
        if (particle.lifeSpan > 0.0f) {
            Color particleColor = Fade(particle.color, particle.alpha);
            DrawCircleV(particle.position, particle.size, particleColor);
        }
    }
}
