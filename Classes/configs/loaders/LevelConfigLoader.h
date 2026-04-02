#ifndef LEVELCONFIGLOADER_H
#define LEVELCONFIGLOADER_H

#include <string>
#include "../models/LevelConfig.h"

/**
 * 关卡配置加载器
 * 负责从JSON文件加载关卡配置
 */
class LevelConfigLoader {
private:
    /**
     * 从JSON文件加载配置
     * @param filePath 文件路径
     * @param config 关卡配置对象
     * @return 是否加载成功
     */
    static bool loadFromJson(const std::string& filePath, LevelConfig& config);

public:
    /**
     * 加载关卡配置
     * @param levelId 关卡ID
     * @return 关卡配置对象
     */
    static LevelConfig loadLevelConfig(int levelId);
};

#endif // LEVELCONFIGLOADER_H