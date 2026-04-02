#include "PlayFieldController.h"

PlayFieldController::PlayFieldController(GameModel* gameModel, PlayFieldView* playFieldView) {
    _gameModel = gameModel;
    _playFieldView = playFieldView;
}

PlayFieldController::~PlayFieldController() {
    // 释放卡牌控制器
    for (auto controller : _cardControllers) {
        delete controller;
    }
    _cardControllers.clear();
}

void PlayFieldController::init() {
    if (_gameModel && _playFieldView) {
        // 从游戏模型获取主牌区卡牌
        const auto& playfieldCards = _gameModel->getPlayfieldCards();
        
        // 为每张卡牌创建控制器和视图
        for (auto cardModel : playfieldCards) {
            // 创建卡牌视图
            CardView* cardView = CardView::create(cardModel);
            if (cardView) {
                // 创建卡牌控制器
                CardController* cardController = new CardController(cardModel, cardView);
                _cardControllers.push_back(cardController);
                
                // 添加卡牌视图到主牌区视图
                _playFieldView->addCardView(cardView);
                
                // 初始翻开所有卡牌
                cardController->flipCard(true);
            }
        }
        
        // 设置卡牌点击回调
        _playFieldView->setOnCardClickCallback([this](int cardId) {
            if (_onCardClickCallback) {
                _onCardClickCallback(cardId);
            }
        });
    }
}

bool PlayFieldController::handleCardClick(int cardId) {
    if (!_gameModel) {
        return false;
    }
    
    // 获取卡牌控制器
    CardController* cardController = getCardController(cardId);
    if (!cardController) {
        return false;
    }
    
    // 获取卡牌模型
    CardModel* cardModel = cardController->getCardModel();
    if (!cardModel) {
        return false;
    }
    
    // 检查卡牌是否可以匹配
    if (_gameModel->canMatchCard(cardModel)) {
        // 匹配卡牌
        if (_gameModel->matchCard(cardModel)) {
            // 播放匹配动画
            playMatchAnimation(cardId);
            return true;
        }
    }
    
    return false;
}

void PlayFieldController::setOnCardClickCallback(const std::function<void(int)>& callback) {
    _onCardClickCallback = callback;
}

CardController* PlayFieldController::getCardController(int cardId) const {
    for (auto controller : _cardControllers) {
        if (controller->getCardModel()->getId() == cardId) {
            return controller;
        }
    }
    return nullptr;
}

void PlayFieldController::playMatchAnimation(int cardId) {
    if (_playFieldView) {
        _playFieldView->playMatchAnimation(cardId);
    }
}