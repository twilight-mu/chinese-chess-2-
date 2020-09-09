#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "stone.h"
#include <QPainter>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

    Stone s[32]; //定义三十二个棋子

    int _r;//棋子半径
    int _selectedid;//选中棋子id,若为-1则没有选中任何棋子

    QPoint center(int row,int col);//获取棋盘行列对应坐标
    QPoint center(int id);//获取某一棋子的中心坐标
    bool getRowCol (QPoint pt,int &row,int &col);
    void drawStone(QPainter &painter,int id);//绘画棋子
    bool CanMove(int moveid,int row,int col,int killid);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent*);


signals:

public slots:
};

#endif // BOARD_H
