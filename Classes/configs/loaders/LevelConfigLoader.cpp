#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"

USING_NS_CC;

bool LevelConfigLoader::loadFromJson(const std::string& filePath, LevelConfig& config) {
    // 读取JSON文件内容
    std::string content = FileUtils::getInstance()->getStringFromFile(filePath);
    if (content.empty()) {
        return false;
    }

    // 解析JSON
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(content.c_str());

    if (doc.HasParseError()) {
        return false;
    }

    // 解析主牌区卡牌
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& playfieldArray = doc["Playfield"];
        for (size_t i = 0; i < playfieldArray.Size(); i++) {
            const rapidjson::Value& cardObj = playfieldArray[i];
            if (cardObj.HasMember("CardFace") && cardObj.HasMember("CardSuit") && cardObj.HasMember("Position")) {
                CardConfig card;
                card.CardFace = cardObj["CardFace"].GetInt();
                card.CardSuit = cardObj["CardSuit"].GetInt();
                
                const rapidjson::Value& posObj = cardObj["Position"];
                if (posObj.HasMember("x") && posObj.HasMember("y")) {
                    card.Position.x = posObj["x"].GetDouble();
                    card.Position.y = posObj["y"].GetDouble();
                }
                
                config.addPlayfieldCard(card);
            }
        }
    }

    // 解析备用牌堆卡牌
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const rapidjson::Value& stackArray = doc["Stack"];
        for (size_t i = 0; i < stackArray.Size(); i++) {
            const rapidjson::Value& cardObj = stackArray[i];
            if (cardObj.HasMember("CardFace") && cardObj.HasMember("CardSuit") && cardObj.HasMember("Position")) {
                CardConfig card;
                card.CardFace = cardObj["CardFace"].GetInt();
                card.CardSuit = cardObj["CardSuit"].GetInt();
                
                const rapidjson::Value& posObj = cardObj["Position"];
                if (posObj.HasMember("x") && posObj.HasMember("y")) {
                    card.Position.x = posObj["x"].GetDouble();
                    card.Position.y = posObj["y"].GetDouble();
                }
                
                config.addStackCard(card);
            }
        }
    }

    return true;
}

LevelConfig LevelConfigLoader::loadLevelConfig(int levelId) {
    LevelConfig config;
    
    // 构建关卡配置文件路径
    std::string filePath = StringUtils::format("res/level_%d.json", levelId);
    
    // 尝试加载配置文件
    if (!loadFromJson(filePath, config)) {
        // 如果加载失败，使用默认配置
        // 这里使用README文档中的示例配置
        CardConfig card1 = {12, 0, Vec2(250, 1000)};
        CardConfig card2 = {2, 0, Vec2(300, 800)};
        CardConfig card3 = {2, 1, Vec2(350, 600)};
        CardConfig card4 = {2, 0, Vec2(850, 1000)};
        CardConfig card5 = {2, 0, Vec2(800, 800)};
        CardConfig card6 = {1, 3, Vec2(750, 600)};
        
        config.addPlayfieldCard(card1);
        config.addPlayfieldCard(card2);
        config.addPlayfieldCard(card3);
        config.addPlayfieldCard(card4);
        config.addPlayfieldCard(card5);
        config.addPlayfieldCard(card6);
        
        CardConfig stack1 = {2, 0, Vec2(0, 0)};
        CardConfig stack2 = {0, 2, Vec2(0, 0)};
        CardConfig stack3 = {3, 0, Vec2(0, 0)};
        
        config.addStackCard(stack1);
        config.addStackCard(stack2);
        config.addStackCard(stack3);
    }
    
    return config;
}