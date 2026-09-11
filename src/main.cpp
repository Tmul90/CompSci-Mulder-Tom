#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <vector>
#include <random>

const int CELL_SIZE = 10;
const int WIDTH = 30;
const int HEIGHT = 30;

std::vector<std::vector<int>> grid (HEIGHT, std::vector<int>(WIDTH, false));

void randomize() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 1);

    for (auto& row : grid) {
        for (auto& cell : row) {
            cell = dis(gen);
        }
    }
}

int countNeighbours(int x, int y) {
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                if (grid[ny][nx])
                    count++;
            }
        }
    }

    return count;
}

void update() {
    auto nextGrid = grid;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int neighbours = countNeighbours(x, y);

            if (grid[y][x]) {
                nextGrid[y][x] = (neighbours == 2 || neighbours == 3);
            }
            else {
                nextGrid[y][x] = (neighbours == 3);
            }
        }
    }

    grid = nextGrid;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE)), "Game of life");

    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window))
        return 1;

    sf::Clock clock;

    bool running = false;
    float speed = 10.0f;
    float timer = 0.0f;

    randomize();

    while (window.isOpen())
    {

        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        sf::Time deltaTime = clock.restart();

        ImGui::SFML::Update(window, deltaTime);

        ImGui::Begin("Game of Life");

        if (ImGui::Button(running ? "Pause" : "Start"))
            running = !running;

        if (ImGui::Button("Step")) {
            update();
        }

        if (ImGui::Button("Randomize")) {
            randomize();
        }

        if (ImGui::Button("Clear")) {
            for (auto& row : grid) {
                for (auto& cell : row) {
                    cell = false;
                }
            }
        }

        ImGui::SliderFloat("Speed", &speed, 1.0f, 30.0f, "%.2f");

        ImGui::End();

        timer += deltaTime.asSeconds();

        if (running && timer >= 1.0f / speed) {
            update();
            timer = 0.0f;
        }

        window.clear(sf::Color(30, 30, 30));

        sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        cellShape.setFillColor(sf::Color::White);

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (grid[y][x]) {
                    cellShape.setPosition(sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE));

                    window.draw(cellShape);
                }
            }
        }

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
