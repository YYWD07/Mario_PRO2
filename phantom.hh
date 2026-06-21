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
        bool is_ghost;
        bool paint_ = true;
        int count = 0;
        Item *next;
        Item *prev;
        Item(): pos_({0,0}), vx_(0), vy_(0), is_ghost(false), prev(nullptr), next(nullptr) {}
        Item(pro2::Pt pos, int vx, int vy) :
        pos_(pos), vx_(vx), vy_(vy), is_ghost(false), prev(nullptr), next(nullptr) {}
    };
    
    Item inf_, sup_;

    void insert_end(Item *t) {
        Item *last = sup_.prev;
        last->next = t;
        t->prev = last;
        t->next = &sup_;
        sup_.prev = t;
    }

    void erase(Item *t) {
        Item *before = t->prev;
        Item *after = t->next;
        before->next = after;
        after->prev = before;
        delete t;
    }

public:
    Phantom() {inf_.next = &sup_; sup_.prev = &inf_;}
    ~Phantom() {clear();}

    void spawn(pro2::Pt mario_pos);
    void paint(pro2::Window& w);
    void update(pro2::Window& w, const std::set<const Platform *>& p);
    void clear();
    bool empty() {return inf_.next == &sup_;}
    bool has_ghost();
    pro2::Pt nearest_ghost(pro2::Pt mario_pos);

 private:
    static const std::vector<std::vector<int>> phantom_sprite_normal_;
};

#endif