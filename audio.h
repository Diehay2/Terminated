#ifndef AUDIO_H
#define AUDIO_H

#include "SDL_utils.h"

class Audio {
public:
    Audio()
        : shootSound(nullptr), deathSound(nullptr), upgradeSound(nullptr),
          loseSound(nullptr), bgm(nullptr), hurtSound(nullptr), isMuted(false) {}

    ~Audio() {
        cleanUp();
    }

    bool init() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;

        shootSound = Mix_LoadWAV("Assets/Audio/shoot.ogg");
        deathSound = Mix_LoadWAV("Assets/Audio/death.ogg");
        upgradeSound = Mix_LoadWAV("Assets/Audio/pick_up.ogg");
        loseSound = Mix_LoadWAV("Assets/Audio/lose.ogg");
        hurtSound = Mix_LoadWAV("Assets/Audio/hurt.ogg");
        bgm = Mix_LoadMUS("Assets/Audio/game_music.mp3");

        return shootSound && deathSound && upgradeSound && loseSound && hurtSound && bgm;
    }

    void playShoot() {
        if (!isMuted && shootSound) Mix_PlayChannel(-1, shootSound, 0);
    }

    void playDeath() {
        if (!isMuted && deathSound) Mix_PlayChannel(-1, deathSound, 0);
    }

    void playUpgrade() {
        if (!isMuted && upgradeSound) Mix_PlayChannel(-1, upgradeSound, 0);
    }

    void playLose() {
        if (!isMuted && loseSound) Mix_PlayChannel(-1, loseSound, 0);
    }

    void playHurt() {
        if (!isMuted && hurtSound) Mix_PlayChannel(-1, hurtSound, 0);
    }

    void playMusic(bool loop = true) {
        if (!isMuted && bgm) Mix_PlayMusic(bgm, loop ? -1 : 1);
    }

    void stopMusic() {
        Mix_HaltMusic();
    }

    void setMuted(bool muted) {
        isMuted = muted;
        Mix_Volume(-1, muted ? 0 : MIX_MAX_VOLUME);
        Mix_VolumeMusic(muted ? 0 : MIX_MAX_VOLUME);
    }

    void toggleMute() {
        setMuted(!isMuted);
    }

    void cleanUp() {
        if (shootSound) {
            Mix_FreeChunk(shootSound);
            shootSound = nullptr;
        }
        if (deathSound) {
            Mix_FreeChunk(deathSound);
            deathSound = nullptr;
        }
        if (upgradeSound) {
            Mix_FreeChunk(upgradeSound);
            upgradeSound = nullptr;
        }
        if (loseSound) {
            Mix_FreeChunk(loseSound);
            loseSound = nullptr;
        }
        if (hurtSound) {
            Mix_FreeChunk(hurtSound);
            hurtSound = nullptr;
        }
        if (bgm) {
            Mix_FreeMusic(bgm);
            bgm = nullptr;
        }
        Mix_CloseAudio();
    }

private:
    Mix_Chunk* shootSound;
    Mix_Chunk* deathSound;
    Mix_Chunk* upgradeSound;
    Mix_Chunk* loseSound;
    Mix_Chunk* hurtSound;
    Mix_Music* bgm;
    bool isMuted;
};

#endif

