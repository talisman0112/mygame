#include "UndoModel.h"

UndoModel::UndoModel() {
}

UndoModel::~UndoModel() {
    // OperationRecord 仅保存指针引用，不拥有卡牌内存
    _operationStack.clear();
}

void UndoModel::addMatchCardRecord(CardModel* trayCard, CardModel* targetCard, const cocos2d::Vec2& fromWorldPos) {
    OperationRecord record;
    record.type = OP_MATCH_CARD;
    record.prevTrayCard = trayCard;
    record.movedCard = targetCard;
    record.fromWorldPos = fromWorldPos;
    record.movedWasMatched = targetCard ? targetCard->isMatched() : false;
    _operationStack.push_back(record);
}

void UndoModel::addDrawStackRecord(CardModel* trayCard, CardModel* stackCard, const cocos2d::Vec2& fromWorldPos) {
    OperationRecord record;
    record.type = OP_DRAW_STACK;
    record.prevTrayCard = trayCard;
    record.movedCard = stackCard;
    record.fromWorldPos = fromWorldPos;
    record.movedWasMatched = stackCard ? stackCard->isMatched() : false;
    _operationStack.push_back(record);
}

void UndoModel::addSwapHandRecord(CardModel* trayCard, CardModel* handCard, const cocos2d::Vec2& fromWorldPos) {
    OperationRecord record;
    record.type = OP_SWAP_HAND;
    record.prevTrayCard = trayCard;
    record.movedCard = handCard;
    record.fromWorldPos = fromWorldPos;
    record.movedWasMatched = handCard ? handCard->isMatched() : false;
    _operationStack.push_back(record);
}

OperationRecord* UndoModel::popOperationRecord() {
    if (_operationStack.empty()) {
        return nullptr;
    }
    
    OperationRecord* record = new OperationRecord(_operationStack.back());
    _operationStack.pop_back();
    return record;
}

bool UndoModel::canUndo() const {
    return !_operationStack.empty();
}

void UndoModel::clear() {
    _operationStack.clear();
}