### `phantom.hh`

```cpp
#ifndef PHANTOM_HH
#define PHANTOM_HH
#include "window.hh"
#include "platform.hh"
#include <set>

class Phantom {
    class Item {
    public:
        pro2::Pt pos_;
        int vx_, vy_;
        bool is_ghost_;     // false=飞行球, true=幻影
        Item *next_, *prev_;
        Item():vx_(0),vy_(0),is_ghost_(false),next_(nullptr),prev_(nullptr){}
        Item(pro2::Pt p,int vx,int vy)
            :pos_(p),vx_(vx),vy_(vy),is_ghost_(false),next_(nullptr),prev_(nullptr){}
    };

    Item inf_, sup_; // 哨兵：inf_<->节点...<->sup_

    void insert_(Item* it){
        it->prev_=sup_.prev_; it->next_=&sup_;
        sup_.prev_->next_=it; sup_.prev_=it;
    }
    void erase_(Item* it){
        it->prev_->next_=it->next_;
        it->next_->prev_=it->prev_;
        delete it;
    }

public:
    Phantom(){ inf_.next_=&sup_; sup_.prev_=&inf_; }
    ~Phantom(){ clear(); }

    void spawn(pro2::Pt pos);
    void update(pro2::Rect screen, const std::set<const Platform*>& plats);
    void paint(pro2::Window& w, pro2::Pt mario_pos) const;
    void clear();
    bool empty() const { return inf_.next_==&sup_; }
    bool has_ghost() const;
    pro2::Pt nearest_ghost(pro2::Pt mp) const;
};
#endif
```

---

### `phantom.cc`

```cpp
#include "phantom.hh"
using namespace pro2;

void Phantom::spawn(Pt pos){
    insert_(new Item(pos, -5, -6)); // 左上
    insert_(new Item(pos,  0, -8)); // 正上
    insert_(new Item(pos,  5, -6)); // 右上
}

void Phantom::clear(){
    Item* c=inf_.next_;
    while(c!=&sup_){ Item* t=c->next_; delete c; c=t; }
    inf_.next_=&sup_; sup_.prev_=&inf_;
}

void Phantom::update(Rect s, const std::set<const Platform*>& plats){
    Item* c=inf_.next_;
    while(c!=&sup_){
        Item* nxt=c->next_;
        if(not c->is_ghost_){ 
            c->pos_.x+=c->vx_; c->pos_.y+=c->vy_;
            bool out = c->pos_.x<s.left-20 or c->pos_.x>s.right+20
                    or c->pos_.y<s.top-20  or c->pos_.y>s.bottom+20;
            if(out){ erase_(c); }
            else {
                for(const Platform* p:plats){
                    if(p->is_pt_inside(c->pos_)){
                        c->pos_.y=p->top(); c->vx_=0; c->vy_=0;
                        c->is_ghost_=true;
                    }
                }
            }
        }
        c=nxt;
    }
}

bool Phantom::has_ghost() const {
    for(Item* c=inf_.next_;c!=&sup_;c=c->next_)
        if(c->is_ghost_) return true;
    return false;
}

Pt Phantom::nearest_ghost(Pt mp) const {
    Pt best={0,0}; int bd=0; bool first=true;
    for(Item* c=inf_.next_;c!=&sup_;c=c->next_){
        if(c->is_ghost_){
            int d=abs(c->pos_.x-mp.x)+abs(c->pos_.y-mp.y);
            if(first or d<bd){ bd=d; best=c->pos_; first=false; }
        }
    }
    return best;
}

void Phantom::paint(Window& w, Pt mp) const {
    for(Item* c=inf_.next_;c!=&sup_;c=c->next_){
        if(c->is_ghost_){
            // 棋盘格幻影（浅蓝白12x16）
            for(int dy=0;dy<16;dy++) for(int dx=0;dx<12;dx++)
                w.set_pixel({c->pos_.x-6+dx,c->pos_.y-15+dy},
                            (dx+dy)%2==0?0xddeeff:0xaabbdd);
            // 连线Mario→幻影
            int dx=c->pos_.x-mp.x, dy=c->pos_.y-mp.y;
            int steps=std::max(abs(dx),abs(dy));
            if(steps>0) for(int i=0;i<=steps;i++)
                w.set_pixel({mp.x+dx*i/steps,mp.y+dy*i/steps},0xffffff);
        } else {
            // 飞行球：4x4白块
            for(int dy=0;dy<4;dy++) for(int dx=0;dx<4;dx++)
                w.set_pixel({c->pos_.x+dx,c->pos_.y+dy},0xffffff);
        }
    }
}
```

---

### `mario.hh` +1行

```cpp
void set_pos(pro2::Pt p){ pos_=p; last_pos_=p; } // 瞬移用
```

---

### `game.hh` +3行

```cpp
#include "phantom.hh"
    Phantom phantoms_;
    int     spawn_timer_; // 倒计时：发射后300帧触发瞬移，之后60帧再发射
```

---

### `game.cc`

**初始化列表加：**
```cpp
spawn_timer_(0)
```

**构造函数体：**
```cpp
for(const Platform& p:v_platforms_) platforms_.add(&p);
phantoms_.spawn({WIDTH/2,150}); // 游戏开始立刻发射
spawn_timer_=300;               // 5秒后瞬移
```

**`update_objects()`末尾：**
```cpp
const std::set<const Platform*> vis=print_area<Platform>(window,platforms_);
phantoms_.update(window.camera_rect(),vis);

if(spawn_timer_<0) {
    spawn_timer_++;
} else {
    spawn_timer_--;
}

// 5秒到：瞬移到最近幻影，清空，等1秒再发射
if(spawn_timer_==0){
    if(phantoms_.has_ghost())
        mario_.set_pos(phantoms_.nearest_ghost(mario_.pos()));
    phantoms_.clear();
    spawn_timer_=-60; // -60到0这段是等待1秒
}
// 等待结束：重新发射
if(spawn_timer_==-1){
    phantoms_.spawn(mario_.pos());
    spawn_timer_=300;
}

// Mario掉过死亡线
if(mario_.pos().y>400){
    finished_=true;
}
```

**`paint()`加：**
```cpp
phantoms_.paint(window, mario_.pos());
```

---

**行数：**

| 文件 | 行 |
|------|----|
| phantom.hh | 38 |
| phantom.cc | 55 |
| game.cc新增 | 15 |
| game.hh+mario.hh | 4 |
| **合计** | **112行** |