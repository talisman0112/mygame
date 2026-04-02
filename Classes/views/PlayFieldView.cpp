#include "PlayFieldView.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create() {
    PlayFieldView* playFieldView = new (std::nothrow) PlayFieldView();
    if (playFieldView && playFieldView->init()) {
        playFieldView->autorelease();
        return playFieldView;
    }
    delete playFieldView;
    return nullptr;
}

bool PlayFieldView::init() {
    if (!Layer::init()) {
        return false;
    }

    // 设置主牌区大小
    setContentSize(Size(1080, 1500));
    setPosition(Vec2(0, 580));

    // 可以添加背景
    auto background = LayerColor::create(Color4B(200, 200, 200, 255), 1080, 1500);
    addChild(background);

    return true;
}

void PlayFieldView::addCardView(CardView* cardView) {
    if (cardView) {
        _cardViews.push_back(cardView);
        addChild(cardView);
        
        // 设置卡牌点击回调
        cardView->setOnClickCallback([this](CardView* view) {
            if (_onCardClickCallback) {
                _onCardClickCallback(view->getCardModel()->getId());
            }
        });
    }
}

void PlayFieldView::removeCardView(CardView* cardView) {
    if (cardView) {
        auto it = std::find(_cardViews.begin(), _cardViews.end(), cardView);
        if (it != _cardViews.end()) {
            _cardViews.erase(it);
            removeChild(cardView);
        }
    }
}

void PlayFieldView::setOnCardClickCallback(const std::function<void(int)>& callback) {
    _onCardClickCallback = callback;
}

CardView* PlayFieldView::getCardView(int cardId) const {
    for (auto cardView : _cardViews) {
        if (cardView->getCardModel()->getId() == cardId) {
            return cardView;
        }
    }
    return nullptr;
}

void PlayFieldView::playMatchAnimation(int cardId) {
    CardView* cardView = getCardView(cardId);
    if (cardView) {
        cardView->playMatchAnimation();
    }
}