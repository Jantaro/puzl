#include <SFML/Graphics.hpp>
using sf::Sprite;
using sf::RenderWindow;
#include <iostream>
#include <utility>
using std::pair;

typedef pair<int, int> Coords;
typedef pair<Coords, Coords> ClickBox;

bool inClickBox(const ClickBox&, const Coords&);
ClickBox makeClickBox(int, int, int, int);

int main()
{
  sf::RenderWindow App(sf::VideoMode(512, 512, 32), "puzl");

  App.SetFramerateLimit(30);

  sf::Image Image;
  Image.LoadFromFile("test.png");
  sf::Sprite Sprite;
  Sprite.SetImage(Image);

  Coords cursor;
  ClickBox testBox = makeClickBox(189, 175, 208, 194);

  while (App.IsOpened()){
    sf::Event Event;
    while (App.GetEvent(Event)){
      if (Event.Type == sf::Event::Closed)
        App.Close();
      if (Event.Type == sf::Event::MouseButtonPressed){
        cursor.first = Event.MouseButton.X;
        cursor.second = Event.MouseButton.Y;
        std::cout << cursor.first << " " << cursor.second << std::endl;
        if (inClickBox(testBox, cursor)){
          std::cout << "Good aim." << std::endl;
        }
      }
    }

    App.Clear();
    App.Draw(Sprite);
    App.Display();
  }

  return EXIT_SUCCESS;
}

bool inClickBox(const ClickBox& box, const Coords& cursor){
  return cursor.first >= box.first.first && cursor.second >= box.first.second && cursor.first <= box.second.first && cursor.second <= box.second.second;
}

ClickBox makeClickBox(int x1, int y1, int x2, int y2){
  ClickBox box(pair<int, int>(x1, y1),pair<int, int>(x2, y2));
  return box;
}
