#include "GameView.h"

USING_NS_CC;

GameView::GameView()
    : _playFieldView(nullptr),
      _stackView(nullptr),
      _undoButton(nullptr),
      _menu(nullptr),
      _winOverlay(nullptr) {}

GameView* GameView::create() {
    GameView* gameView = new (std::nothrow) GameView();
    if (gameView && gameView->init()) {
        gameView->autorelease();
        return gameView;
    }
    delete gameView;
    return nullptr;
}

bool GameView::init() {
    if (!Layer::init()) {
        return false;
    }

    // 让GameView坐标系与可视区域一致
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();
    setContentSize(visibleSize);
    setPosition(origin);

    // 创建主牌区视图
    _playFieldView = PlayFieldView::create();
    if (_playFieldView) {
        addChild(_playFieldView);
    }

    // 创建牌堆视图
    _stackView = StackView::create();
    if (_stackView) {
        addChild(_stackView);
    }

    // 创建回退按钮
    auto label = Label::createWithSystemFont("回退", "Microsoft YaHei", 48);
    if (label) {
        _undoButton = MenuItemLabel::create(label, [](Ref*) {});
    }
    if (_undoButton) {
        // 放在底部牌堆区域右侧（与截图一致）
        _undoButton->setPosition(Vec2(1000, 290));
        _menu = Menu::create(_undoButton, nullptr);
        _menu->setPosition(Vec2::ZERO);
        addChild(_menu, 2000);
    }

    return true;
}

PlayFieldView* GameView::getPlayFieldView() const {
    return _playFieldView;
}

StackView* GameView::getStackView() const {
    return _stackView;
}

void GameView::setOnUndoCallback(const std::function<void()>& callback) {
    if (_undoButton) {
        _undoButton->setCallback([callback](Ref*) {
            if (callback) {
                callback();
            }
        });
    }
}

void GameView::showWinOverlay() {
    if (_winOverlay) {
        return;
    }

    const auto visibleSize = getContentSize();
    _winOverlay = LayerColor::create(Color4B(0, 0, 0, 160), visibleSize.width, visibleSize.height);
    addChild(_winOverlay, 5000);

    auto title = Label::createWithSystemFont("胜利", "Microsoft YaHei", 72);
    if (title) {
        title->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.62f));
        _winOverlay->addChild(title);
    }

    auto exitLabel = Label::createWithSystemFont("退出程序", "Microsoft YaHei", 42);
    if (exitLabel) {
        auto exitItem = MenuItemLabel::create(exitLabel, [](Ref*) {
            Director::getInstance()->end();
        });
        if (exitItem) {
            auto menu = Menu::create(exitItem, nullptr);
            menu->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.48f));
            _winOverlay->addChild(menu);
        }
    }
}