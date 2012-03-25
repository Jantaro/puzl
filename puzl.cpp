#include <SFML/Graphics.hpp>
using sf::Sprite;
using sf::RenderWindow;
#include <iostream>
#include <utility>
using std::pair;
#include <vector>
using std::vector;

typedef pair<int, int> Coords;
typedef pair<Coords, Coords> ClickBox;

struct View{
  Sprite sprite;
  vector<ClickBox> boxen;
};
struct State{
  View view;
};

bool inClickBox(const ClickBox&, const Coords&);
ClickBox makeClickBox(int, int, int, int);
void clickHandle(int);

State state;
View view1;
View view2;

int main()
{
  sf::RenderWindow App(sf::VideoMode(512, 512, 32), "puzl");

  App.SetFramerateLimit(30);

  sf::Image Image_1;
  sf::Image Image_2;
  Image_1.LoadFromFile("test.png");
  Image_2.LoadFromFile("test_2.png");
  sf::Sprite Sprite_1;
  sf::Sprite Sprite_2;
  Sprite_1.SetImage(Image_1);
  Sprite_2.SetImage(Image_2);

  view1.sprite = Sprite_1;
  view2.sprite = Sprite_2;
  view1.boxen = vector<ClickBox>{makeClickBox(189, 175, 208, 194), makeClickBox(353, 182, 372, 201)};
  view2.boxen = vector<ClickBox>{makeClickBox(224, 118, 266, 140), makeClickBox(152, 70, 163, 81)};
  state.view = view1;

  Coords cursor;

  while (App.IsOpened()){
    sf::Event Event;
    while (App.GetEvent(Event)){
      if (Event.Type == sf::Event::Closed)
        App.Close();
      if (Event.Type == sf::Event::MouseButtonPressed){
        cursor.first = Event.MouseButton.X;
        cursor.second = Event.MouseButton.Y;
        std::cout << cursor.first << " " << cursor.second << std::endl;
        for(unsigned int i = 0; i != state.view.boxen.size(); ++i){
          if (inClickBox(state.view.boxen[i], cursor)){
            clickHandle(i);
            break;
          }
        }
      }
    }

    App.Clear();
    App.Draw(state.view.sprite);
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

void clickHandle(int boxID){
  if (boxID == 0){
    state.view = view1;
    std::cout << "1" << std::endl;
  }
  else if (boxID == 1){
    state.view = view2;
    std::cout << "2" << std::endl;
  }
}
