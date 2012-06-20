#include "data.h"
#include <iostream>
using std::cerr;
//from <SFML/Graphics.hpp>
using sf::Sprite;
using sf::RenderWindow;
//from <utility>
using std::pair;
//from <vector>
using std::vector;
using std::string;

bool inClickBox(const ClickBox&, const Coords&);
Action makeChangeView(View*);
Action makeChangeState(string, unsigned int);

State state;
View pedestal;
ViewState pedestal_button_up;
ViewState pedestal_button_down;

int main()
{
  sf::RenderWindow App(sf::VideoMode(512, 512, 32), "puzl");

  App.SetFramerateLimit(60);
  App.ShowMouseCursor(false);

  sf::Image cursorImage;
  cursorImage.LoadFromFile("cursor.png");
  cursorImage.SetSmooth(false);
  Sprite cursorSet;
  Sprite cursorPointer;
  Sprite cursorAction;
  cursorPointer.SetImage(cursorImage);
  cursorAction.SetImage(cursorImage);
  cursorPointer.SetSubRect(sf::IntRect(0, 0, 16, 16));
  cursorAction.SetSubRect(sf::IntRect(15, 0, 32, 16));

  sf::Image Image_1;
  sf::Image Image_2;
  Image_1.LoadFromFile("pedestal_close_button-up.png");
  Image_2.LoadFromFile("pedestal_close_button-down.png");
  Sprite Sprite_1;
  Sprite Sprite_2;
  Sprite_1.SetImage(Image_1);
  Sprite_2.SetImage(Image_2);

  pedestal_button_up.sprite = Sprite_1;
  pedestal_button_down.sprite = Sprite_2;
  vector<Action> pushButton = {makeChangeState("pedestal_button", 1)};
  vector<Action> releaseButton = {makeChangeState("pedestal_button", 0)};
  pedestal_button_up.boxen = vector<ClickBox>{ClickBox(230, 64, 282, 189, pushButton)};
  pedestal_button_down.boxen = vector<ClickBox>{ClickBox(230, 64, 282, 189, releaseButton)};

  pedestal.states.push_back(pedestal_button_up);
  pedestal.states.push_back(pedestal_button_down);
  state.view = &pedestal;
  state.viewState = &pedestal_button_up;
  state.values.push_back(pair<string, unsigned int>("pedestal_button", 0));

  Coords cursor;

  while (App.IsOpened()){
    cursorSet = cursorPointer;
    cursor.first = App.GetInput().GetMouseX()-7;
    cursor.second = App.GetInput().GetMouseY()-7;
    ClickBox box(0,0,0,0,vector<Action>());
    for(unsigned int i = 0; i != state.viewState->boxen.size(); ++i){
      if (inClickBox(state.viewState->boxen[i], cursor)){
        cursorSet = cursorAction;
        box = state.viewState->boxen[i];
        break;
      }
    }
    sf::Event Event;
    while (App.GetEvent(Event)){
      if (Event.Type == sf::Event::Closed)
        App.Close();
      if (Event.Type == sf::Event::MouseButtonPressed){
        box.doActions();
        state.view->setState();
      }
    }

    //std::cout << App.GetInput().GetMouseX() << " " << App.GetInput().GetMouseY() << std::endl;
    cursorSet.SetPosition(cursor.first, cursor.second);
    App.Clear();
    App.Draw(state.viewState->sprite);
    App.Draw(cursorSet);
    App.Display();
  }

  return EXIT_SUCCESS;
}

bool inClickBox(const ClickBox& box, const Coords& cursor){
  return cursor.first >= box.a.first && cursor.second >= box.a.second && cursor.first <= box.b.first && cursor.second <= box.b.second;
}

ClickBox::ClickBox(int x1, int y1, int x2, int y2, vector<Action> es): a(x1, y1), b(x2, y2), effects(es){}

void ClickBox::doActions(){
  for(unsigned int i = 0; i != effects.size(); ++i){
    effects[i].act();
  }
}

void View::setState(){ //chooses which ViewState with the value of the "pedestal_button" variable
  for(unsigned int i = 0; i != state.values.size(); ++i){
    if (state.values[i].first == "pedestal_button"){ // TODO hardcoded
      state.viewState = &states[state.values[i].second];
    }
  }
  //TODO: needs to error somehow if variable not found
}

Action makeChangeView(View* v){
  Action cv;
  cv.type = ChangeView;
  cv.view = v;
  return cv;
}

Action makeChangeState(string t, unsigned int v){
  Action cs;
  cs.type = ChangeState;
  cs.target = t;
  cs.value = v;
  return cs;
}

void Action::act(){
  switch (type) {
    case ChangeView  : {state.view = view; break;}
    case ChangeState :
      {for(unsigned int i = 0; i != state.values.size(); ++i){
         if (state.values[i].first == target){
           state.values[i].second = value;
           break;
         }
       } break;
      }
    default : {cerr << "invalid Action type!";}
  }
}
