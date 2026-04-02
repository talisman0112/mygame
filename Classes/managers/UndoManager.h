#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include "../models/UndoModel.h"
#include "../models/GameModel.h"

/**
 * 回退管理器类
 * 负责处理回退操作
 */
class UndoManager {
private:
    UndoModel* _undoModel; // 回退数据模型
    GameModel* _gameModel; // 游戏数据模型

public:
    /**
     * 构造函数
     * @param gameModel 游戏数据模型
     */
    UndoManager(GameModel* gameModel);

    /**
     * 析构函数
     */
    ~UndoManager();

    /**
     * 初始化
     */
    void init();

    /**
     * 记录匹配卡牌操作
     * @param trayCard 操作前的底牌
     * @param targetCard 匹配的卡牌
     */
    void recordMatchCard(CardModel* trayCard, CardModel* targetCard);
    void recordMatchCard(CardModel* trayCard, CardModel* targetCard, const cocos2d::Vec2& fromWorldPos);

    /**
     * 记录抽取备用牌操作
     * @param trayCard 操作前的底牌
     * @param stackCard 抽取的卡牌
     */
    void recordDrawStack(CardModel* trayCard, CardModel* stackCard);
    void recordDrawStack(CardModel* trayCard, CardModel* stackCard, const cocos2d::Vec2& fromWorldPos);
    void recordSwapHand(CardModel* trayCard, CardModel* handCard, const cocos2d::Vec2& fromWorldPos);

    /**
     * 执行回退操作
     * @return 是否回退成功
     */
    bool undo();

    /**
     * 弹出最近一次操作记录（用于驱动回退动画）
     * @param outRecord 输出操作记录
     * @return 是否成功弹出
     */
    bool popLastRecord(OperationRecord& outRecord);

    /**
     * 检查是否可以回退
     * @return 是否可以回退
     */
    bool canUndo() const;

    /**
     * 清空回退记录
     */
    void clear();
};

#endif // UNDOMANAGER_H