#pragma once
#include <string>
#include "raylib.h"
#include <map>

enum Song {
    MUSIC_NULL,
    MUSIC_DAY,
    MUSIC_NIGHT,
    MUSIC_TENSE,
    MUSIC_SAD,
    MUSIC_SHINNANIGANS
};

enum SFX {
    SFX_WALK,
    SFX_MONEY,
    SFX_ERROR,
    SFX_BOUNCE,
    SFX_SWIPE,
    SFX_CLICK,
    SFX_DIALOG_MALE,
    SFX_DIALOG_FEMALE
};

class SoundManager {
    protected:
        SoundManager();
        ~SoundManager();
        static SoundManager* manager;

        std::map<Song, std::string> songs;
        std::map<SFX, Sound> sounds;
        Music music;
        Song currentMusic;
        float timePlayed;
        float volume;
        float volumeSound;
        float volumeMusic;
    public:
        SoundManager(SoundManager &other) = delete;
        void operator=(const SoundManager &) = delete;
        static SoundManager *GetInstance();

        void update();
        void playMusic(Song song);
        void pauseMusic();
        void resumeMusic();
        void playSound(SFX sound);
        bool isPlaying(SFX sound);
        void stopSound(SFX sound);
        void stopSounds();
        float getVolume();
        float getVolumeSound();
        float getVolumeMusic();
        void setVolume(float v);
        void setVolumeSound(float v);
        void setVolumeMusic(float v);
};