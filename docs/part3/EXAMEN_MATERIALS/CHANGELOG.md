1. # phantom.hh
<!-- 

#ifndef PHANTOM_HH
#define PHANTOM_HH

#include "window.hh"
#include "platform.hh"
#include <set>
#include <vector>

class Phantom {
    class Item {
    public:
        pro2::Pt pos_;
        int vx_, vy_;
        bool is_ghost_;
        bool sparkle=false; int count=0;
        Item *next, *prev;
        Item(): pos_({0, 0}), vx_(0), vy_(0), is_ghost_(false), next(nullptr), prev(nullptr) {}
        Item(pro2::Pt pos, int vx, int vy): 
        pos_(pos), vx_(vx), vy_(vy), is_ghost_(false), next(nullptr), prev(nullptr) {}
    };

    Item sup_, inf_;

    void insert(Item *it) {
        Item *last = sup_.prev;
        last->next = it;
        it->prev = last;
        it->next = &sup_;
        sup_.prev = it;
    }

    void erase(Item *it) {
        Item *before = it->prev;
        Item *after = it->next;
        before->next = after;
        after->prev = before;
        delete it;
    }

public:
    Phantom() {sup_.prev = &inf_; inf_.next = &sup_;}
    ~Phantom() {clear();}

    void spawn(pro2::Pt pt);
    void update(pro2::Rect rect, const std::set<const Platform *>& s);
    void paint(pro2::Window& w, pro2::Pt mario_pos) const;
    void clear();
    bool has_ghost();
    bool empty() const {return inf_.next == &sup_;}
    pro2::Pt nearest_ghost(pro2::Pt pt) const;

 private:
    static const std::vector<std::vector<int>> mario_sprite_;
};

#endif
-->

2. # phantom.cc
<!-- 

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

const vector<vector<int>> Phantom::mario_sprite_ = {
    {_, _, _, r, _, r, _, r, _, _, _, _},
    {_, _, r, r, r, r, r, r, r, r, r, _},
    {_, _, h, h, _, s, s, w, s, _, _, _},
    {_, h, s, h, s, s, s, h, s, w, s, _},
    {_, h, s, w, w, s, _, s, h, w, s, s},
    {_, h, _, s, s, _, s, h, h, h, h, _},
    {_, _, _, s, s, s, s, s, s, s, _, _},
    {_, _, r, r, b, r, r, r, _, _, _, _},
    {_, r, _, _, b, _, r, b, r, r, r, _},
    {r, r, r, r, b, b, b, b, r, r, r, r},
    {g, g, _, b, _, b, _, y, b, _, g, g},
    {g, g, g, b, w, b, _, _, b, g, g, g},
    {g, g, b, _, b, b, b, b, _, b, g, g},
    {_, _, b, b, b, _, _, b, b, b, _, _},
    {_, w, _, w, _, _, _, _, _, w, w, _},
    {w, _, w, w, _, _, _, _, w, w, w, w},
};

void Phantom::spawn(pro2::Pt pt) {
    insert(new Item(pt, -3, -4));
    insert(new Item(pt, 3, -4));
    insert(new Item(pt, 0, -6));
}

void Phantom::clear() {
    Item *c = inf_.next;
    while(c!=&sup_) {
        Item *temp = c->next;
        delete c;
        c = temp;
    }
    inf_.next = &sup_;
    sup_.prev = &inf_;
}

bool Phantom::has_ghost() {
    for(Item *it=inf_.next; it!=&sup_; it=it->next) {
        if(it->is_ghost_) return true;
    }
    return false;
}

pro2::Pt Phantom::nearest_ghost(pro2::Pt pt) const{
    pro2::Pt best = {0, 0}; int distance = 0; bool first = true; 
    for(Item *it=inf_.next; it!=&sup_; it=it->next) {
        if(it->is_ghost_) {
            int temp = abs(it->pos_.x - pt.x) + abs(it->pos_.y - pt.y);
            if(first or temp < distance) {
                distance = temp; first = false; best = it->pos_;
            }
        }
    }
    return best;
}

void Phantom::update(pro2::Rect rect, const std::set<const Platform *>& s) {
    Item *c = inf_.next;
    while(c != &sup_) {
        Item *n = c->next;
        if(!c->is_ghost_) {
           c->pos_.x += c->vx_; c->pos_.y += c->vy_; 
           bool out = c->pos_.x > rect.right+20 or c->pos_.x < rect.left-20
           or c->pos_.y < rect.top-20 or c->pos_.y > rect.bottom+20;
            if(out) {
                erase(c);
            } else {
                for(const Platform *p: s) {
                    if(p->is_pt_inside(c->pos_)) {
                        c->pos_.y = p->top();
                        c->vx_ = 0;
                        c->vy_ = 0;
                        c->is_ghost_ = true;
                    }
                }
            }
        } 
        c = n;
    }
}

void Phantom::paint(pro2::Window& w, pro2::Pt mario_pos) const {
    for(Item *it=inf_.next; it!=&sup_; it=it->next) {
        if(it->is_ghost_) {
            it->count ++;
            if(it->count == 13) {
                it->sparkle = !it->sparkle;
                it->count = 0;
            }
            if(!it->sparkle) {
                pro2::Pt top_left = {it->pos_.x-6, it->pos_.y - 15};
                paint_sprite(w, top_left, mario_sprite_, false);
            }
        } else {
            for(int dy=0; dy<4; dy++) {
                for(int dx=0; dx<4; dx++) {
                    w.set_pixel({it->pos_.x+dx, it->pos_.y+dy}, 0xffffff);
                }
            }
        }   
    }
}
-->

3. # mario.hh
<!-- 
void set_pos(pro2::Pt pt) {pos_ = pt; last_pos_ = pt;} 
-->

4. # game.hh
<!-- 
#include "phantom.hh"
Phantom phantoms_;
int spawn_timer_; 
-->

5. # game.cc
- 初始化列表:
<!-- 
spawn_timer_(0) 

v_platforms_ {
    Platform(100, 300, 200, 211),
    Platform(0, 200, 60, 71),
    Platform(250, 400, 150, 161), 
},
-->

- 构造函数体:
<!-- 
for(const Coin& c: v_coins_) {
    coins_.add(&c);
    phantoms_.spawn({width / 2, 150});
    spawn_timer_ = 200;
} 

for (int i = 1; i < 30000; i++) {
    v_platforms_.push_back(Platform(250 + i * 200, 400 + i * 200, 150, 161)); // 左，右，上，下
    platforms_.add(&v_platforms_.back());

    v_platforms_.push_back(Platform(80 + i * 200, 230 + i * 200, 60, 71)); // 左，右，上，下
    platforms_.add(&v_platforms_.back());

    v_platforms_.push_back(Platform(30 + i * 200, 180 + i * 200, -30, -19)); // 左，右，上，下
    platforms_.add(&v_platforms_.back());
-->

- update_objects()末尾:
<!-- 
void Game::update_objects(pro2::Window& window) {
    const std::set<const Platform *> result = print_area<Platform>(window, platforms_);
    mario_.update(window, result);
    mario2_.update(window, result);     // 更新mario2_的移动，动作

    phantoms_.update(window.camera_rect(), result);

    if(spawn_timer_ > 0) {
        spawn_timer_ --;
    } else {
        spawn_timer_ ++;
    }

    if(spawn_timer_ == 0) {
        if(phantoms_.has_ghost()) {
            pro2:: Pt pt = phantoms_.nearest_ghost(mario_.pos());
            if(!(pt.x < window.camera_rect().left)) {
                mario_.set_pos(pt);
            }
            phantoms_.clear();
            spawn_timer_ = -60;
        } else {
            spawn_timer_ = -1;
        }
    }
    
    if(spawn_timer_ == -1) {
        phantoms_.spawn(mario_.pos());
        spawn_timer_ = 200;
    }

    if(mario_.pos().y > 1000) {
        finished_ = true;
    }
}
-->

- paint()加：
<!-- 
phantoms_.paint(window, mario_.pos()); 
-->

6. # 如果有时间就把sprite加一些_或者w

