#include <animation/te2d_animator.hpp>

te2d_animator te2d_animator::create(te2d_sprite* sprite) {
    te2d_animator anim = {};
    anim.sprite = sprite;
    return anim;
}

void te2d_animator::play(te2d_animation* anim) {
    if (current_anim == anim) return; // уже проигрывается

    current_anim = anim;
    current_frame = 0;
    timer = 0.0f;

    // Применяем первый кадр
    if (anim && anim->frame_count > 0) {
        if (sprite) {
            sprite->set_frame(anim->frames[0].uv_region);
        }
    }
}

void te2d_animator::set_state(te2d_anim_state state,
                               te2d_animation* idle,
                               te2d_animation* run,
                               te2d_animation* jump,
                               te2d_animation* attack) {
    if (state == current_state) return;
    current_state = state;

    te2d_animation* anim = nullptr;
    switch (state) {
        case te2d_anim_state::idle:   anim = idle;   break;
        case te2d_anim_state::run:    anim = run;    break;
        case te2d_anim_state::jump:   anim = jump;   break;
        case te2d_anim_state::attack: anim = attack; break;
    }

    play(anim);
}

void te2d_animator::update(f32 dt) {
    if (!current_anim || !sprite) return;
    if (current_anim->frame_count == 0) return;

    timer += dt;
    const te2d_anim_frame& frame = current_anim->get_frame(current_frame);

    if (timer >= frame.duration) {
        timer -= frame.duration;
        ++current_frame;

        // Проверка выхода за границы
        if (current_frame >= current_anim->frame_count) {
            if (current_anim->loop) {
                current_frame = 0;
            } else {
                current_frame = current_anim->frame_count - 1; // застыть
            }
        }

        // Применяем новый кадр
        const te2d_anim_frame& next_frame = current_anim->get_frame(current_frame);
        sprite->set_frame(next_frame.uv_region);
    }
}

void te2d_animator::reset() {
    current_frame = 0;
    timer = 0.0f;

    if (current_anim && current_anim->frame_count > 0 && sprite) {
        sprite->set_frame(current_anim->frames[0].uv_region);
    }
}