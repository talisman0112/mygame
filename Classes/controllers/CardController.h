#ifndef CARDCONTROLLER_H
#define CARDCONTROLLER_H

#include "../models/CardModel.h"
#include "../views/CardView.h"

/**
 * 卡牌控制器类
 * 负责处理卡牌相关的逻辑
 */
class CardController {
private:
    CardModel* _cardModel; // 卡牌数据模型
    CardView* _cardView;   // 卡牌视图

public:
    /**
     * 构造函数
     * @param cardModel 卡牌数据模型
     * @param cardView 卡牌视图
     */
    CardController(CardModel* cardModel, CardView* cardView);

    /**
     * 析构函数
     */
    ~CardController();

    /**
     * 获取卡牌数据模型
     * @return 卡牌数据模型
     */
    CardModel* getCardModel() const;

    /**
     * 获取卡牌视图
     * @return 卡牌视图
     */
    CardView* getCardView() const;

    /**
     * 翻转卡牌
     * @param flipped 是否翻开
     */
    void flipCard(bool flipped);

    /**
     * 播放匹配动画
     */
    void playMatchAnimation();
};

#endif // CARDCONTROLLER_H