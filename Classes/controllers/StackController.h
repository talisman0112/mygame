#ifndef STACKCONTROLLER_H
#define STACKCONTROLLER_H

#include "../models/GameModel.h"
#include "../views/StackView.h"
#include "CardController.h"

/**
 * 牌堆控制器类
 * 负责处理牌堆区的逻辑
 */
class StackController {
private:
    GameModel* _gameModel; // 游戏数据模型
    StackView* _stackView; // 牌堆视图
    CardController* _trayCardController; // 底牌控制器
    std::function<void()> _onStackClickCallback; // 备用牌堆点击回调
    std::function<void(int)> _onHandCardClickCallback; // 手牌点击回调

public:
    /**
     * 构造函数
     * @param gameModel 游戏数据模型
     * @param stackView 牌堆视图
     */
    StackController(GameModel* gameModel, StackView* stackView);

    /**
     * 析构函数
     */
    ~StackController();

    /**
     * 初始化
     */
    void init();

    /**
     * 处理备用牌堆点击
     * @return 是否处理成功
     */
    bool handleStackClick();
    void setOnStackClickCallback(const std::function<void()>& callback);

    /**
     * 处理手牌区预览牌点击（翻牌替换到底牌）
     */
    bool handleHandCardClick(int cardId);

    void setOnHandCardClickCallback(const std::function<void(int)>& callback);

    void animateHandSwapToTray(int cardId);

    /**
     * 更新底牌
     * @param cardModel 底牌数据模型
     */
    void updateTrayCard(CardModel* cardModel);

    /**
     * 使用已存在的CardView设置底牌（用于跨区域移动动画后接管）
     */
    void setTrayCardFromView(CardModel* cardModel, CardView* cardView);

    void refreshHandCards();

    /**
     * 获取底牌控制器
     * @return 底牌控制器
     */
    CardController* getTrayCardController() const;
};

#endif // STACKCONTROLLER_H