#include "object.hh"
#include "utils.hh"

const int _ = -1;
const int r = pro2::red;
const int s = 0xecc49b;
const int b = 0x5e6ddc;
const int y = pro2::yellow;
const int h = pro2::black;
const int g = 0xaaaaaa;
const int w = 0x8d573c;

// clang-format off
const vector<vector<int>> Object::object_sprite_1 = {
    {_, _, _, y, y, y, y, y, _, _, _, _},
    {_, _, y, y, y, y, y, y, y, _, _, _},
    {_, _, y, y, y, y, y, y, y, _, _, _},
    {_, _, y, y, y, y, y, y, y, _, _, _},
    {_, _, y, y, y, y, y, y, y, _, _, _},
    {_, _, _, y, y, y, y, y, _, _, _, _},
};
const vector<vector<int>> Object::object_sprite_2 = {
    {_, _, _, _, y, y, y, _, _, _, _, _},
    {_, _, _, y, y, y, y, y, _, _, _, _},
    {_, _, _, y, y, y, y, y, _, _, _, _},
    {_, _, _, y, y, y, y, y, _, _, _, _},
    {_, _, _, y, y, y, y, y, _, _, _, _},
    {_, _, _, _, y, y, y, _, _, _, _, _},
};
const vector<vector<int>> Object::object_sprite_3 = {
    {_, _, _, _, _, y, y, _, _, _, _, _},
    {_, _, _, _, _, y, y, _, _, _, _, _},
    {_, _, _, _, _, y, y, _, _, _, _, _},
    {_, _, _, _, _, y, y, _, _, _, _, _},
    {_, _, _, _, _, y, y, _, _, _, _, _},
    {_, _, _, _, _, y, y, _, _, _, _, _},
};
// clang-format on

void Object::update(pro2::Pt mario_pos) {
    if(!collected_) {
        if(abs(mario_pos.x - pos_.x) < 10 && abs(mario_pos.y - pos_.y) < 10) {
            collected_ = true;
        }
    }
    if(increasing) {
        turn_ ++;
    } else {
        turn_ --;
    }
    if(turn_ == 3) {
        increasing = false;
    } else if(turn_ == 1) {
        increasing = true;
    }

    /*
    if(inc_move_) {
        pos_.x ++;
        move_ ++;
    } else {
        pos_.x --;
        move_ --;
    }
    if(move_ == 20) {
        inc_move_ = false;
    } else if(move_ == 0) {
        inc_move_ = true;
    }
    */
}

void Object::paint(pro2::Window& window) const {
    pro2::Pt top_left = {pos_.x-5, pos_.y-5};
    if(turn_ == 1) {
        paint_sprite(window, top_left, object_sprite_1, false);
    } else if(turn_ == 2) {
        paint_sprite(window, top_left, object_sprite_2, false);
    } else if(turn_ == 3) {
        paint_sprite(window, top_left, object_sprite_3, false);
    }
}