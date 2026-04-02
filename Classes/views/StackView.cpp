#include "StackView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

StackView::StackView()
    : _trayCardView(nullptr),
      _stackSprite(nullptr) {}

StackView* StackView::create() {
    StackView* stackView = new (std::nothrow) StackView();
    if (stackView && stackView->init()) {
        stackView->autorelease();
        return stackView;
    }
    delete stackView;
    return nullptr;
}

bool StackView::init() {
    if (!Layer::init()) {
        return false;
    }

    setContentSize(Size(1080, 580));
    setPosition(Vec2(0, 0));

    auto background = LayerColor::create(Color4B(150, 150, 150, 255), 1080, 580);
    addChild(background);

    CardResConfig resConfig;
    _stackSprite = Sprite::create(resConfig.getCardBackPath());
    if (_stackSprite) {
        _stackSprite->setPosition(Vec2(200, 290));
        addChild(_stackSprite);
    }

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(StackView::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(StackView::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void StackView::setTrayCard(CardView* cardView) {
    if (_trayCardView && _trayCardView->getParent()) {
        _trayCardView->removeFromParent();
    }

    _trayCardView = cardView;
    if (_trayCardView) {
        if (_trayCardView->getParent()) {
            _trayCardView->removeFromParent();
        }
        _trayCardView->setPosition(Vec2(840, 290));
        addChild(_trayCardView, 20);
    }
}

CardView* StackView::getTrayCard() const {
    return _trayCardView;
}

void StackView::setHandCards(const std::vector<CardView*>& cardViews) {
    for (auto* view : _handCardViews) {
        if (view && view != _trayCardView && view->getParent()) {
            view->removeFromParent();
        }
    }
    _handCardViews.clear();

    _handCardViews = cardViews;
    for (auto* view : _handCardViews) {
        if (!view) {
            continue;
        }
        if (view->getParent()) {
            view->removeFromParent();
        }
        addChild(view, 15);
    }

    layoutHandCards();
}

CardView* StackView::getHandCard(int cardId) const {
    for (auto* view : _handCardViews) {
        if (view && view->getCardModel() && view->getCardModel()->getId() == cardId) {
            return view;
        }
    }
    return nullptr;
}

void StackView::setOnHandCardClickCallback(const std::function<void(int)>& callback) {
    _onHandCardClickCallback = callback;
}

void StackView::setOnStackClickCallback(const std::function<void()>& callback) {
    _onStackClickCallback = callback;
}

void StackView::playDrawStackAnimation() {
    if (_stackSprite) {
        auto scaleDown = ScaleTo::create(0.1f, 0.9f);
        auto scaleUp = ScaleTo::create(0.1f, 1.0f);
        auto sequence = Sequence::create(scaleDown, scaleUp, nullptr);
        _stackSprite->runAction(sequence);
    }
}

void StackView::setStackVisible(bool visible) {
    if (_stackSprite) {
        _stackSprite->setVisible(visible);
    }
}

void StackView::layoutHandCards() {
    const float baseX = 360.0f;
    const float baseY = 290.0f;
    const float maxRightX = 690.0f;
    const int count = static_cast<int>(_handCardViews.size());
    if (count <= 0) {
        return;
    }

    float stepX = 90.0f;
    if (count > 1) {
        stepX = std::min(90.0f, (maxRightX - baseX) / (count - 1));
        stepX = std::max(38.0f, stepX);
    }

    for (int i = 0; i < count; ++i) {
        auto* view = _handCardViews[i];
        if (!view) {
            continue;
        }
        view->setPosition(Vec2(baseX + stepX * i, baseY));
        view->setLocalZOrder(10 + i);
        view->setVisible(true);
    }
}

bool StackView::onTouchBegan(Touch* touch, Event* event) {
    Vec2 touchPos = convertTouchToNodeSpace(touch);
    if (_stackSprite && _stackSprite->getBoundingBox().containsPoint(touchPos)) {
        return true;
    }
    return false;
}

void StackView::onTouchEnded(Touch* touch, Event* event) {
    Vec2 touchPos = convertTouchToNodeSpace(touch);
    if (_stackSprite && _stackSprite->getBoundingBox().containsPoint(touchPos) && _onStackClickCallback) {
        _onStackClickCallback();
    }
}