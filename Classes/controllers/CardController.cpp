#include "CardController.h"

CardController::CardController(CardModel* cardModel, CardView* cardView) {
    _cardModel = cardModel;
    _cardView = cardView;
}

CardController::~CardController() {
}

CardModel* CardController::getCardModel() const {
    return _cardModel;
}

CardView* CardController::getCardView() const {
    return _cardView;
}

void CardController::flipCard(bool flipped) {
    if (_cardView) {
        _cardView->flip(flipped);
    }
}

void CardController::playMatchAnimation() {
    if (_cardView) {
        _cardView->playMatchAnimation();
    }
}