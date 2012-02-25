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

  sf::Image Image;
  Image.LoadFromFile("test.png");
  sf::Sprite Sprite;
  Sprite.SetImage(Image);

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
    App.Draw(Sprite);
    App.Display();
  }

  return EXIT_SUCCESS;
}
