#include <iostream>
#include <SFML/Graphics.hpp>

class EditorContext {
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "Diagram Editor");

    // TODO add an option
    window.setVerticalSyncEnabled(true);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Font font;
    if (!font.loadFromFile("/home/phileas/.fonts/JetBrainsMono/ttf/JetBrainsMono-SemiBold.ttf")) {
        std::cerr << "Error: Cannot load font" << std::endl;
        return 1;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(L"Hello world ⇨");
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                // TODO handle resizing
            }
        }

        window.clear();
        //window.draw(shape);
        window.draw(text);
        window.display();
    }

    return 0;
}

