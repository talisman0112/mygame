#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include <vector>
#include <string>
#include "cocos2d.h"

/**
 * 卡牌配置结构
 */
struct CardConfig {
    int CardFace;     // 牌面数字
    int CardSuit;     // 花色
    cocos2d::Vec2 Position; // 位置
};

/**
 * 关卡配置类
 * 负责管理游戏关卡的配置信息
 */
class LevelConfig {
private:
    // 主牌区卡牌配置
    std::vector<CardConfig> _playfieldCards;
    // 备用牌堆配置
    std::vector<CardConfig> _stackCards;

public:
    /**
     * 构造函数
     */
    LevelConfig();

    /**
     * 析构函数
     */
    ~LevelConfig();

    /**
     * 添加主牌区卡牌配置
     * @param card 卡牌配置
     */
    void addPlayfieldCard(const CardConfig& card);

    /**
     * 添加备用牌堆卡牌配置
     * @param card 卡牌配置
     */
    void addStackCard(const CardConfig& card);

    /**
     * 获取主牌区卡牌配置列表
     * @return 主牌区卡牌配置列表
     */
    const std::vector<CardConfig>& getPlayfieldCards() const;

    /**
     * 获取备用牌堆卡牌配置列表
     * @return 备用牌堆卡牌配置列表
     */
    const std::vector<CardConfig>& getStackCards() const;
};

#endif // LEVELCONFIG_H