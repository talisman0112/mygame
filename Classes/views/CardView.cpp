#include "CardView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

namespace {
static bool isRedSuit(int suit) {
    return suit == CST_DIAMONDS || suit == CST_HEARTS;
}

static std::string faceToken(int face) {
    switch (face) {
        case CFT_ACE:   return "A";
        case CFT_TWO:   return "2";
        case CFT_THREE: return "3";
        case CFT_FOUR:  return "4";
        case CFT_FIVE:  return "5";
        case CFT_SIX:   return "6";
        case CFT_SEVEN: return "7";
        case CFT_EIGHT: return "8";
        case CFT_NINE:  return "9";
        case CFT_TEN:   return "10";
        case CFT_JACK:  return "J";
        case CFT_QUEEN: return "Q";
        case CFT_KING:  return "K";
        default:        return "A";
    }
}

static std::string numberSpritePath(int face, int suit) {
    const std::string color = isRedSuit(suit) ? "red" : "black";
    return "res/number/big_" + color + "_" + faceToken(face) + ".png";
}
}

CardView* CardView::create(CardModel* cardModel) {
    CardView* cardView = new (std::nothrow) CardView();
    if (cardView && cardView->init(cardModel)) {
        cardView->autorelease();
        return cardView;
    }
    delete cardView;
    return nullptr;
}

bool CardView::init(CardModel* cardModel) {
    if (!Sprite::init()) {
        return false;
    }

    _cardModel = cardModel;
    _isFlipped = false;

    // 创建卡牌正面和背面
    CardResConfig resConfig;
    
    _frontSprite = Sprite::create(resConfig.getCardFrontPath());
    _backSprite = Sprite::create(resConfig.getCardBackPath());

    if (_frontSprite && _backSprite) {
        // 让CardView本体具有明确尺寸，触摸命中才可靠
        setAnchorPoint(Vec2(0.5f, 0.5f));
        setContentSize(_backSprite->getContentSize());

        // 居中放置正反面
        const auto cs = getContentSize();
        _frontSprite->setPosition(Vec2(cs.width * 0.5f, cs.height * 0.5f));
        _backSprite->setPosition(Vec2(cs.width * 0.5f, cs.height * 0.5f));

        addChild(_frontSprite);
        addChild(_backSprite);

        // 在正面叠加数字与花色（根据 CardModel）
        const int face = _cardModel ? _cardModel->getFace() : 0;
        const int suit = _cardModel ? _cardModel->getSuit() : 0;

        const std::string numberPath = numberSpritePath(face, suit);

        auto numberSprite = Sprite::create(numberPath);
        auto suitSprite = Sprite::create(resConfig.getSuitPath(suit));
        if (numberSprite) {
            numberSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
            numberSprite->setScale(0.92f);
            numberSprite->setPosition(Vec2(
                _frontSprite->getContentSize().width * 0.5f,
                _frontSprite->getContentSize().height * 0.50f));
            _frontSprite->addChild(numberSprite, 1);
        }
        if (suitSprite) {
            suitSprite->setAnchorPoint(Vec2(0.0f, 1.0f));
            suitSprite->setScale(0.62f);
            suitSprite->setPosition(Vec2(18.0f, _frontSprite->getContentSize().height - 18.0f));
            _frontSprite->addChild(suitSprite, 1);
        }
        
        // 初始状态显示背面
        _frontSprite->setVisible(false);
        _backSprite->setVisible(true);
        
        // 设置位置
        setPosition(_cardModel->getPosition());
        
        // 添加触摸事件监听
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(CardView::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        
        return true;
    }
    
    return false;
}

void CardView::flip(bool flipped) {
    if (_isFlipped == flipped) {
        return;
    }
    
    _isFlipped = flipped;
    
    // 翻转动画
    auto flipAction = FlipX::create(true);
    auto callback = CallFunc::create([this, flipped]() {
        _frontSprite->setVisible(flipped);
        _backSprite->setVisible(!flipped);
    });
    auto flipBackAction = FlipX::create(false);
    auto sequence = Sequence::create(flipAction, callback, flipBackAction, nullptr);
    runAction(sequence);
    
    // 更新模型状态
    _cardModel->setFlipped(flipped);
}

void CardView::setOnClickCallback(const std::function<void(CardView*)>& callback) {
    _onClickCallback = callback;
}

CardModel* CardView::getCardModel() const {
    return _cardModel;
}

void CardView::playMatchAnimation() {
    // 匹配动画：缩放并淡出
    auto scaleUp = ScaleTo::create(0.2f, 1.2f);
    auto fadeOut = FadeOut::create(0.3f);
    auto sequence = Sequence::create(scaleUp, fadeOut, nullptr);
    runAction(sequence);
}

bool CardView::onTouchBegan(Touch* touch, Event* event) {
    Vec2 touchPos = convertTouchToNodeSpaceAR(touch);
    const Size sz = getContentSize();
    Rect rect(-sz.width * 0.5f, -sz.height * 0.5f, sz.width, sz.height);
    if (rect.containsPoint(touchPos)) {
        return true;
    }
    return false;
}

void CardView::onTouchMoved(Touch* touch, Event* event) {
    // 可以添加拖动逻辑
}

void CardView::onTouchEnded(Touch* touch, Event* event) {
    Vec2 touchPos = convertTouchToNodeSpaceAR(touch);
    const Size sz = getContentSize();
    Rect rect(-sz.width * 0.5f, -sz.height * 0.5f, sz.width, sz.height);
    if (rect.containsPoint(touchPos) && _onClickCallback) {
        _onClickCallback(this);
    }
}