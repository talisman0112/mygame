#ifndef STACKVIEW_H
#define STACKVIEW_H

#include "cocos2d.h"
#include <vector>
#include "CardView.h"

/**
 * 牌堆视图类
 * 负责显示备用牌堆和底牌
 */
class StackView : public cocos2d::Layer {
private:
    CardView* _trayCardView; // 底牌视图
    std::vector<CardView*> _handCardViews; // 手牌区卡牌列表
    cocos2d::Sprite* _stackSprite; // 备用牌堆精灵
    std::function<void()> _onStackClickCallback; // 备用牌堆点击回调
    std::function<void(int)> _onHandCardClickCallback; // 手牌点击回调

public:
    StackView();

    /**
     * 创建牌堆视图
     * @return 牌堆视图
     */
    static StackView* create();

    /**
     * 初始化
     * @return 是否初始化成功
     */
    bool init() override;

    /**
     * 设置底牌
     * @param cardView 底牌视图
     */
    void setTrayCard(CardView* cardView);

    /**
     * 获取底牌
     * @return 底牌视图
     */
    CardView* getTrayCard() const;

    void setHandCards(const std::vector<CardView*>& cardViews);
    CardView* getHandCard(int cardId) const;
    void setOnHandCardClickCallback(const std::function<void(int)>& callback);

    /**
     * 设置备用牌堆点击回调
     * @param callback 备用牌堆点击回调
     */
    void setOnStackClickCallback(const std::function<void()>& callback);

    /**
     * 播放抽取备用牌动画
     */
    void playDrawStackAnimation();

    /**
     * 设置备用牌堆显隐
     */
    void setStackVisible(bool visible);

private:
    void layoutHandCards();

    /**
     * 触摸事件处理
     * @param touch 触摸事件
     * @param event 事件
     * @return 是否处理
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     * 触摸结束事件处理
     * @param touch 触摸事件
     * @param event 事件
     */
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // STACKVIEW_H