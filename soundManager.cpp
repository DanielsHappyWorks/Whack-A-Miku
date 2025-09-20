#include "soundManager.h"

SoundManager* SoundManager::manager = nullptr;

SoundManager *SoundManager::GetInstance()
{
    if(manager==nullptr){
        manager = new SoundManager();
    }
    return manager;
}

SoundManager::SoundManager() {
    InitAudioDevice();
    
    songs = {
        {MUSIC_DAY, "./assets/sounds/music/Town_-_Sidewalk_Stroll.ogg"},
        {MUSIC_NIGHT, "./assets/sounds/music/Theme_-_Starry_Night.ogg"},
        {MUSIC_TENSE, "./assets/sounds/music/Drama_-_Impatient_Waiting.ogg"},
        {MUSIC_SAD, "./assets/sounds/music/Drama_-_Bittersweet_Reunion.ogg"},
        {MUSIC_SHINNANIGANS, "./assets/sounds/music/Town_-_Boutique_Hopper.ogg"}
    };
    sounds = {
        {SFX_WALK, LoadSound("./assets/sounds/freesound/366111__wakuwakuwakuwaku__indoor-footsteps.wav")},
        {SFX_SWIPE, LoadSound("./assets/sounds/freesound/515625__mrickey13__throwswipe.wav")},
        {SFX_MONEY, LoadSound("./assets/sounds/freesound/393909__pogmog__money-collect-4.wav")},
        {SFX_BOUNCE, LoadSound("./assets/sounds/freesound/615576__adhdreaming__bit-bounce.wav")},
        {SFX_ERROR, LoadSound("./assets/sounds/freesound/542040__rob_marion__gasp_ui_reject.wav")},
        {SFX_CLICK, LoadSound("./assets/sounds/freesound/475188__sheyvan__button-clicking-1.wav")},
        {SFX_DIALOG_MALE, LoadSound("./assets/sounds/dialog_male.wav")},
        {SFX_DIALOG_FEMALE, LoadSound("./assets/sounds/dialog_female.wav")}
    };

    //Music music;
    timePlayed = 0.0f;
    currentMusic = MUSIC_NULL;

    volume = 0.5f;
    volumeSound = 1.0f;
    volumeMusic = 1.0f;

    playMusic(MUSIC_DAY);
}

SoundManager::~SoundManager() {
    UnloadMusicStream(music);

    //REF loop over map https://stackoverflow.com/questions/26281979/c-loop-through-map
    std::map<SFX, Sound>::iterator it;
    for (it = sounds.begin(); it != sounds.end(); it++)
    {
        UnloadSound(it->second);
    }

    CloseAudioDevice();
}

void SoundManager::update() {
    SetMasterVolume(volume);
    SetMusicVolume(music, volumeMusic);

    UpdateMusicStream(music);

    timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);
    if (timePlayed > 1.0f) timePlayed = 1.0f;   // Make sure time played is no longer than music
}

void SoundManager::playMusic(Song song) {
    //already playing lets let it keep looping!
    if (song == currentMusic) {
        return;
    }

    //stop current and unload
    if (currentMusic != MUSIC_NULL) {
        UnloadMusicStream(music);
    }

    if (auto search = songs.find(song); search != songs.end()) {
        music = LoadMusicStream(search->second.c_str());
        SetMusicVolume(music, volumeMusic);
        PlayMusicStream(music);
        currentMusic = song;
    }
}

void SoundManager::pauseMusic() {
    if (currentMusic != MUSIC_NULL) {
        PauseMusicStream(music);
    }
}

void SoundManager::resumeMusic() {
    if (currentMusic != MUSIC_NULL) {
        ResumeMusicStream(music);
    }
}

void SoundManager::playSound(SFX sound) {
    if (auto search = sounds.find(sound); search != sounds.end()) {
        PlaySound(search->second);
    }
}

bool SoundManager::isPlaying(SFX sound) {
    if (auto search = sounds.find(sound); search != sounds.end()) {
        return IsSoundPlaying(search->second);
    }
    return false;
}

void SoundManager::stopSound(SFX sound) {
    if (auto search = sounds.find(sound); search != sounds.end()) {
        if (IsSoundPlaying(search->second)) {
            StopSound(search->second);
        }
    }
}

void SoundManager::stopSounds() {
    for(auto sound : sounds) {
        if (IsSoundPlaying(sound.second)) {
            StopSound(sound.second);
        }
    }
}

float SoundManager::getVolume() {
    return volume;
}

float SoundManager::getVolumeSound() {
    return volumeSound;
}

float SoundManager::getVolumeMusic() {
    return volumeMusic;
}

void SoundManager::setVolume(float v) {
    volume = v;
    SetMasterVolume(volume);
}

void SoundManager::setVolumeSound(float v) {
    volumeSound = v;

    for (auto sound : sounds) {
        SetSoundVolume(sound.second, volumeSound);
    }
}

void SoundManager::setVolumeMusic(float v) {
    volumeMusic = v;
    SetMusicVolume(music, volumeMusic);
}
