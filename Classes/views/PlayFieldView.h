#ifndef PLAYFIELDVIEW_H
#define PLAYFIELDVIEW_H

#include "cocos2d.h"
#include <vector>
#include "CardView.h"

/**
 * 主牌区视图类
 * 负责显示主牌区的所有卡牌
 */
class PlayFieldView : public cocos2d::Layer {
private:
    std::vector<CardView*> _cardViews;
    std::function<void(int)> _onCardClickCallback;

public:
    /**
     * 创建主牌区视图
     * @return 主牌区视图
     */
    static PlayFieldView* create();

    /**
     * 初始化
     * @return 是否初始化成功
     */
    bool init() override;

    /**
     * 添加卡牌视图
     * @param cardView 卡牌视图
     */
    void addCardView(CardView* cardView);

    /**
     * 移除卡牌视图
     * @param cardView 卡牌视图
     */
    void removeCardView(CardView* cardView);

    /**
     * 设置卡牌点击回调
     * @param callback 卡牌点击回调
     */
    void setOnCardClickCallback(const std::function<void(int)>& callback);

    /**
     * 获取卡牌视图
     * @param cardId 卡牌ID
     * @return 卡牌视图
     */
    CardView* getCardView(int cardId) const;

    /**
     * 播放匹配动画
     * @param cardId 卡牌ID
     */
    void playMatchAnimation(int cardId);
};

#endif // PLAYFIELDVIEW_H