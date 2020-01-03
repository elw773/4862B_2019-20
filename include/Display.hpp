#ifndef _DISPLAY_HPP_
#define _DISPLAY_HPP_

namespace Display{
  class DisplayBtn{
    bool wasPressed = false;
    std::function<bool(void)> isPressed;
  public:
    DisplayBtn(std::function<bool(void)> isPressed);
    bool isClicked(void);
  };
  extern DisplayBtn upBtn;

  extern DisplayBtn dnBtn;

  void init();

  void update();
};

#endif //_DISPLAY_HPP_
