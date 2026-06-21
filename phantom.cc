#include "phantom.hh"
#include "utils.hh"

using namespace std;

const int _ = -1;
const int r = pro2::red;
const int s = 0xecc49b;
const int b = 0x5e6ddc;
const int y = pro2::yellow;
const int h = pro2::black;
const int g = 0xaaaaaa;
const int w = 0x8d573c;

// clang-format off
const vector<vector<int>> Phantom::phantom_sprite_normal_ = {
    {_, _, _, r, r, r, r, r, _, _, _, _},
    {_, _, r, r, r, r, r, r, r, r, r, _},
    {_, _, h, h, h, s, s, h, s, _, _, _},
    {_, h, s, h, s, s, s, h, s, s, s, _},
    {_, h, s, h, h, s, s, s, h, s, s, s},
    {_, h, h, s, s, s, s, h, h, h, h, _},
    {_, _, _, s, s, s, s, s, s, s, _, _},
    {_, _, r, r, b, r, r, r, _, _, _, _},
    {_, r, r, r, b, r, r, b, r, r, r, _},
    {r, r, r, r, b, b, b, b, r, r, r, r},
    {g, g, r, b, y, b, b, y, b, r, g, g},
    {g, g, g, b, b, b, b, b, b, g, g, g},
    {g, g, b, b, b, b, b, b, b, b, g, g},
    {_, _, b, b, b, _, _, b, b, b, _, _},
    {_, w, w, w, _, _, _, _, w, w, w, _},
    {w, w, w, w, _, _, _, _, w, w, w, w},
};
// clang-format on

void Phantom::spawn(pro2::Pt mario_pos) {
    insert_end(new Item(mario_pos, -3, -2));
    insert_end(new Item(mario_pos, 3, -2));
    insert_end(new Item(mario_pos, 0, -2));
}

void Phantom::paint(pro2::Window& w) {
    for(Item *t = inf_.next; t != &sup_; t=t->next) {
        if(t->is_ghost) {
            t->count ++;
            if(t->count == 13) {
                t->paint_ = !t->paint_;
                t->count = 0;
            }
            if(t->paint_) {
                pro2::Pt top_left = {t->pos_.x-5, t->pos_.y-15};
                paint_sprite(w, top_left, phantom_sprite_normal_, false);
            }
        } else {
            for(int dy=0; dy<6; dy++) {
                for(int dx=0; dx<6; dx++) {
                    w.set_pixel({t->pos_.x+dx, t->pos_.y+dy}, 0xffffff);
                }
            }
        }
    }
}

void Phantom::update(pro2::Window& w, const std::set<const Platform *>& p) {
    Item *t = inf_.next;
    while(t != &sup_) {
        Item *temp = t->next;
        if(!t->is_ghost) {
            t->pos_.x += t->vx_;
            t->pos_.y += t->vy_;
            pro2::Rect rect = w.camera_rect();
            bool out = t->pos_.x > rect.right or t->pos_.x < rect.left 
                        or t->pos_.y < rect.top or t->pos_.y > rect.bottom;
            if(out) {
                erase(t);
            } else {
                for(auto it = p.begin(); it != p.end(); it++) {
                    if((*it)->is_pt_inside(t->pos_)) {
                        t->pos_.y = (*it)->top();
                        t->is_ghost = true;
                        t->vx_ = 0;
                        t->vy_ = 0;
                    }
                } 
            }
        }
        t = temp;
    }
}

void Phantom::clear() {
    Item *t = inf_.next;
    while(t != &sup_) {
        Item *temp = t->next;
        delete t;
        t = temp;
    }
    inf_.next = &sup_;
    sup_.prev = &inf_;
}

bool Phantom::has_ghost() {
    for(Item *t = inf_.next; t != &sup_; t=t->next) {
        if(t->is_ghost) {
            return true;
        }
    }
    return false;
}

pro2::Pt Phantom::nearest_ghost(pro2::Pt mario_pos) {
    pro2::Pt best = {0, 0};
    int d = 0;
    bool first = true;
    for(Item *t = inf_.next; t != &sup_; t=t->next) {
        if(t->is_ghost) {
            int temp = abs(t->pos_.x - mario_pos.x) + abs(t->pos_.y - mario_pos.y);
            if(first or temp < d) {
                d = temp;
                first = false;
                best = t->pos_;
            }
        }
    }
    return best;
}