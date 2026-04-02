#include "GameModelFromLevelGenerator.h"

CardModel* GameModelFromLevelGenerator::createCardModel(const CardConfig& config, int id) {
    return new CardModel(id, config.CardFace, config.CardSuit, config.Position);
}

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig& levelConfig) {
    GameModel* gameModel = new GameModel();
    int cardId = 0;
    
    // 添加主牌区卡牌
    const auto& playfieldCards = levelConfig.getPlayfieldCards();
    for (const auto& cardConfig : playfieldCards) {
        CardModel* cardModel = createCardModel(cardConfig, cardId++);
        gameModel->addPlayfieldCard(cardModel);
    }
    
    // 添加备用牌堆卡牌
    const auto& stackCards = levelConfig.getStackCards();
    for (const auto& cardConfig : stackCards) {
        CardModel* cardModel = createCardModel(cardConfig, cardId++);
        gameModel->addStackCard(cardModel);
    }
    
    // 设置初始底牌（从备用牌堆抽取）
    CardModel* trayCard = gameModel->drawStackCard();
    if (trayCard) {
        gameModel->setTrayCard(trayCard);
    }

    // 初始再抽一张到手牌区，形成可交换的玩家手牌
    gameModel->drawCardToHand();
    
    return gameModel;
}