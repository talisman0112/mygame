#include "StackController.h"
#include "cocos2d.h"

USING_NS_CC;

StackController::StackController(GameModel* gameModel, StackView* stackView) {
    _gameModel = gameModel;
    _stackView = stackView;
    _trayCardController = nullptr;
}

StackController::~StackController() {
    if (_trayCardController) {
        delete _trayCardController;
        _trayCardController = nullptr;
    }
}

void StackController::init() {
    if (_gameModel && _stackView) {
        // 设置备用牌堆点击回调
        _stackView->setOnStackClickCallback([this]() {
            handleStackClick();
        });
        _stackView->setOnHandCardClickCallback([this](int cardId) {
            handleHandCardClick(cardId);
        });
        
        // 初始化底牌
        CardModel* trayCard = _gameModel->getTrayCard();
        if (trayCard) {
            updateTrayCard(trayCard);
        }

        refreshHandCards();
    }
}

bool StackController::handleStackClick() {
    if (_onStackClickCallback) {
        _onStackClickCallback();
        return true;
    }
    return false;
}

void StackController::setOnStackClickCallback(const std::function<void()>& callback) {
    _onStackClickCallback = callback;
}

bool StackController::handleHandCardClick(int cardId) {
    if (_onHandCardClickCallback) {
        _onHandCardClickCallback(cardId);
        return true;
    }
    return false;
}

void StackController::setOnHandCardClickCallback(const std::function<void(int)>& callback) {
    _onHandCardClickCallback = callback;
}

void StackController::animateHandSwapToTray(int cardId) {
    if (!_stackView) {
        return;
    }

    CardView* handView = _stackView->getHandCard(cardId);
    if (!handView) {
        return;
    }

    const Vec2 trayPos = Vec2(840, 290);
    auto move = MoveTo::create(0.18f, trayPos);
    auto done = CallFunc::create([this]() {
        refreshHandCards();
    });
    handView->runAction(Sequence::create(move, done, nullptr));
}

void StackController::updateTrayCard(CardModel* cardModel) {
    if (!_stackView || !cardModel) {
        return;
    }
    
    // 创建卡牌视图
    CardView* cardView = CardView::create(cardModel);
    if (cardView) {
        // 创建卡牌控制器
        if (_trayCardController) {
            delete _trayCardController;
        }
        _trayCardController = new CardController(cardModel, cardView);
        
        // 设置底牌视图
        _stackView->setTrayCard(cardView);
        
        // 翻开底牌
        _trayCardController->flipCard(true);
    }
}

void StackController::setTrayCardFromView(CardModel* cardModel, CardView* cardView) {
    if (!cardModel || !cardView) {
        return;
    }

    // 顶部底牌不响应点击（按需求：点击自己手牌不应触发任何动作）
    cardView->setOnClickCallback(nullptr);

    if (_trayCardController) {
        delete _trayCardController;
        _trayCardController = nullptr;
    }
    _trayCardController = new CardController(cardModel, cardView);
    _trayCardController->flipCard(true);
}

void StackController::refreshHandCards() {
    if (!_gameModel || !_stackView) {
        return;
    }

    _stackView->setStackVisible(!_gameModel->getStackCards().empty());

    const auto& handCards = _gameModel->getHandCards();
    std::vector<CardView*> handViews;
    handViews.reserve(handCards.size());

    for (auto* model : handCards) {
        if (!model) {
            continue;
        }

        CardView* handView = CardView::create(model);
        if (!handView) {
            continue;
        }

        CardController tmpController(model, handView);
        tmpController.flipCard(true);

        handView->setOnClickCallback([this](CardView* view) {
            if (view && view->getCardModel()) {
                this->handleHandCardClick(view->getCardModel()->getId());
            }
        });

        handViews.push_back(handView);
    }

    _stackView->setHandCards(handViews);
}

CardController* StackController::getTrayCardController() const {
    return _trayCardController;
}