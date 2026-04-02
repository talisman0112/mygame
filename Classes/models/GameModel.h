#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include "CardModel.h"

/**
 * 游戏模型类
 * 表示游戏的整体数据模型
 */
class GameModel {
private:
    // 主牌区卡牌
    std::vector<CardModel*> _playfieldCards;
    // 备用牌堆（未翻出的隐藏牌）
    std::vector<CardModel*> _stackCards;
    // 手牌区（可见可交换）
    std::vector<CardModel*> _handCards;
    // 底牌
    CardModel* _trayCard;
    // 当前翻开的牌
    CardModel* _currentFlippedCard;

public:
    /**
     * 构造函数
     */
    GameModel();

    /**
     * 析构函数
     */
    ~GameModel();

    /**
     * 添加主牌区卡牌
     * @param card 卡牌
     */
    void addPlayfieldCard(CardModel* card);

    /**
     * 添加备用牌堆卡牌
     * @param card 卡牌
     */
    void addStackCard(CardModel* card);

    /**
     * 从手牌区移除指定卡牌
     */
    void removeHandCard(CardModel* card);

    /**
     * 检查指定卡牌是否在手牌区
     */
    bool hasHandCard(CardModel* card) const;

    /**
     * 设置底牌
     * @param card 卡牌
     */
    void setTrayCard(CardModel* card);

    /**
     * 获取底牌
     * @return 底牌
     */
    CardModel* getTrayCard() const;

    /**
     * 获取主牌区卡牌列表
     * @return 主牌区卡牌列表
     */
    const std::vector<CardModel*>& getPlayfieldCards() const;

    /**
     * 获取备用牌堆卡牌列表
     * @return 备用牌堆卡牌列表
     */
    const std::vector<CardModel*>& getStackCards() const;

    /**
     * 获取手牌区卡牌列表
     */
    const std::vector<CardModel*>& getHandCards() const;

    /**
     * 从备用牌堆抽取卡牌
     * @return 抽取的卡牌，如果没有则返回nullptr
     */
    CardModel* drawStackCard();

    /**
     * 从备用牌堆抽取一张牌加入手牌区
     */
    CardModel* drawCardToHand();

    /**
     * 使用手牌区中的一张牌与顶部牌交换
     * @param stackCard 手牌区中的卡牌
     * @return 是否交换成功
     */
    bool swapTrayWithStackCard(CardModel* stackCard);

    /**
     * 使用桌面牌替换顶部牌，并将旧顶部牌放回手牌区
     * @param card 桌面匹配成功的卡牌
     * @return 是否替换成功
     */
    bool replaceTrayWithMatchedCard(CardModel* card);

    /**
     * 检查卡牌是否可以匹配
     * @param card 要检查的卡牌
     * @return 是否可以匹配
     */
    bool canMatchCard(CardModel* card) const;

    /**
     * 匹配卡牌
     * @param card 要匹配的卡牌
     * @return 是否匹配成功
     */
    bool matchCard(CardModel* card);

    /**
     * 设置当前翻开的牌
     * @param card 当前翻开的牌
     */
    void setCurrentFlippedCard(CardModel* card);

    /**
     * 获取当前翻开的牌
     * @return 当前翻开的牌
     */
    CardModel* getCurrentFlippedCard() const;

    /**
     * 根据ID查找卡牌（主牌区与备用牌堆）
     * @param cardId 卡牌ID
     * @return 卡牌指针，如果找不到则返回nullptr
     */
    CardModel* findCardById(int cardId) const;
};

#endif // GAMEMODEL_H