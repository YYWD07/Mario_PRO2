# Window 类名 - 管理游戏窗口的类

# window.hh 函数解析
- is_key_down, 键是否被按下, 只要按住就不停触发, 持续移动方向、持续按住加速
- was_key_pressed, 键是否刚被按下, 按下的那一瞬间触发一次, 跳跃、发射单发子弹、打开菜单
- is_modkey_down, 控制键是否按下, 按住特定的控制键就触发, 通常用来做组合键。
  比如你想实现“按住 Shift 键 + 方向键 = 加速跑”，你就会用到它来检查 Shift 键的状态。
- window.hh: Rect camera_rect() const; 这个函数用于计算相机当前可见的游戏世界区域（视口矩形），
  返回{left, top, right, bottom} 四个边界值。 
- void Window::set_pixel(Pt pt, Color color); Pintar punt a punt (x,y) de tipus Pt
- void Window::clear(Color color) pintar tota la pantalla de color