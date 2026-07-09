#include "raylib.h"

#include <algorithm>
#include <cmath>
#include <deque>
#include <random>
#include <string>
#include <vector>

// This small Vec2 struct keeps the vector math visible for the project.
struct Vec2
{
    float x = 0.0f; // x is the horizontal part of the vector.
    float y = 0.0f; // y is the vertical part of the vector.

    Vec2() = default; // Default vector starts at zero.
    Vec2(float xValue, float yValue) : x(xValue), y(yValue) {} // Quick way to make a vector.

    Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; } // Vector addition.
    Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; } // Vector subtraction.
    Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }       // Scalar multiplication.
    Vec2 operator/(float scalar) const { return {x / scalar, y / scalar}; }       // Scalar division.

    Vec2& operator+=(const Vec2& other) // Adds another vector into this vector.
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator*=(float scalar) // Scales this vector in place.
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    float Length() const // Finds how long the vector is.
    {
        return std::sqrt((x * x) + (y * y));
    }

    Vec2 Normalized() const // Gives the same direction with length 1.
    {
        const float length = Length();
        if (length < 0.0001f)
        {
            return {0.0f, 0.0f};
        }
        return {x / length, y / length};
    }
};

// This lets me write scalar * vector too.
Vec2 operator*(float scalar, const Vec2& vector)
{
    return vector * scalar;
}

// raylib uses Vector2, so this converts my vector type for drawing.
Vector2 ToRaylib(Vec2 vector)
{
    return {vector.x, vector.y};
}

// I use this to keep values inside a clean range.
float ClampFloat(float value, float low, float high)
{
    return std::max(low, std::min(value, high));
}

// These settings control the whole physics simulation.
struct PhysicsSettings
{
    float mass = 1.0f;              // Mass turns net force into acceleration.
    float gravity = 480.0f;         // Gravity pulls particles down.
    float wind = 160.0f;            // Wind pushes particles sideways.
    float drag = 0.55f;             // Drag makes air resistance stronger.
    bool dragEnabled = true;        // This toggles friction/air resistance.
    float groundY = 660.0f;         // The floor position.
    float bounce = 0.52f;           // Bounce keeps the motion fun to watch.
    float groundFriction = 0.82f;   // This slows particles when they hit the floor.
};

// A particle stores position, velocity, age, color, and a small trail.
struct Particle
{
    Vec2 position;                  // Current location.
    Vec2 velocity;                  // Current velocity.
    Color color = WHITE;            // Trail and particle color.
    float age = 0.0f;               // How long the particle has existed.
    float lifetime = 8.5f;          // When it disappears.
    float radius = 4.0f;            // Visual size on screen.
    std::deque<Vec2> trail;         // Older positions become a colorful trail.

    bool IsAlive() const // This tells the update loop if the particle stays.
    {
        return age < lifetime;
    }
};

// These are the forces acting on one particle at one moment.
struct ForceBreakdown
{
    Vec2 gravity;       // Downward force.
    Vec2 wind;          // Sideways force.
    Vec2 drag;          // Opposite of velocity.
    Vec2 netForce;      // gravity + wind + drag.
    Vec2 acceleration;  // netForce / mass.
};

// This calculates all forces for a particle.
ForceBreakdown CalculateForces(const Particle& particle, const PhysicsSettings& settings)
{
    ForceBreakdown forces;

    forces.gravity = {0.0f, settings.mass * settings.gravity}; // Fg points down.
    forces.wind = {settings.mass * settings.wind, 0.0f};       // Wind points left or right.

    if (settings.dragEnabled)
    {
        forces.drag = particle.velocity * (-settings.drag);    // Drag fights the current velocity.
    }
    else
    {
        forces.drag = {0.0f, 0.0f};                            // No drag when it is toggled off.
    }

    forces.netForce = forces.gravity + forces.wind + forces.drag; // Vector addition happens here.
    forces.acceleration = forces.netForce / settings.mass;        // Newton's second law: a = F / m.

    return forces;
}

// This makes a new particle from the cannon.
Particle MakeParticle(Vec2 emitter, int particleNumber, std::mt19937& rng)
{
    std::uniform_real_distribution<float> angleJitter(-9.0f, 9.0f); // Small angle randomness.
    std::uniform_real_distribution<float> speedJitter(-55.0f, 55.0f); // Small speed randomness.
    std::uniform_real_distribution<float> sizeJitter(3.0f, 6.0f); // Slightly different sizes.

    const float angleDegrees = -52.0f + angleJitter(rng); // Negative y launches upward on the screen.
    const float angleRadians = angleDegrees * DEG2RAD;
    const float speed = 450.0f + speedJitter(rng);

    Particle particle;
    particle.position = emitter;
    particle.velocity = {std::cos(angleRadians) * speed, std::sin(angleRadians) * speed};
    particle.radius = sizeJitter(rng);
    particle.color = ColorFromHSV(std::fmod(static_cast<float>(particleNumber) * 17.0f, 360.0f), 0.85f, 1.0f);
    particle.trail.push_back(particle.position);

    return particle;
}

// This moves one particle using the vector update formulas.
void UpdateParticle(Particle& particle, float dt, const PhysicsSettings& settings)
{
    const ForceBreakdown forces = CalculateForces(particle, settings);

    particle.velocity += forces.acceleration * dt; // v = v + a * t.
    particle.position += particle.velocity * dt;   // p = p + v * t.
    particle.age += dt;

    particle.trail.push_back(particle.position);   // Save the current position for the trail.
    while (particle.trail.size() > 70)
    {
        particle.trail.pop_front();                // Keep trails from getting too long.
    }

    if (particle.position.y + particle.radius > settings.groundY) // Simple floor collision.
    {
        particle.position.y = settings.groundY - particle.radius;

        if (particle.velocity.y > 0.0f)
        {
            particle.velocity.y *= -settings.bounce;      // Reverse y velocity for a bounce.
            particle.velocity.x *= settings.groundFriction; // Lose some x speed on the ground.
        }
    }
}

// This draws one arrow with an arrowhead and a label.
void DrawArrow(Vec2 start, Vec2 vector, Color color, const char* label, float thickness = 4.0f)
{
    const float length = vector.Length();
    if (length < 2.0f)
    {
        return;
    }

    const Vec2 end = start + vector;
    const Vec2 direction = vector.Normalized();
    const Vec2 side = {-direction.y, direction.x};
    const float headLength = 14.0f;

    DrawLineEx(ToRaylib(start), ToRaylib(end), thickness, color); // Main arrow line.

    const Vec2 left = end - direction * headLength + side * (headLength * 0.55f);
    const Vec2 right = end - direction * headLength - side * (headLength * 0.55f);
    DrawTriangle(ToRaylib(end), ToRaylib(left), ToRaylib(right), color); // Arrow head.

    if (label != nullptr)
    {
        DrawText(label, static_cast<int>(end.x + 8.0f), static_cast<int>(end.y - 8.0f), 16, color);
    }
}

// This draws the background grid so movement is easier to judge.
void DrawGridBackground(int width, int height)
{
    ClearBackground({12, 15, 25, 255});

    const Color gridColor = {35, 42, 62, 255};
    for (int x = 0; x < width; x += 40)
    {
        DrawLine(x, 0, x, height, gridColor);
    }
    for (int y = 0; y < height; y += 40)
    {
        DrawLine(0, y, width, y, gridColor);
    }
}

// This draws the ground where particles bounce.
void DrawGround(float groundY, int screenWidth)
{
    DrawRectangle(0, static_cast<int>(groundY), screenWidth, 80, {22, 30, 38, 255});
    DrawLineEx({0.0f, groundY}, {static_cast<float>(screenWidth), groundY}, 4.0f, {100, 210, 190, 255});
    DrawText("ground", 20, static_cast<int>(groundY + 12.0f), 18, {160, 220, 220, 255});
}

// This draws the faded trail behind one particle.
void DrawParticleTrail(const Particle& particle)
{
    if (particle.trail.size() < 2)
    {
        return;
    }

    for (std::size_t i = 1; i < particle.trail.size(); ++i)
    {
        const float fadeAmount = static_cast<float>(i) / static_cast<float>(particle.trail.size());
        Color trailColor = particle.color;
        trailColor.a = static_cast<unsigned char>(35 + 180 * fadeAmount);

        DrawLineEx(ToRaylib(particle.trail[i - 1]), ToRaylib(particle.trail[i]), 3.0f, trailColor);
    }
}

// This draws the emitter and the starting velocity arrow.
void DrawEmitter(Vec2 emitter)
{
    const Vec2 launchVelocity = {280.0f, -350.0f};

    DrawCircleV(ToRaylib(emitter), 14.0f, {250, 250, 250, 255});
    DrawCircleV(ToRaylib(emitter), 8.0f, {50, 70, 95, 255});
    DrawArrow(emitter, launchVelocity * 0.18f, {70, 220, 255, 255}, "initial v", 5.0f);
}

// This picks a particle in the middle so the vector arrows are not too jumpy.
const Particle* PickHighlightedParticle(const std::vector<Particle>& particles, float groundY)
{
    for (const Particle& particle : particles)
    {
        if (particle.age > 0.75f && particle.position.y < groundY - 35.0f)
        {
            return &particle;
        }
    }

    if (!particles.empty())
    {
        return &particles.back();
    }

    return nullptr;
}

// This draws force arrows on the highlighted particle.
void DrawForcesOnParticle(const Particle& particle, const PhysicsSettings& settings)
{
    const ForceBreakdown forces = CalculateForces(particle, settings);
    const float forceScale = 0.16f;

    DrawCircleLines(static_cast<int>(particle.position.x), static_cast<int>(particle.position.y), particle.radius + 8.0f, WHITE);
    DrawArrow(particle.position, forces.gravity * forceScale, {80, 140, 255, 255}, "gravity");
    DrawArrow(particle.position, forces.wind * forceScale, {255, 214, 80, 255}, "wind");

    if (settings.dragEnabled)
    {
        DrawArrow(particle.position, forces.drag * forceScale, {255, 100, 120, 255}, "friction");
    }

    DrawArrow(particle.position, forces.netForce * forceScale, {105, 255, 130, 255}, "net", 5.0f);
}

// This panel shows the vectors added tail-to-tip.
void DrawVectorAdditionPanel(const Particle* highlighted, const PhysicsSettings& settings)
{
    const int panelX = 905;
    const int panelY = 85;
    const int panelW = 340;
    const int panelH = 260;
    const Vec2 base = {panelX + 70.0f, panelY + 75.0f};
    const float scale = 0.17f;

    DrawRectangle(panelX, panelY, panelW, panelH, {20, 25, 39, 230});
    DrawRectangleLines(panelX, panelY, panelW, panelH, {90, 110, 150, 255});
    DrawText("Vector Addition View", panelX + 18, panelY + 16, 20, WHITE);
    DrawText("gravity + wind + friction = net force", panelX + 18, panelY + 42, 15, {190, 205, 220, 255});

    if (highlighted == nullptr)
    {
        DrawText("waiting for particles...", panelX + 18, panelY + 105, 18, {200, 205, 220, 255});
        return;
    }

    const ForceBreakdown forces = CalculateForces(*highlighted, settings);

    const Vec2 g = forces.gravity * scale;
    const Vec2 w = forces.wind * scale;
    const Vec2 d = forces.drag * scale;
    const Vec2 n = forces.netForce * scale;

    DrawCircleV(ToRaylib(base), 5.0f, WHITE);
    DrawArrow(base, g, {80, 140, 255, 255}, "g", 3.0f);
    DrawArrow(base + g, w, {255, 214, 80, 255}, "wind", 3.0f);

    if (settings.dragEnabled)
    {
        DrawArrow(base + g + w, d, {255, 100, 120, 255}, "drag", 3.0f);
    }

    DrawArrow(base, n, {105, 255, 130, 255}, "result", 5.0f);

    DrawText("Tail-to-tip shows the same", panelX + 18, panelY + 205, 16, {210, 220, 235, 255});
    DrawText("answer as one net arrow.", panelX + 18, panelY + 225, 16, {210, 220, 235, 255});
}

// This panel explains the controls and the math formula.
void DrawInfoPanel(const PhysicsSettings& settings, int particleCount)
{
    DrawRectangle(18, 18, 395, 190, {20, 25, 39, 230});
    DrawRectangleLines(18, 18, 395, 190, {90, 110, 150, 255});

    DrawText("Physics-Based Particle System", 34, 32, 22, WHITE);
    DrawText("Math: net force = gravity + wind + friction", 34, 65, 17, {210, 220, 235, 255});
    DrawText("Update: v = v + (a * dt),  p = p + (v * dt)", 34, 89, 17, {210, 220, 235, 255});

    DrawText("A/D or Left/Right: change wind", 34, 123, 16, {185, 205, 230, 255});
    DrawText("Q/E: change gravity   1/2: change friction", 34, 145, 16, {185, 205, 230, 255});
    DrawText("F: toggle friction   Space: burst   R: reset", 34, 167, 16, {185, 205, 230, 255});

    DrawText(TextFormat("particles: %i", particleCount), 300, 32, 17, {150, 255, 180, 255});
    DrawText(TextFormat("wind: %.0f", settings.wind), 300, 56, 17, {255, 214, 80, 255});
}

// This draws live values so the user can see what changed.
void DrawStatsPanel(const PhysicsSettings& settings)
{
    const int x = 905;
    const int y = 365;

    DrawRectangle(x, y, 340, 145, {20, 25, 39, 230});
    DrawRectangleLines(x, y, 340, 145, {90, 110, 150, 255});

    DrawText("Live Force Settings", x + 18, y + 16, 20, WHITE);
    DrawText(TextFormat("gravity force: %.0f", settings.gravity), x + 18, y + 52, 17, {80, 140, 255, 255});
    DrawText(TextFormat("wind force: %.0f", settings.wind), x + 18, y + 78, 17, {255, 214, 80, 255});
    DrawText(TextFormat("friction: %.2f  [%s]", settings.drag, settings.dragEnabled ? "on" : "off"), x + 18, y + 104, 17, {255, 100, 120, 255});
}

// This handles keyboard and mouse input.
void HandleInput(PhysicsSettings& settings, Vec2& emitter, std::vector<Particle>& particles, float dt)
{
    const float windStep = 520.0f * dt;
    const float gravityStep = 360.0f * dt;
    const float dragStep = 0.75f * dt;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        settings.wind += windStep;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        settings.wind -= windStep;
    }
    if (IsKeyDown(KEY_E))
    {
        settings.gravity += gravityStep;
    }
    if (IsKeyDown(KEY_Q))
    {
        settings.gravity -= gravityStep;
    }
    if (IsKeyDown(KEY_TWO))
    {
        settings.drag += dragStep;
    }
    if (IsKeyDown(KEY_ONE))
    {
        settings.drag -= dragStep;
    }
    if (IsKeyPressed(KEY_F))
    {
        settings.dragEnabled = !settings.dragEnabled;
    }
    if (IsKeyPressed(KEY_C))
    {
        particles.clear();
    }
    if (IsKeyPressed(KEY_R))
    {
        settings.wind = 160.0f;
        settings.gravity = 480.0f;
        settings.drag = 0.55f;
        settings.dragEnabled = true;
        particles.clear();
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        emitter = {static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())};
    }

    settings.wind = ClampFloat(settings.wind, -650.0f, 650.0f);
    settings.gravity = ClampFloat(settings.gravity, 0.0f, 900.0f);
    settings.drag = ClampFloat(settings.drag, 0.0f, 2.0f);
}

// This removes particles after they are too old.
void RemoveDeadParticles(std::vector<Particle>& particles)
{
    particles.erase(
        std::remove_if(particles.begin(), particles.end(), [](const Particle& particle)
        {
            return !particle.IsAlive();
        }),
        particles.end());
}

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Vector Physics Particle System");
    SetTargetFPS(60);

    PhysicsSettings settings;
    Vec2 emitter = {115.0f, settings.groundY - 18.0f};
    std::vector<Particle> particles;
    particles.reserve(350);

    std::mt19937 rng(static_cast<unsigned int>(GetTime() * 100000.0));
    float emitTimer = 0.0f;
    int particleNumber = 0;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        dt = ClampFloat(dt, 0.0f, 1.0f / 30.0f); // This avoids giant jumps if the window pauses.

        HandleInput(settings, emitter, particles, dt);

        emitTimer += dt;
        while (emitTimer >= 0.035f && particles.size() < 320)
        {
            particles.push_back(MakeParticle(emitter, particleNumber, rng));
            ++particleNumber;
            emitTimer -= 0.035f;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            for (int i = 0; i < 35 && particles.size() < 320; ++i)
            {
                particles.push_back(MakeParticle(emitter, particleNumber, rng));
                ++particleNumber;
            }
        }

        for (Particle& particle : particles)
        {
            UpdateParticle(particle, dt, settings);
        }

        RemoveDeadParticles(particles);

        const Particle* highlighted = PickHighlightedParticle(particles, settings.groundY);

        BeginDrawing();

        DrawGridBackground(screenWidth, screenHeight);
        DrawGround(settings.groundY, screenWidth);
        DrawEmitter(emitter);

        for (const Particle& particle : particles)
        {
            DrawParticleTrail(particle);
        }

        for (const Particle& particle : particles)
        {
            DrawCircleV(ToRaylib(particle.position), particle.radius, particle.color);
        }

        if (highlighted != nullptr)
        {
            DrawForcesOnParticle(*highlighted, settings);
        }

        DrawInfoPanel(settings, static_cast<int>(particles.size()));
        DrawVectorAdditionPanel(highlighted, settings);
        DrawStatsPanel(settings);

        DrawText("Left click moves the emitter. Watch the trails bend when wind changes.", 34, screenHeight - 35, 18, {210, 220, 235, 255});

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
