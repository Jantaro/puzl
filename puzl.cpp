#include <SFML/Graphics.hpp>
using sf::Sprite;
using sf::RenderWindow;
#include <iostream>
#include <utility>
using std::pair;
#include <vector>
using std::vector;
using std::string;

typedef pair<int, int> Coords;

struct Action{
  virtual void act() = 0;
};
struct ClickBox{
  ClickBox(int, int, int, int, vector<Action*>);
  void doActions();
  Coords a;
  Coords b;
  vector<Action*> effects;
};
struct View{
  Sprite sprite;
  vector<ClickBox> boxen;
};
struct State{
  View* view;
  vector<pair<string, int> > values;
};
struct ChangeView: public Action{
  void act();
  //ChangeView(View*);
  View* target;
};
struct ChangeState: public Action{
  void act();
  //ChangeState(string, int);
  string target;
  int value;
};

bool inClickBox(const ClickBox&, const Coords&);
ChangeView makeChangeView(View*);
ChangeState makeChangeState(string, int);

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
  ChangeView toView1 = makeChangeView(&view1);
  ChangeView toView2 = makeChangeView(&view2);
  vector<Action*> act1 = {&toView1};
  vector<Action*> act2 = {&toView2};
  view1.boxen = vector<ClickBox>{ClickBox(189, 175, 208, 194, act1),
                                 ClickBox(353, 182, 372, 201, act2)};
  view2.boxen = vector<ClickBox>{ClickBox(224, 118, 266, 140, act1),
                                 ClickBox(152, 70, 163, 81, act2)};
  state.view = &view1;

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
        for(unsigned int i = 0; i != state.view->boxen.size(); ++i){
          if (inClickBox(state.view->boxen[i], cursor)){
            state.view->boxen[i].doActions();
            break;
          }
        }
      }
    }

    App.Clear();
    App.Draw(state.view->sprite);
    App.Display();
  }

  return EXIT_SUCCESS;
}

bool inClickBox(const ClickBox& box, const Coords& cursor){
  return cursor.first >= box.a.first && cursor.second >= box.a.second && cursor.first <= box.b.first && cursor.second <= box.b.second;
}

ClickBox::ClickBox(int x1, int y1, int x2, int y2, vector<Action*> es): a(x1, y1), b(x2, y2), effects(es){}

void ClickBox::doActions(){
  for(unsigned int i = 0; i != effects.size(); ++i){
    effects[i]->act();
  }
}

//ChangeView::ChangeView(View* v): view(v){}

ChangeView makeChangeView(View* v){
  ChangeView cv;
  cv.target = v;
  return cv;
}

void ChangeView::act(){
  state.view = target;
}

//ChangeState::ChangeState(string t, int v): target(t), value(v){}

ChangeState makeChangeState(string t, int v){
  ChangeState cs;
  cs.target = t;
  cs.value = v;
  return cs;
}

void ChangeState::act(){
  for(unsigned int i = 0; i != state.values.size(); ++i){
    if (state.values[i].first == target){
      state.values[i].second = value;
      break;
    }
  }
}
