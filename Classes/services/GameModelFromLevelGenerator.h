#ifndef GAMEMODELFROMLEVELGENERATOR_H
#define GAMEMODELFROMLEVELGENERATOR_H

#include "../models/GameModel.h"
#include "../configs/models/LevelConfig.h"

/**
 * 游戏模型生成器
 * 负责将关卡配置转换为游戏模型
 */
class GameModelFromLevelGenerator {
private:
    /**
     * 从卡牌配置创建卡牌模型
     * @param config 卡牌配置
     * @param id 卡牌ID
     * @return 卡牌模型
     */
    static CardModel* createCardModel(const CardConfig& config, int id);

public:
    /**
     * 生成游戏模型
     * @param levelConfig 关卡配置
     * @return 游戏模型
     */
    static GameModel* generateGameModel(const LevelConfig& levelConfig);
};

#endif // GAMEMODELFROMLEVELGENERATOR_H