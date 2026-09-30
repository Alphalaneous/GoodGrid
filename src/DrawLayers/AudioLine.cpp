#include "../../include/DrawLayers/AudioLine.hpp"
#include "../Utils.hpp"

namespace alpha::grid {

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

void AudioLine::draw(const cocos2d::CCRect& visibleRect) {
    Color color = {2, 255, 2, 255};
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    m_impl->m_colorsForTime.rebuildIfNeeded();

    auto startSpeed = editorLayer->m_levelSettings->m_startSpeed;
    auto isPlatformer = editorLayer->m_isPlatformer;
    auto playbackActive = editorLayer->m_playbackActive;
    auto& rotateChannel = editorLayer->m_gameState.m_rotateChannel;
    auto& playbackX = getDrawGridLayer()->m_playbackX;
    auto& playbackY = getDrawGridLayer()->m_playbackY;
    auto& playbackTime = getDrawGridLayer()->m_playbackTime;

    auto speedObjects = getDrawGridLayer()->m_speedObjects;

    float width = 5.f;

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
        playbackX = 0.f;
        playbackY = 0.f;

        if (LevelTools::getLastGameplayRotated()) {
            playbackY = pos.y;
        } 
        else {
            playbackX = pos.x;
        }
    } 
    else {
        color = {2, 255, 2, 100};
        width = 3.f;
    }

    if (editorLayer->m_playbackMode == PlaybackMode::Playing) {
        playbackX = 0.f;
        playbackY = 0.f;
        playbackTime = 0.f;
    }

    for (auto& fn : m_impl->m_colorsForTime.flat) {
        fn(color, playbackActive, playbackTime, {playbackX, playbackY}, width);
    }

    if (playbackX != 0.f) {
        drawLine({playbackX, visibleRect.getMinY()}, {playbackX, visibleRect.getMaxY()}, color, width);
    }

    if (playbackY != 0.f) {
        drawLine({visibleRect.getMinX(), playbackY}, {visibleRect.getMaxX(), playbackY}, color, width);
    }
}

void AudioLine::setPropertiesForTime(AudioLineCallback colorForTime, int priority) {
    m_impl->m_colorsForTime.add(std::move(colorForTime), priority);
}

}