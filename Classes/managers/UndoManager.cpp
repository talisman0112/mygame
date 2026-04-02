#include "UndoManager.h"

UndoManager::UndoManager(GameModel* gameModel) {
    _gameModel = gameModel;
    _undoModel = new UndoModel();
}

UndoManager::~UndoManager() {
    if (_undoModel) {
        delete _undoModel;
        _undoModel = nullptr;
    }
}

void UndoManager::init() {
    // 初始化
}

void UndoManager::recordMatchCard(CardModel* trayCard, CardModel* targetCard) {
    if (_undoModel) {
        _undoModel->addMatchCardRecord(trayCard, targetCard, cocos2d::Vec2::ZERO);
    }
}

void UndoManager::recordMatchCard(CardModel* trayCard, CardModel* targetCard, const cocos2d::Vec2& fromWorldPos) {
    if (_undoModel) {
        _undoModel->addMatchCardRecord(trayCard, targetCard, fromWorldPos);
    }
}

void UndoManager::recordDrawStack(CardModel* trayCard, CardModel* stackCard) {
    if (_undoModel) {
        _undoModel->addDrawStackRecord(trayCard, stackCard, cocos2d::Vec2::ZERO);
    }
}

void UndoManager::recordDrawStack(CardModel* trayCard, CardModel* stackCard, const cocos2d::Vec2& fromWorldPos) {
    if (_undoModel) {
        _undoModel->addDrawStackRecord(trayCard, stackCard, fromWorldPos);
    }
}

void UndoManager::recordSwapHand(CardModel* trayCard, CardModel* handCard, const cocos2d::Vec2& fromWorldPos) {
    if (_undoModel) {
        _undoModel->addSwapHandRecord(trayCard, handCard, fromWorldPos);
    }
}

bool UndoManager::undo() {
    if (!_undoModel || !_gameModel || !_undoModel->canUndo()) {
        return false;
    }
    
    // 弹出最近的操作记录
    OperationRecord* record = _undoModel->popOperationRecord();
    if (!record) {
        return false;
    }
    
    // 根据操作类型执行回退
    switch (record->type) {
        case OP_MATCH_CARD:
            // 回退匹配卡牌操作
            if (record->prevTrayCard) {
                _gameModel->setTrayCard(record->prevTrayCard);
            }
            if (record->movedCard) {
                record->movedCard->setMatched(record->movedWasMatched);
            }
            break;
        
        case OP_DRAW_STACK:
            // 回退抽取备用牌操作
            if (record->movedCard) {
                if (_gameModel->hasHandCard(record->movedCard)) {
                    _gameModel->removeHandCard(record->movedCard);
                }
                _gameModel->addStackCard(record->movedCard);
            }
            if (record->prevTrayCard) {
                _gameModel->setTrayCard(record->prevTrayCard);
            }
            break;
        case OP_SWAP_HAND:
            if (record->prevTrayCard && record->movedCard) {
                _gameModel->swapTrayWithStackCard(record->prevTrayCard);
            }
            break;
    }
    
    // 释放操作记录
    delete record;
    
    return true;
}

bool UndoManager::popLastRecord(OperationRecord& outRecord) {
    if (!_undoModel || !_undoModel->canUndo()) {
        return false;
    }
    OperationRecord* record = _undoModel->popOperationRecord();
    if (!record) {
        return false;
    }
    outRecord = *record;
    delete record;
    return true;
}

bool UndoManager::canUndo() const {
    return _undoModel && _undoModel->canUndo();
}

void UndoManager::clear() {
    if (_undoModel) {
        _undoModel->clear();
    }
}