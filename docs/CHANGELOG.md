# 新功能 Part0
1. Ex0.1. 按下P暂停，再次按下P继续。55daf6d
2. Ex0.2. 增加一个mario2_（在mario_左侧）, 和mario_同时移动，同样动作。50c7a95
3. Ex0.3. mario2_用WAD控制，mario_用LeftRightSpace控制，摄像机跟着mario_。fb0647e
4. Ex0.4. 镜头一直保持mario_在中心，持续跟随移动。e052c2d
5. Ex0.5-0.8. 根据相机位置，画出游戏界面边界框。在util.cc增加了paint_rect, 作用是画出长方形。
   在game.hh增加成员变量，使得长方形可以闪烁。19a69db

# 新功能 Part1
- 创建coin.cc和coin.hh, 实现打印硬币，旋转，触碰机制。在game.cc里面构造，实例化。b6fae44
- 优化了从台子底部穿模的问题。(具体在platform和mario.cc里的update函数) 47841f4
- 优化了硬币的随机生成逻辑，间距和随机性问题。(具体在game.cc 和 main.cc)   (较难) 

# 新功能 Part2
- finder.hh
- 硬币和平台都创建了finder对象

# Part3
- 传送...