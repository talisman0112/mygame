#include "CardModel.h"

CardModel::CardModel(int id, int face, int suit, const cocos2d::Vec2& position) {
    _id = id;
    _face = face;
    _suit = suit;
    _isFlipped = false;
    _isMatched = false;
    _position = position;
}

CardModel::~CardModel() {
}

int CardModel::getId() const {
    return _id;
}

int CardModel::getFace() const {
    return _face;
}

int CardModel::getSuit() const {
    return _suit;
}

bool CardModel::isFlipped() const {
    return _isFlipped;
}

void CardModel::setFlipped(bool flipped) {
    _isFlipped = flipped;
}

bool CardModel::isMatched() const {
    return _isMatched;
}

void CardModel::setMatched(bool matched) {
    _isMatched = matched;
}

const cocos2d::Vec2& CardModel::getPosition() const {
    return _position;
}

void CardModel::setPosition(const cocos2d::Vec2& position) {
    _position = position;
}