#include "LevelConfig.h"

LevelConfig::LevelConfig() {
}

LevelConfig::~LevelConfig() {
}

void LevelConfig::addPlayfieldCard(const CardConfig& card) {
    _playfieldCards.push_back(card);
}

void LevelConfig::addStackCard(const CardConfig& card) {
    _stackCards.push_back(card);
}

const std::vector<CardConfig>& LevelConfig::getPlayfieldCards() const {
    return _playfieldCards;
}

const std::vector<CardConfig>& LevelConfig::getStackCards() const {
    return _stackCards;
}