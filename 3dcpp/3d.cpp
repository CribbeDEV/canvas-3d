#include <SDL.h>
#include <cmath>
#include <vector>

#define WIDTH 800
#define HEIGHT 800
#define POINT_SIZE 20
#define FPS 60

#define FOREGROUND 0xFF20C20E
#define BACKGROUND 0xFF101010

struct Point3 {
    float x, y, z;
};

struct Point {
    float x, y;
};

Point screen(const Point& p)
{
    return Point{
        (p.x + 1.0f) * 0.5f * WIDTH,
        (1.0f - (p.y + 1.0f) * 0.5f) * HEIGHT
    };
}

Point project(const Point3& p)
{
    return Point{p.x / p.z, p.y / p.z};
}

Point3 translate_z(const Point3& p, float dz)
{
    return Point3{p.x, p.y, p.z + dz};
}

Point3 rotate_xz(const Point3& p, float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);
    return Point3{
        p.x * c - p.z * s,
        p.y,
        p.x * s + p.z * c
    };
}

Point3 rotate_xy(const Point3& p, float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);
    return Point3{
        p.x * c - p.y * s,
        p.x * s + p.y * c,
        p.z
    };
}

void clear(SDL_Surface* surface)
{
    SDL_FillRect(surface, nullptr, BACKGROUND);
}

void point(SDL_Surface* surface, const Point& p)
{
    SDL_Rect r{
        static_cast<int>(p.x - POINT_SIZE / 2),
        static_cast<int>(p.y - POINT_SIZE / 2),
        POINT_SIZE,
        POINT_SIZE
    };
    SDL_FillRect(surface, &r, FOREGROUND);
}

void line(SDL_Surface* surface, const Point& a, const Point& b)
{
    SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_SetRenderDrawColor(renderer, 0x20, 0xC2, 0x0E, 0xFF);
    SDL_RenderDrawLine(renderer,
        static_cast<int>(a.x), static_cast<int>(a.y),
        static_cast<int>(b.x), static_cast<int>(b.y)
    );
    SDL_DestroyRenderer(renderer);
}

void line_thick(SDL_Surface* surface, const Point& a, const Point& b, int thickness)
{
    for (int dx = -thickness/2; dx <= thickness/2; ++dx) {
        for (int dy = -thickness/2; dy <= thickness/2; ++dy) {
            SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);
            SDL_SetRenderDrawColor(renderer, 0x20, 0xC2, 0x0E, 0xFF);
            SDL_RenderDrawLine(renderer,
                static_cast<int>(a.x + dx), static_cast<int>(a.y + dy),
                static_cast<int>(b.x + dx), static_cast<int>(b.y + dy)
            );
            SDL_DestroyRenderer(renderer);
        }
    }
}

int main(int, char**)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "3D Render",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        0
    );

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    std::vector<Point3> vs = {
        { 0.25f,  0.25f,  0.25f},
        {-0.25f,  0.25f,  0.25f},
        {-0.25f, -0.25f,  0.25f},
        { 0.25f, -0.25f,  0.25f},

        { 0.25f,  0.25f, -0.25f},
        {-0.25f,  0.25f, -0.25f},
        {-0.25f, -0.25f, -0.25f},
        { 0.25f, -0.25f, -0.25f},

        { 0.0f, 0.6f, 0.0f}
    };

    std::vector<std::vector<int>> fs = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
        {0, 8},
        {1, 8},
        {4, 8},
        {5, 8},
    };

    float dz = 1.0f;
    float angle = 0.0f;
    float dt = 1.0f / FPS;

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        angle += static_cast<float>(M_PI) * dt;

        clear(surface);

        for (const auto& f : fs) {
            for (size_t i = 0; i < f.size(); ++i) {
                Point3 a = rotate_xz(vs[f[i]], angle);
                Point3 b = rotate_xz(vs[f[(i + 1) % f.size()]], angle);

                a = translate_z(a, dz);
                b = translate_z(b, dz);

                if (a.z > 0.1f && b.z > 0.1f) {
                    Point pa = screen(project(a));
                    Point pb = screen(project(b));
                    line_thick(surface, pa, pb, 5);
                }
            }
        }

        SDL_UpdateWindowSurface(window);
        SDL_Delay(1000 / FPS);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
