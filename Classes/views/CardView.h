#ifndef CARDVIEW_H
#define CARDVIEW_H

#include "cocos2d.h"
#include "models/CardModel.h"

/**
 * 卡牌视图类
 * 负责显示卡牌的视觉效果
 */
class CardView : public cocos2d::Sprite {
private:
    CardModel* _cardModel; // 卡牌数据模型
    bool _isFlipped;       // 是否翻开
    cocos2d::Sprite* _frontSprite; // 卡牌正面
    cocos2d::Sprite* _backSprite;  // 卡牌背面
    std::function<void(CardView*)> _onClickCallback; // 点击回调

public:
    /**
     * 创建卡牌视图
     * @param cardModel 卡牌数据模型
     * @return 卡牌视图
     */
    static CardView* create(CardModel* cardModel);

    /**
     * 初始化
     * @param cardModel 卡牌数据模型
     * @return 是否初始化成功
     */
    bool init(CardModel* cardModel);

    /**
     * 翻牌动画
     * @param flipped 是否翻开
     */
    void flip(bool flipped);

    /**
     * 设置点击回调
     * @param callback 点击回调
     */
    void setOnClickCallback(const std::function<void(CardView*)>& callback);

    /**
     * 获取卡牌数据模型
     * @return 卡牌数据模型
     */
    CardModel* getCardModel() const;

    /**
     * 匹配动画
     */
    void playMatchAnimation();

private:
    /**
     * 触摸事件处理
     * @param touch 触摸事件
     * @param event 事件
     * @return 是否处理
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     * 触摸移动事件处理
     * @param touch 触摸事件
     * @param event 事件
     */
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     * 触摸结束事件处理
     * @param touch 触摸事件
     * @param event 事件
     */
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // CARDVIEW_H