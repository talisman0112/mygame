#ifndef UNDOMODEL_H
#define UNDOMODEL_H

#include <vector>
#include "CardModel.h"

/**
 * 操作类型
 */
enum OperationType {
    OP_MATCH_CARD,    // 桌面牌匹配到底牌
    OP_DRAW_STACK,    // 从备用牌堆抽到手牌
    OP_SWAP_HAND      // 手牌与牌顶交换
};

/**
 * 操作记录
 */
struct OperationRecord {
    OperationType type;      // 操作类型
    CardModel* prevTrayCard; // 操作前的底牌
    CardModel* movedCard;    // 被移动并替换为新底牌的卡牌
    cocos2d::Vec2 fromWorldPos; // 移动前的世界坐标（用于回退动画）
    bool movedWasMatched;    // 操作前 movedCard 的 matched 状态
};

/**
 * 回退模型类
 * 用于记录操作历史，支持回退功能
 */
class UndoModel {
private:
    // 操作记录栈
    std::vector<OperationRecord> _operationStack;

public:
    /**
     * 构造函数
     */
    UndoModel();

    /**
     * 析构函数
     */
    ~UndoModel();

    /**
     * 添加匹配卡牌操作记录
     * @param trayCard 操作前的底牌
     * @param targetCard 匹配的卡牌
     */
    void addMatchCardRecord(CardModel* trayCard, CardModel* targetCard, const cocos2d::Vec2& fromWorldPos);

    /**
     * 添加抽取备用牌操作记录
     * @param trayCard 操作前的底牌
     * @param stackCard 抽取的卡牌
     */
    void addDrawStackRecord(CardModel* trayCard, CardModel* stackCard, const cocos2d::Vec2& fromWorldPos);
    void addSwapHandRecord(CardModel* trayCard, CardModel* handCard, const cocos2d::Vec2& fromWorldPos);

    /**
     * 弹出最近的操作记录
     * @return 操作记录，如果没有则返回nullptr
     */
    OperationRecord* popOperationRecord();

    /**
     * 检查是否可以回退
     * @return 是否可以回退
     */
    bool canUndo() const;

    /**
     * 清空操作记录
     */
    void clear();
};

#endif // UNDOMODEL_H