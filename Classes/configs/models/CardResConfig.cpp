#include "CardResConfig.h"

CardResConfig::CardResConfig() {
    // 初始化卡牌资源路径
    _cardBackPath = "res/card_general.png";
    _cardFrontPath = "res/card_general.png";

    // 初始化数字图片路径
    _numberPaths[0] = "res/number/big_red_A.png";
    _numberPaths[1] = "res/number/big_red_2.png";
    _numberPaths[2] = "res/number/big_red_3.png";
    _numberPaths[3] = "res/number/big_red_4.png";
    _numberPaths[4] = "res/number/big_red_5.png";
    _numberPaths[5] = "res/number/big_red_6.png";
    _numberPaths[6] = "res/number/big_red_7.png";
    _numberPaths[7] = "res/number/big_red_8.png";
    _numberPaths[8] = "res/number/big_red_9.png";
    _numberPaths[9] = "res/number/big_red_10.png";
    _numberPaths[10] = "res/number/big_red_J.png";
    _numberPaths[11] = "res/number/big_red_Q.png";
    _numberPaths[12] = "res/number/big_red_K.png";

    // 初始化花色图片路径
    _suitPaths[0] = "res/suits/club.png";
    _suitPaths[1] = "res/suits/diamond.png";
    _suitPaths[2] = "res/suits/heart.png";
    _suitPaths[3] = "res/suits/spade.png";
}

CardResConfig::~CardResConfig() {
}

std::string CardResConfig::getCardBackPath() const {
    return _cardBackPath;
}

std::string CardResConfig::getCardFrontPath() const {
    return _cardFrontPath;
}

std::string CardResConfig::getNumberPath(int face) const {
    auto it = _numberPaths.find(face);
    if (it != _numberPaths.end()) {
        return it->second;
    }
    return "";
}

std::string CardResConfig::getSuitPath(int suit) const {
    auto it = _suitPaths.find(suit);
    if (it != _suitPaths.end()) {
        return it->second;
    }
    return "";
}