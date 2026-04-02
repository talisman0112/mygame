#ifndef PLAYFIELDCONTROLLER_H
#define PLAYFIELDCONTROLLER_H

#include <vector>
#include "../models/GameModel.h"
#include "../views/PlayFieldView.h"
#include "CardController.h"

/**
 * 主牌区控制器类
 * 负责处理主牌区的逻辑
 */
class PlayFieldController {
private:
    GameModel* _gameModel; // 游戏数据模型
    PlayFieldView* _playFieldView; // 主牌区视图
    std::vector<CardController*> _cardControllers; // 卡牌控制器列表
    std::function<void(int)> _onCardClickCallback; // 卡牌点击回调

public:
    /**
     * 构造函数
     * @param gameModel 游戏数据模型
     * @param playFieldView 主牌区视图
     */
    PlayFieldController(GameModel* gameModel, PlayFieldView* playFieldView);

    /**
     * 析构函数
     */
    ~PlayFieldController();

    /**
     * 初始化
     */
    void init();

    /**
     * 处理卡牌点击
     * @param cardId 卡牌ID
     * @return 是否处理成功
     */
    bool handleCardClick(int cardId);

    /**
     * 设置卡牌点击回调
     * @param callback 卡牌点击回调
     */
    void setOnCardClickCallback(const std::function<void(int)>& callback);

    /**
     * 获取卡牌控制器
     * @param cardId 卡牌ID
     * @return 卡牌控制器
     */
    CardController* getCardController(int cardId) const;

    /**
     * 播放匹配动画
     * @param cardId 卡牌ID
     */
    void playMatchAnimation(int cardId);
};

#endif // PLAYFIELDCONTROLLER_H