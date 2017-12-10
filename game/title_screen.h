#include "stdafx.h"

class TitleScreen {
public:
    TitleScreen();

    void Update(sf::Time elapsed_time);
    void Draw(sf::RenderWindow& window);

    sf::Font bonbon_font;
    sf::Font square_font;
    sf::Font linebrk_font;

private:
    sf::Text game_title;
    sf::Text game_subtitle;
    sf::Text press_space;
    sf::Text barichello;
};
