#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "PlayFieldController.h"
#include "StackController.h"
#include "../managers/UndoManager.h"

/**
 * 游戏控制器类
 * 负责协调各个子控制器和管理游戏流程
 */
class GameController {
private:
    GameModel* _gameModel; // 游戏数据模型
    GameView* _gameView; // 游戏视图
    PlayFieldController* _playFieldController; // 主牌区控制器
    StackController* _stackController; // 牌堆控制器
    UndoManager* _undoManager; // 回退管理器

    bool _isAnimating; // 动画中禁止再次操作

    bool checkWinCondition() const;
    void tryShowWinOverlay();

public:
    /**
     * 构造函数
     */
    GameController();

    /**
     * 析构函数
     */
    ~GameController();

    /**
     * 初始化游戏
     * @param levelId 关卡ID
     */
    void initGame(int levelId);

    /**
     * 处理卡牌点击
     * @param cardId 卡牌ID
     * @return 是否处理成功
     */
    bool handleCardClick(int cardId);

    /**
     * 处理备用牌堆点击
     * @return 是否处理成功
     */
    bool handleStackClick();

    /**
     * 处理手牌区预览牌点击（翻牌替换）
     */
    bool handleHandCardClick(int cardId);

    /**
     * 处理回退操作
     * @return 是否回退成功
     */
    bool handleUndo();

    /**
     * 获取游戏视图
     * @return 游戏视图
     */
    GameView* getGameView() const;
};

#endif // GAMECONTROLLER_H