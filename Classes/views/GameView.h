#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "cocos2d.h"
#include "PlayFieldView.h"
#include "StackView.h"

/**
 * 游戏视图类
 * 负责显示整个游戏的视图
 */
class GameView : public cocos2d::Layer {
private:
    PlayFieldView* _playFieldView; // 主牌区视图
    StackView* _stackView;         // 牌堆视图
    cocos2d::MenuItem* _undoButton; // 回退按钮
    cocos2d::Menu* _menu; // UI菜单
    cocos2d::LayerColor* _winOverlay; // 胜利遮罩

public:
    GameView();

    /**
     * 创建游戏视图
     * @return 游戏视图
     */
    static GameView* create();

    /**
     * 初始化
     * @return 是否初始化成功
     */
    bool init() override;

    /**
     * 获取主牌区视图
     * @return 主牌区视图
     */
    PlayFieldView* getPlayFieldView() const;

    /**
     * 获取牌堆视图
     * @return 牌堆视图
     */
    StackView* getStackView() const;

    /**
     * 设置回退按钮点击回调
     * @param callback 回退按钮点击回调
     */
    void setOnUndoCallback(const std::function<void()>& callback);

    /**
     * 显示胜利界面和退出按钮
     */
    void showWinOverlay();
};

#endif // GAMEVIEW_H