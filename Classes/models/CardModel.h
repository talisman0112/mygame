#ifndef CARDMODEL_H
#define CARDMODEL_H

#include "cocos2d.h"

/**
 * 花色类型
 */
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

/**
 * 正面类型
 */
enum CardFaceType {
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

/**
 * 卡牌模型类
 * 表示卡牌的数据模型
 */
class CardModel {
private:
    int _id;         // 卡牌ID
    int _face;       // 牌面数字
    int _suit;       // 花色
    bool _isFlipped; // 是否翻开
    bool _isMatched; // 是否已匹配
    cocos2d::Vec2 _position; // 位置

public:
    /**
     * 构造函数
     * @param id 卡牌ID
     * @param face 牌面数字
     * @param suit 花色
     * @param position 位置
     */
    CardModel(int id, int face, int suit, const cocos2d::Vec2& position);

    /**
     * 析构函数
     */
    ~CardModel();

    /**
     * 获取卡牌ID
     * @return 卡牌ID
     */
    int getId() const;

    /**
     * 获取牌面数字
     * @return 牌面数字
     */
    int getFace() const;

    /**
     * 获取花色
     * @return 花色
     */
    int getSuit() const;

    /**
     * 检查卡牌是否翻开
     * @return 是否翻开
     */
    bool isFlipped() const;

    /**
     * 设置卡牌是否翻开
     * @param flipped 是否翻开
     */
    void setFlipped(bool flipped);

    /**
     * 检查卡牌是否已匹配
     * @return 是否已匹配
     */
    bool isMatched() const;

    /**
     * 设置卡牌是否已匹配
     * @param matched 是否已匹配
     */
    void setMatched(bool matched);

    /**
     * 获取卡牌位置
     * @return 卡牌位置
     */
    const cocos2d::Vec2& getPosition() const;

    /**
     * 设置卡牌位置
     * @param position 卡牌位置
     */
    void setPosition(const cocos2d::Vec2& position);
};

#endif // CARDMODEL_H