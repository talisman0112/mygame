#include "GameController.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../services/GameModelFromLevelGenerator.h"

bool GameController::checkWinCondition() const {
    if (!_gameModel) {
        return false;
    }

    const auto& playfieldCards = _gameModel->getPlayfieldCards();
    for (auto* card : playfieldCards) {
        if (card && !card->isMatched()) {
            return false;
        }
    }
    return !playfieldCards.empty();
}

void GameController::tryShowWinOverlay() {
    if (_gameView && checkWinCondition()) {
        _gameView->showWinOverlay();
    }
}

GameController::GameController() {
    _gameModel = nullptr;
    _gameView = nullptr;
    _playFieldController = nullptr;
    _stackController = nullptr;
    _undoManager = nullptr;
    _isAnimating = false;
}

GameController::~GameController() {
    if (_gameModel) {
        delete _gameModel;
        _gameModel = nullptr;
    }
    
    if (_playFieldController) {
        delete _playFieldController;
        _playFieldController = nullptr;
    }
    
    if (_stackController) {
        delete _stackController;
        _stackController = nullptr;
    }
    
    if (_undoManager) {
        delete _undoManager;
        _undoManager = nullptr;
    }
}

void GameController::initGame(int levelId) {
    // 加载关卡配置
    LevelConfig levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    
    // 生成游戏模型
    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    
    // 创建游戏视图
    _gameView = GameView::create();
    
    // 创建回退管理器
    _undoManager = new UndoManager(_gameModel);
    _undoManager->init();
    
    // 创建主牌区控制器
    _playFieldController = new PlayFieldController(_gameModel, _gameView->getPlayFieldView());
    _playFieldController->init();
    _playFieldController->setOnCardClickCallback([this](int cardId) {
        handleCardClick(cardId);
    });
    
    // 创建牌堆控制器
    _stackController = new StackController(_gameModel, _gameView->getStackView());
    _stackController->init();
    _stackController->setOnStackClickCallback([this]() {
        handleStackClick();
    });
    _stackController->setOnHandCardClickCallback([this](int cardId) {
        handleHandCardClick(cardId);
    });
    
    // 设置回退按钮点击回调
    _gameView->setOnUndoCallback([this]() {
        handleUndo();
    });
}

bool GameController::handleCardClick(int cardId) {
    if (_isAnimating || !_gameModel || !_playFieldController || !_gameView || !_stackController) {
        return false;
    }
    
    CardController* cardController = _playFieldController->getCardController(cardId);
    if (!cardController) {
        return false;
    }
    CardModel* cardModel = cardController->getCardModel();
    if (!cardModel) {
        return false;
    }

    if (!_gameModel->canMatchCard(cardModel)) {
        return false;
    }

    auto playFieldView = _gameView->getPlayFieldView();
    auto stackView = _gameView->getStackView();
    if (!playFieldView || !stackView) {
        return false;
    }

    CardView* cardView = playFieldView->getCardView(cardId);
    if (!cardView) {
        return false;
    }

    CardModel* prevTray = _gameModel->getTrayCard();
    const cocos2d::Vec2 fromWorld = playFieldView->convertToWorldSpace(cardView->getPosition());
    _undoManager->recordMatchCard(prevTray, cardModel, fromWorld);

    // 将cardView从桌面平移到底牌位置并替换
    _isAnimating = true;
    cardView->retain();
    playFieldView->removeCardView(cardView);

    const cocos2d::Vec2 trayWorld = stackView->convertToWorldSpace(cocos2d::Vec2(800, 290));
    const cocos2d::Vec2 startInRoot = _gameView->convertToNodeSpace(fromWorld);
    const cocos2d::Vec2 endInRoot = _gameView->convertToNodeSpace(trayWorld);

    _gameView->addChild(cardView, 1000);
    cardView->setPosition(startInRoot);

    auto move = cocos2d::MoveTo::create(0.18f, endInRoot);
    auto done = cocos2d::CallFunc::create([this, cardModel, cardView]() {
        // 更新模型：桌面牌成为新牌顶，旧牌顶回到手牌区
        _gameModel->replaceTrayWithMatchedCard(cardModel);

        // 将移动的牌接管为底牌视图
        cardView->removeFromParent();
        _gameView->getStackView()->setTrayCard(cardView);
        _stackController->setTrayCardFromView(cardModel, cardView);
        _stackController->refreshHandCards();

        _isAnimating = false;
        tryShowWinOverlay();
        cardView->release();
    });
    cardView->runAction(cocos2d::Sequence::create(move, done, nullptr));
    return true;
}

bool GameController::handleStackClick() {
    if (!_gameModel || !_stackController) {
        return false;
    }
    
    if (!_gameModel->getStackCards().empty()) {
        CardModel* nextCard = _gameModel->getStackCards().back();
        _undoManager->recordDrawStack(_gameModel->getTrayCard(), nextCard);
    }

    CardModel* stackCard = _gameModel->drawCardToHand();
    if (!stackCard) {
        return false;
    }

    _gameView->getStackView()->playDrawStackAnimation();
    _stackController->refreshHandCards();
    return true;
}

bool GameController::handleHandCardClick(int cardId) {
    if (_isAnimating || !_gameModel || !_stackController || !_gameView || !_undoManager) {
        return false;
    }

    auto stackView = _gameView->getStackView();
    if (!stackView) {
        return false;
    }

    CardModel* selectedCard = _gameModel->findCardById(cardId);
    CardModel* trayCard = _gameModel->getTrayCard();
    if (!selectedCard || !trayCard || selectedCard == trayCard) {
        return false;
    }

    CardView* handView = stackView->getHandCard(cardId);
    if (!handView) {
        return false;
    }

    const cocos2d::Vec2 fromWorld = stackView->convertToWorldSpace(handView->getPosition());
    _undoManager->recordSwapHand(trayCard, selectedCard, fromWorld);

    if (!_gameModel->swapTrayWithStackCard(selectedCard)) {
        return false;
    }

    _isAnimating = true;
    handView->retain();
    if (handView->getParent()) {
        handView->removeFromParent();
    }

    const cocos2d::Vec2 trayWorld = stackView->convertToWorldSpace(cocos2d::Vec2(840, 290));
    const cocos2d::Vec2 startInRoot = _gameView->convertToNodeSpace(fromWorld);
    const cocos2d::Vec2 endInRoot = _gameView->convertToNodeSpace(trayWorld);

    _gameView->addChild(handView, 1000);
    handView->setPosition(startInRoot);

    auto move = cocos2d::MoveTo::create(0.18f, endInRoot);
    auto done = cocos2d::CallFunc::create([this, selectedCard, handView]() {
        handView->removeFromParent();
        _gameView->getStackView()->setTrayCard(handView);
        _stackController->setTrayCardFromView(selectedCard, handView);
        _stackController->refreshHandCards();
        _isAnimating = false;
        handView->release();
    });
    handView->runAction(cocos2d::Sequence::create(move, done, nullptr));
    return true;
}

bool GameController::handleUndo() {
    if (_isAnimating || !_undoManager || !_stackController || !_gameModel || !_gameView) {
        return false;
    }

    OperationRecord record;
    if (!_undoManager->popLastRecord(record)) {
        return false;
    }

    auto playFieldView = _gameView->getPlayFieldView();
    auto stackView = _gameView->getStackView();
    if (!playFieldView || !stackView) {
        return false;
    }

    auto refreshBottomArea = [this]() {
        if (_gameModel->getTrayCard()) {
            _stackController->updateTrayCard(_gameModel->getTrayCard());
        }
        _stackController->refreshHandCards();
    };

    switch (record.type) {
        case OP_MATCH_CARD: {
            CardView* trayView = stackView->getTrayCard();
            if (!trayView || !record.movedCard) {
                return false;
            }

            _isAnimating = true;
            trayView->retain();
            // 先从牌顶区域解绑，否则后续 refreshBottomArea 会把它误当成旧牌顶再次移除
            stackView->setTrayCard(nullptr);

            const cocos2d::Vec2 startInRoot = _gameView->convertToNodeSpace(
                stackView->convertToWorldSpace(cocos2d::Vec2(840, 290)));
            const cocos2d::Vec2 endInRoot = _gameView->convertToNodeSpace(record.fromWorldPos);
            _gameView->addChild(trayView, 1000);
            trayView->setPosition(startInRoot);

            auto move = cocos2d::MoveTo::create(0.22f, endInRoot);
            auto ease = cocos2d::EaseSineInOut::create(move);
            auto done = cocos2d::CallFunc::create([=]() {
                if (record.prevTrayCard) {
                    _gameModel->removeHandCard(record.prevTrayCard);
                    _gameModel->setTrayCard(record.prevTrayCard);
                }
                record.movedCard->setMatched(record.movedWasMatched);

                trayView->removeFromParent();
                trayView->setPosition(playFieldView->convertToNodeSpace(record.fromWorldPos));
                playFieldView->addCardView(trayView);

                refreshBottomArea();
                _isAnimating = false;
                trayView->release();
            });
            trayView->runAction(cocos2d::Sequence::create(ease, done, nullptr));
            break;
        }
        case OP_DRAW_STACK: {
            if (!record.movedCard) {
                return false;
            }

            CardView* handView = stackView->getHandCard(record.movedCard->getId());
            if (!handView) {
                if (_gameModel->hasHandCard(record.movedCard)) {
                    _gameModel->removeHandCard(record.movedCard);
                }
                _gameModel->addStackCard(record.movedCard);
                if (record.prevTrayCard) {
                    _gameModel->setTrayCard(record.prevTrayCard);
                }
                refreshBottomArea();
                return true;
            }

            _isAnimating = true;
            handView->retain();
            const cocos2d::Vec2 pileWorld = stackView->convertToWorldSpace(cocos2d::Vec2(200, 290));
            const cocos2d::Vec2 startInRoot = _gameView->convertToNodeSpace(
                stackView->convertToWorldSpace(handView->getPosition()));
            const cocos2d::Vec2 endInRoot = _gameView->convertToNodeSpace(pileWorld);

            handView->removeFromParent();
            _gameView->addChild(handView, 1000);
            handView->setPosition(startInRoot);

            auto move = cocos2d::MoveTo::create(0.2f, endInRoot);
            auto scale = cocos2d::ScaleTo::create(0.2f, 0.85f);
            auto done = cocos2d::CallFunc::create([=]() {
                if (_gameModel->hasHandCard(record.movedCard)) {
                    _gameModel->removeHandCard(record.movedCard);
                }
                _gameModel->addStackCard(record.movedCard);
                if (record.prevTrayCard) {
                    _gameModel->setTrayCard(record.prevTrayCard);
                }

                handView->removeFromParent();
                refreshBottomArea();
                stackView->playDrawStackAnimation();
                _isAnimating = false;
                handView->release();
            });
            handView->runAction(cocos2d::Sequence::create(
                cocos2d::Spawn::create(move, scale, nullptr),
                done,
                nullptr));
            break;
        }
        case OP_SWAP_HAND: {
            if (!record.prevTrayCard || !record.movedCard) {
                return false;
            }

            CardView* trayView = stackView->getTrayCard();
            if (!trayView) {
                _gameModel->swapTrayWithStackCard(record.prevTrayCard);
                refreshBottomArea();
                return true;
            }

            _isAnimating = true;
            trayView->retain();
            stackView->setTrayCard(nullptr);
            const cocos2d::Vec2 startInRoot = _gameView->convertToNodeSpace(
                stackView->convertToWorldSpace(cocos2d::Vec2(840, 290)));
            const cocos2d::Vec2 endInRoot = _gameView->convertToNodeSpace(record.fromWorldPos);

            _gameView->addChild(trayView, 1000);
            trayView->setPosition(startInRoot);

            auto move = cocos2d::MoveTo::create(0.2f, endInRoot);
            auto rotate = cocos2d::RotateBy::create(0.2f, -6.0f);
            auto done = cocos2d::CallFunc::create([=]() {
                _gameModel->swapTrayWithStackCard(record.prevTrayCard);
                trayView->removeFromParent();
                refreshBottomArea();
                _isAnimating = false;
                trayView->release();
            });
            trayView->runAction(cocos2d::Sequence::create(
                cocos2d::Spawn::create(move, rotate, nullptr),
                cocos2d::CallFunc::create([trayView]() { trayView->setRotation(0.0f); }),
                done,
                nullptr));
            break;
        }
    }
    return true;
}

GameView* GameController::getGameView() const {
    return _gameView;
}