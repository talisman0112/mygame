#include "GameModel.h"

GameModel::GameModel() {
    _trayCard = nullptr;
    _currentFlippedCard = nullptr;
}

GameModel::~GameModel() {
    // 释放所有卡牌
    for (auto card : _playfieldCards) {
        delete card;
    }
    _playfieldCards.clear();
    
    for (auto card : _stackCards) {
        delete card;
    }
    _stackCards.clear();
    for (auto card : _handCards) {
        delete card;
    }
    _handCards.clear();

    // _trayCard 与 _currentFlippedCard 仅为引用指针，不在此处释放
    _trayCard = nullptr;
    _currentFlippedCard = nullptr;
}

void GameModel::addPlayfieldCard(CardModel* card) {
    _playfieldCards.push_back(card);
}

void GameModel::addStackCard(CardModel* card) {
    _stackCards.push_back(card);
}

void GameModel::removeHandCard(CardModel* card) {
    for (auto it = _handCards.begin(); it != _handCards.end(); ++it) {
        if (*it == card) {
            _handCards.erase(it);
            return;
        }
    }
}

bool GameModel::hasHandCard(CardModel* card) const {
    for (auto* handCard : _handCards) {
        if (handCard == card) {
            return true;
        }
    }
    return false;
}

void GameModel::setTrayCard(CardModel* card) {
    _trayCard = card;
}

CardModel* GameModel::getTrayCard() const {
    return _trayCard;
}

const std::vector<CardModel*>& GameModel::getPlayfieldCards() const {
    return _playfieldCards;
}

const std::vector<CardModel*>& GameModel::getStackCards() const {
    return _stackCards;
}

const std::vector<CardModel*>& GameModel::getHandCards() const {
    return _handCards;
}

CardModel* GameModel::drawStackCard() {
    if (_stackCards.empty()) {
        return nullptr;
    }
    
    CardModel* card = _stackCards.back();
    _stackCards.pop_back();
    return card;
}

CardModel* GameModel::drawCardToHand() {
    CardModel* card = drawStackCard();
    if (!card) {
        return nullptr;
    }
    _handCards.push_back(card);
    return card;
}

bool GameModel::swapTrayWithStackCard(CardModel* stackCard) {
    if (!_trayCard || !stackCard) {
        return false;
    }

    for (size_t i = 0; i < _handCards.size(); ++i) {
        if (_handCards[i] == stackCard) {
            CardModel* oldTray = _trayCard;
            _trayCard = stackCard;
            _trayCard->setFlipped(true);
            _handCards[i] = oldTray;
            return true;
        }
    }

    return false;
}

bool GameModel::replaceTrayWithMatchedCard(CardModel* card) {
    if (!canMatchCard(card)) {
        return false;
    }

    if (_trayCard) {
        _handCards.push_back(_trayCard);
    }

    card->setMatched(true);
    card->setFlipped(true);
    _trayCard = card;
    return true;
}

bool GameModel::canMatchCard(CardModel* card) const {
    if (!_trayCard || !card || card->isMatched()) {
        return false;
    }
    
    int trayFace = _trayCard->getFace();
    int cardFace = card->getFace();
    
    // 检查是否相差1
    return (abs(trayFace - cardFace) == 1) || 
           // 特殊情况：A和K可以互相匹配
           (trayFace == CFT_ACE && cardFace == CFT_KING) ||
           (trayFace == CFT_KING && cardFace == CFT_ACE);
}

bool GameModel::matchCard(CardModel* card) {
    return replaceTrayWithMatchedCard(card);
}

void GameModel::setCurrentFlippedCard(CardModel* card) {
    _currentFlippedCard = card;
}

CardModel* GameModel::getCurrentFlippedCard() const {
    return _currentFlippedCard;
}

CardModel* GameModel::findCardById(int cardId) const {
    for (auto card : _playfieldCards) {
        if (card && card->getId() == cardId) {
            return card;
        }
    }
    for (auto card : _stackCards) {
        if (card && card->getId() == cardId) {
            return card;
        }
    }
    for (auto card : _handCards) {
        if (card && card->getId() == cardId) {
            return card;
        }
    }
    if (_trayCard && _trayCard->getId() == cardId) {
        return _trayCard;
    }
    return nullptr;
}