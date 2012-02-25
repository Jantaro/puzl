#include <SFML/Graphics.hpp>
using sf::Sprite;
using sf::RenderWindow;
#include <iostream>

int cursX;
int cursY;

int main()
{
  sf::RenderWindow App(sf::VideoMode(512, 512, 32), "puzl");

  App.SetFramerateLimit(30);

  while (App.IsOpened()){
    sf::Event Event;
    while (App.GetEvent(Event)){
      if (Event.Type == sf::Event::Closed)
        App.Close();
      if (Event.Type == sf::Event::MouseButtonPressed){
        cursX = Event.MouseButton.X;
        cursY = Event.MouseButton.Y;
        std::cout << cursX << " " << cursY << std::endl;
      }
    }

    App.Clear();
    App.Display();
  }

  return EXIT_SUCCESS;
}
