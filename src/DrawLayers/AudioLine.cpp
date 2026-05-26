#include "../../include/DrawLayers/AudioLine.hpp"
#include "../Utils.hpp"

using namespace good_grid;

class AudioLine::Impl final {
public:
    utils::PriorityCallbackList<AudioLineCallback> m_colorsForTime;
};

AudioLine::AudioLine() : m_impl(std::make_unique<Impl>()) {}

AudioLine::~AudioLine() {}

AudioLine* AudioLine::create() {
    auto ret = new AudioLine();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool AudioLine::init() {
    if (!CCNode::init()) return false;
    setID("audio-line"_spr);
    return true;
}

void AudioLine::draw(float minX, float maxX, float minY, float maxY) {
    GradientColor color = {2, 255, 2, 255};
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    m_impl->m_colorsForTime.rebuildIfNeeded();

    const auto& startSpeed = editorLayer->m_levelSettings->m_startSpeed;
    const auto& isPlatformer = editorLayer->m_isPlatformer;
    const auto& playbackActive = editorLayer->m_playbackActive;
    auto& rotateChannel = editorLayer->m_gameState.m_rotateChannel;
    auto& playbackX = getDrawGridLayer()->m_playbackX;
    auto& playbackY = getDrawGridLayer()->m_playbackY;
    auto& playbackTime = getDrawGridLayer()->m_playbackTime;

    auto speedObjects = getDrawGridLayer()->m_speedObjects;

    float width = 5.0f;
    if (playbackActive) {
        cocos2d::CCPoint pos = LevelTools::posForTimeInternal(
            playbackTime,
            speedObjects,
            static_cast<int>(startSpeed),
            isPlatformer,
            true,
            true,
            rotateChannel,
            false
        );
        playbackX = 0;
        playbackY = 0;

        if (LevelTools::getLastGameplayRotated()) {
            playbackY = pos.y;
        } else {
            playbackX = pos.x;
        }
    } else {
        color = {2, 255, 2, 100};
        width = 3.0f;
    }

    if (editorLayer->m_playbackMode == PlaybackMode::Playing) {
        playbackX = 0;
        playbackY = 0;
        playbackTime = 0;
    }

    for (auto& fn : m_impl->m_colorsForTime.flat) {
        fn(color, playbackActive, playbackTime, {playbackX, playbackY}, width);
    }

    if (playbackX != 0) {
        drawLine({playbackX, minY}, {playbackX, maxY}, color, width, BlendMode::ADDITIVE);
    }
    if (playbackY != 0) {
        drawLine({minX, playbackY}, {maxX, playbackY}, color, width, BlendMode::ADDITIVE);
    }
}

void AudioLine::setPropertiesForTime(AudioLineCallback colorForTime, int priority) {
    m_impl->m_colorsForTime.add(std::move(colorForTime), priority);
}