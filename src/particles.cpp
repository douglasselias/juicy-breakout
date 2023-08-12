#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include <ctime>
#include <vector>

typedef struct Particle {
  float x, y;
  float vx, vy;
  Uint32 lifetime;
} Particle;

std::vector<Particle> particles;
std::vector<Particle> init_particles(SDL_FPoint initial_position) {
  std::srand(std::time(nullptr));

  for (int i = 0; i < 10; ++i) {
    Particle particle = {
        .x = initial_position.x,
        .y = initial_position.y,
        .vx = (std::rand() % 15) / 50.0f,
        .vy = (std::rand() % 15) / 50.0f,
        .lifetime = (Uint32)(SDL_GetTicks64() + 500),
    };
    particles.push_back(particle);
  }

  return particles;
}

void update_particles(std::vector<Particle> &particles) {
  Uint32 currentTime = SDL_GetTicks64();
  for (auto &particle : particles) {
    if (currentTime > particle.lifetime) {
      continue;
    }
    particle.x += particle.vx;
    particle.y += particle.vy;
    // particle.vy += 0.005f; // Gravity
  }
}

void render_particles(SDL_Renderer *renderer, std::vector<Particle> particles) {
  Uint32 currentTime = SDL_GetTicks64();
  for (const auto &particle : particles) {
    if (currentTime > particle.lifetime) {
      continue;
    }
    // SDL_RenderDrawPointF(renderer, particle.x, particle.y);

    SDL_FRect p = {.x = particle.x, .y = particle.y, .w = 10.0f, .h = 10.0f};
    int alpha = (particle.lifetime - currentTime) % 255;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
    SDL_RenderFillRectF(renderer, &p);
  }
}