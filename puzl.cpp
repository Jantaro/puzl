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
struct ViewState{
  Sprite sprite;
  vector<ClickBox> boxen;
};
struct View{
  void setState();
  vector<ViewState> states;
};
struct State{
  View* view;
  ViewState* viewState;
  vector<pair<string, unsigned int> > values;
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
ChangeState makeChangeState(string, unsigned int);

State state;
View pedestal;
ViewState pedestal_button_up;
ViewState pedestal_button_down;
Sprite imageNotFound;

int main()
{
  sf::RenderWindow App(sf::VideoMode(512, 512, 32), "puzl");

  App.SetFramerateLimit(60);
  App.ShowMouseCursor(false);

  sf::Image cursorImage;
  cursorImage.LoadFromFile("cursor.png");
  cursorImage.SetSmooth(false);
  Sprite cursorDefault;
  cursorDefault.SetImage(cursorImage);
  cursorDefault.SetSubRect(sf::IntRect(0, 0, 16, 16));

  sf::Image Image_1;
  sf::Image Image_2;
  sf::Image notFound;
  Image_1.LoadFromFile("pedestal_close_button-up.png");
  Image_2.LoadFromFile("pedestal_close_button-down.png");
  notFound.LoadFromFile("image_not_found.png");
  Sprite Sprite_1;
  Sprite Sprite_2;
  Sprite_1.SetImage(Image_1);
  Sprite_2.SetImage(Image_2);
  imageNotFound.SetImage(notFound);

  pedestal_button_up.sprite = Sprite_1;
  pedestal_button_down.sprite = Sprite_2;
  ChangeState pushButton = makeChangeState("pedestal_button", 1);
  ChangeState releaseButton = makeChangeState("pedestal_button", 0);
  vector<Action*> act1 = {&pushButton};
  vector<Action*> act2 = {&releaseButton};
  pedestal_button_up.boxen = vector<ClickBox>{ClickBox(230, 64, 282, 189, act1)};
  pedestal_button_down.boxen = vector<ClickBox>{ClickBox(230, 64, 282, 189, act2)};

  pedestal.states.push_back(pedestal_button_up);
  pedestal.states.push_back(pedestal_button_down);
  state.view = &pedestal;
  state.viewState = &pedestal_button_up;
  state.values.push_back(pair<string, unsigned int>("pedestal_button", 0));

  Coords cursor;

  while (App.IsOpened()){
    sf::Event Event;
    while (App.GetEvent(Event)){
      if (Event.Type == sf::Event::Closed)
        App.Close();
      if (Event.Type == sf::Event::MouseButtonPressed){
        cursor.first = Event.MouseButton.X;
        cursor.second = Event.MouseButton.Y;
        //std::cout << cursor.first << " " << cursor.second << std::endl;
        for(unsigned int i = 0; i != state.viewState->boxen.size(); ++i){
          if (inClickBox(state.viewState->boxen[i], cursor)){
            state.viewState->boxen[i].doActions();
            state.view->setState();
            break;
          }
        }
      }
    }

    //std::cout << App.GetInput().GetMouseX() << " " << App.GetInput().GetMouseY() << std::endl;
    cursorDefault.SetPosition(App.GetInput().GetMouseX()-7, App.GetInput().GetMouseY()-7);
    App.Clear();
    App.Draw(state.viewState->sprite);
    App.Draw(cursorDefault);
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

void View::setState(){
  for(unsigned int i = 0; i != state.values.size(); ++i){
    if (state.values[i].first == "pedestal_button"){
      state.viewState = &states[state.values[i].second];
    }
  }
  //state.viewState = imageNotFound;
  //TODO: needs to error somehow if variable not found
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

ChangeState makeChangeState(string t, unsigned int v){
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
  //TODO: needs to error somehow if it doesn't find the variable
}
