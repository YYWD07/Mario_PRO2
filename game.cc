#include "game.hh"
#include "assert.hh"
using namespace pro2;

Game::Game(int width, int height)
    : mario_({width / 2, 150}),
      l_platforms_{
          Platform(100, 300, 200, 211), // left_, right_, top_, bottom_
          Platform(0, 200, 60, 71),
          Platform(250, 400, 150, 161),
      },
      finished_(false) {
    assert(width > 0 && height > 0, "L'amplada i l'alcada del joc han de ser positives.");
    spawn_timer = 200;
    phantoms.spawn({width / 2, 150});
    for(auto it=l_platforms_.begin(); it!=l_platforms_.end(); it++) {
        platforms_.add(&(*it));
    }
    for (int i = 1; i < 10000; i++) {
        l_platforms_.push_back(Platform(250 + i * 200, 400 + i * 200, 150, 161));
        platforms_.add(&l_platforms_.back());
        l_platforms_.push_back(Platform(80 + i * 200, 80+150 + i * 200, 60, 71));
        platforms_.add(&l_platforms_.back());
        l_platforms_.push_back(Platform(30 + i * 200, 190 + i * 200, -30, -19));
        platforms_.add(&l_platforms_.back());
    }
    for (int i = 1; i < 10000; i++) {
        int step = rand()%21;
        for(int j=30 + i * 200; j<190 + i * 200; j+=30+step) {
            int extra = 0;
            int temp = rand()%2;
            if(temp == 1) {
                extra = -50;
            }
            l_objects_.push_back(Object({j, 148+extra}));
            objects_.add(&l_objects_.back());
            l_objects_.push_back(Object({j, 58+extra}));
            objects_.add(&l_objects_.back());
            l_objects_.push_back(Object({j, -32+extra}));
            objects_.add(&l_objects_.back());
        }
    }
}

void Game::process_keys(pro2::Window& window) {
    if (window.is_key_down(Keys::Escape)) {
        finished_ = true;
        return;
    }
}

void Game::update_objects(pro2::Window& window) {
    const set<const Platform *> r = platforms_.query(window.camera_rect());
    mario_.update(window, r);

    for(auto itr = r.begin(); itr != r.end(); itr++) {
        const_cast<Platform*>(*itr)->update();
        platforms_.update(*itr);
    }

    const set<const Object *> r2 = objects_.query(window.camera_rect());
    auto it=r2.begin();
    while(it!=r2.end()) {
        const_cast<Object*>(*it)->update(mario_.pos());
        objects_.update(*it);
        if((*it)->collected()) {
            objects_.remove(*it);
        }
        it ++;
    }

    phantoms.update(window, r);
    if(spawn_timer > 0) {
        spawn_timer --;
    } else {
        spawn_timer ++;
    }
    if(spawn_timer == 0) {
        if(phantoms.has_ghost()) {
            pro2::Pt pt = phantoms.nearest_ghost(mario_.pos());
            pro2::Rect rect = window.camera_rect();
            if(pt.x >= rect.left && pt.x <= rect.right) {
                mario_.set_pos(pt);
            }
        }
        phantoms.clear();
        spawn_timer = -30;
    }
    if(spawn_timer == -1) {
        spawn_timer = 200;
        phantoms.spawn(mario_.pos());
    }
    
    if(mario_.pos().y > 4000) {
        finished_ = true;
    }
}

void Game::update_camera(pro2::Window& window) {
    const Pt pos = mario_.pos();
    const Pt cam = window.camera_center();

    const int left = cam.x - window.width() / 4;
    const int right = cam.x + window.width() / 4;
    const int top = cam.y - window.height() / 4;
    const int bottom = cam.y + window.height() / 4;

    int dx = 0, dy = 0;
    if (pos.x > right) {
        dx = pos.x - right;
    } else if (pos.x < left) {
        dx = pos.x - left;
    }
    if (pos.y < top) {
        dy = pos.y - top;
    } else if (pos.y > bottom) {
        dy = pos.y - bottom;
    }

    window.move_camera({dx, dy});
}

void Game::update(pro2::Window& window) {
    process_keys(window);
    update_objects(window);
    update_camera(window);
}

void Game::paint(pro2::Window& window) {
    window.clear(sky_blue);
    const set<const Platform *> r = platforms_.query(window.camera_rect());
    for (const Platform * p : r) {
        p->paint(window);
    }
    const set<const Object *> r2 = objects_.query(window.camera_rect());
    for(const Object * o : r2) {
        o->paint(window);
    }
    mario_.paint(window);
    phantoms.paint(window);
}