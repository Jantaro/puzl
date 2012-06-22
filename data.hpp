#ifndef GUARD_data
#define GUARD_data
#include <SFML/Graphics.hpp>
#include <utility> //for std::pair
#include <vector>
#include <string>

typedef std::pair<int, int> Coords;
struct View; // forward declaration so Action knows about View
enum ActionType {ChangeView, ChangeState};
struct Action{
  void act();
  enum ActionType type;
  //viewData
  View* view;
  //stateData
  std::string target;
  unsigned int value;
};
struct ClickBox{
  ClickBox(int, int, int, int, std::vector<Action>);
  void doActions();
  Coords a;
  Coords b;
  std::vector<Action> effects;
};
struct ViewState{
  sf::Sprite sprite;
  std::vector<ClickBox> boxen;
};
struct View{
  void setState();
  std::vector<ViewState> states;
};
struct State{
  View* view;
  ViewState* viewState;
  std::vector<std::pair<std::string, unsigned int> > values;
};
#endif
