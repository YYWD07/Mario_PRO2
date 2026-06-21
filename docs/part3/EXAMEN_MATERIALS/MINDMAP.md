1. # phantom.hh
<!-- 
#ifndef ...
#include "window.hh", "platform.hh" , <set>，<vector>
class Phantom {
    class Item {
    public:
        位置，速度，是否幻影，nextprev和两个构造函数
    };

    两个哨兵没有size
    insert, erase 函数
public:
    构造+析构
    7个函数：初始化球，打印，更新，是否有幻影，距离最近幻影，清除，是否空.

 private:
    static const std::vector<std::vector<int>> mario_sprite_; 从mario.hh复制过来改个名
};
#endif 
-->

2. # phantom.cc
