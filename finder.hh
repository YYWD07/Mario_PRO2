#ifndef FINDER_HH
#define FINDER_HH

#include <set>
#include <map>
#include "geometry.hh"
using namespace std;

template<class T>
class Finder {
    map<int, set<const T *>> objects_;
    map<const T *, pair<int, int>> cache_;
    multiset<int> length_counts_;

public:
    void add(const T *t) {
        remove(t);
        pro2::Rect rect = t->get_rect();
        objects_[rect.left].insert(t);
        cache_[t] = {rect.left, rect.right};
        length_counts_.insert(rect.right - rect.left);
    }

    void remove(const T *t) {
        auto it = cache_.find(t);
        if(it != cache_.end()) {
            int left = it->second.first;
            int right = it->second.second;
            objects_[left].erase(t);
            if(objects_[left].empty()) {objects_.erase(left);}
            auto it2 = length_counts_.find(right - left);
            if(it2 != length_counts_.end()) {
                length_counts_.erase(it2);
            }
            cache_.erase(it);
        }
    }

    void update(const T *t) {
        add(t);
    }

    const set<const T *> query(pro2::Rect rect) const {
        set<const T *> result;
        int max_length = length_counts_.empty()? 0: *length_counts_.rbegin();
        auto it = objects_.lower_bound(rect.left - max_length);
        while(it != objects_.end() && it->first <= rect.right) {
            for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                pro2::Rect rect2 = (*it2)->get_rect();
                if(rect2.top <= rect.bottom && rect2.bottom >= rect.top && rect2.right >= rect.left) {
                    result.insert(*it2);
                }
            }
            it ++;
        }
        return result;
    }
};

#endif