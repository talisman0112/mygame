#ifndef CARDRESCONFIG_H
#define CARDRESCONFIG_H

#include <string>
#include <map>

/**
 * 卡牌资源配置类
 * 负责管理卡牌相关的资源路径和配置
 */
class CardResConfig {
private:
    // 卡牌背面图片路径
    std::string _cardBackPath;
    // 卡牌正面图片路径
    std::string _cardFrontPath;
    // 数字图片路径映射
    std::map<int, std::string> _numberPaths;
    // 花色图片路径映射
    std::map<int, std::string> _suitPaths;

public:
    /**
     * 构造函数
     */
    CardResConfig();

    /**
     * 析构函数
     */
    ~CardResConfig();

    /**
     * 获取卡牌背面图片路径
     * @return 卡牌背面图片路径
     */
    std::string getCardBackPath() const;

    /**
     * 获取卡牌正面图片路径
     * @return 卡牌正面图片路径
     */
    std::string getCardFrontPath() const;

    /**
     * 获取数字图片路径
     * @param face 牌面数字
     * @return 数字图片路径
     */
    std::string getNumberPath(int face) const;

    /**
     * 获取花色图片路径
     * @param suit 花色
     * @return 花色图片路径
     */
    std::string getSuitPath(int suit) const;
};

#endif // CARDRESCONFIG_H