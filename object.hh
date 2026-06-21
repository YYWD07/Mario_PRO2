#ifndef COIN_HH
#define COIN_HH

#include "window.hh"
#include <vector>
using namespace std;

class Object {
    pro2::Pt pos_;
    bool collected_;
    int turn_ = 1;
    bool increasing = true;
    int move_ = 0;
    bool inc_move_ = true;

public:
    Object(pro2::Pt pos): pos_(pos), collected_(false) {}

    void update(pro2::Pt mario_pos);
    void paint(pro2::Window& window) const;
    bool collected() const {return collected_;}
    pro2::Rect get_rect() const {return {pos_.x-5, pos_.y-5, pos_.x + 5, pos_.y};}

 private:
    static const vector<vector<int>> object_sprite_1;
    static const vector<vector<int>> object_sprite_2;
    static const vector<vector<int>> object_sprite_3;
};

#endif