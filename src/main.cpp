#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}),"Test");

    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window))
        return 1;

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        ImGui::SFML::Update(window, clock.restart());

        ImGui::Begin("Test");

        ImGui::Text("test imgui");
        ImGui::Text("it sure looks like its working");

        ImGui::End();

        window.clear(sf::Color::Black);

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
