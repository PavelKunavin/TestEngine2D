#include <animation/te2d_animation.hpp>
#include <cstring>

te2d_animation te2d_animation::create(te2d_allocator& allocator,
                                       const char* name,
                                       const te2d_anim_frame* frames,
                                       u32 frame_count,
                                       bool loop) {
    te2d_animation anim = {};
    anim.name = name;
    anim.loop = loop;
    anim.frame_count = frame_count;

    // Копируем кадры в аллокатор
    anim.frames = allocator.alloc_type<te2d_anim_frame>(frame_count);
    if (anim.frames) {
        std::memcpy(anim.frames, frames, frame_count * sizeof(te2d_anim_frame));
    }

    return anim;
}

const te2d_anim_frame& te2d_animation::get_frame(u32 index) const {
    if (frame_count == 0) {
        static te2d_anim_frame empty = {};
        return empty;
    }

    if (loop) {
        index = index % frame_count;
    } else if (index >= frame_count) {
        index = frame_count - 1; // застыть на последнем кадре
    }

    return frames[index];
}