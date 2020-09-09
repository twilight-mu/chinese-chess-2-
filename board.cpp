#include "board.h"
#include "zhujiemian.h"
#include <QPainter>
#include <QMouseEvent>
Board::Board(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("中国象棋");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background,QColor(218,165,32));
    this->setPalette(palette);
    for(int i=0;i<32;i++)
    {
        s[i].init(i);//棋子初始化
    }
    _selectedid=-1;
}

void drawbiao(QPainter& painter,int i,int j)
{
    painter.drawLine(QPoint(i-3,j-10),QPoint(i-3,j-3));
    painter.drawLine(QPoint(i-10,j-3),QPoint(i-3,j-3));
    painter.drawLine(QPoint(i+3,j-10),QPoint(i+3,j-3));
    painter.drawLine(QPoint(i+3,j-3),QPoint(i+10,j-3));
    painter.drawLine(QPoint(i+3,j+3),QPoint(i+3,j+10));
    painter.drawLine(QPoint(i+3,j+3),QPoint(i+10,j+3));
    painter.drawLine(QPoint(i-10,j+3),QPoint(i-3,j+3));
    painter.drawLine(QPoint(i-3,j+3),QPoint(i-3,j+10));
}

void Board::paintEvent(QPaintEvent *)//绘制棋盘、棋子
{
    QPainter painter(this);
    int d=40;
    _r=d/2;
    for(int i=1;i<=10;i++)//横线
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    painter.setFont(QFont("楷体",20,30));
    painter.drawText(QPoint(d,d*6-7), "   楚河     汉界   ");

    painter.drawLine(QPoint(9*d-3,4*d-10),QPoint(9*d-3,4*d-3));
    painter.drawLine(QPoint(9*d-10,4*d-3),QPoint(9*d-3,4*d-3));
    painter.drawLine(QPoint(9*d-10,4*d+3),QPoint(9*d-3,4*d+3));
    painter.drawLine(QPoint(9*d-3,4*d+3),QPoint(9*d-3,4*d+10));

    painter.drawLine(QPoint(d+3,4*d-10),QPoint(d+3,4*d-3));
    painter.drawLine(QPoint(d+3,4*d-3),QPoint(d+10,4*d-3));
    painter.drawLine(QPoint(d+3,4*d+3),QPoint(d+3,4*d+10));
    painter.drawLine(QPoint(d+3,4*d+3),QPoint(d+10,4*d+3));

    painter.drawLine(QPoint(9*d-3,7*d-10),QPoint(9*d-3,7*d-3));
    painter.drawLine(QPoint(9*d-10,7*d-3),QPoint(9*d-3,7*d-3));
    painter.drawLine(QPoint(9*d-10,7*d+3),QPoint(9*d-3,7*d+3));
    painter.drawLine(QPoint(9*d-3,7*d+3),QPoint(9*d-3,7*d+10));

    painter.drawLine(QPoint(d+3,7*d-10),QPoint(d+3,7*d-3));
    painter.drawLine(QPoint(d+3,7*d-3),QPoint(d+10,7*d-3));
    painter.drawLine(QPoint(d+3,7*d+3),QPoint(d+3,7*d+10));
    painter.drawLine(QPoint(d+3,7*d+3),QPoint(d+10,7*d+3));

    drawbiao(painter,2*d,3*d);
    drawbiao(painter,8*d,3*d);
    drawbiao(painter,2*d,8*d);
    drawbiao(painter,8*d,8*d);
    drawbiao(painter,3*d,4*d);
    drawbiao(painter,5*d,4*d);
    drawbiao(painter,7*d,4*d);
    drawbiao(painter,3*d,7*d);
    drawbiao(painter,5*d,7*d);
    drawbiao(painter,7*d,7*d);


    for(int i=1;i<=9;i++)//竖线
    {
        if(i==1||i==9)
        painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        else
        {
        painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
        painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }
    }
    painter.drawLine(QPoint(4*d,1*d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,1*d),QPoint(4*d,3*d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    for(int i=0;i<32;i++) //画棋子
    {
        drawStone(painter,i);
    }
}

QPoint Board::center(int row, int col) //获取棋盘行列对应坐标
{
    QPoint ret;
    ret.rx()=(col+1)*_r*2;
    ret.ry()=(row+1)*_r*2;
    return ret;
}

QPoint Board::center(int id)//获取棋子的中心坐标
{
    return center(s[id]._row,s[id]._col);
}

void Board::drawStone(QPainter& painter,int id)//画棋子
{
    if(s[id]._dead)//棋子被吃则直接返回
        return;

    QPoint c= center(id);
    QRect rect=QRect(c.x()-_r,c.y()-_r,_r*2,_r*2);
    if(id==_selectedid)
    painter.setBrush(QBrush(QColor(Qt::gray)));
    else
    painter.setBrush(QBrush(QColor(205,133,63)));

    painter.setPen(Qt::black);
    painter.drawEllipse(center(id),_r,_r);
    if(s[id]._color==1)
        painter.setPen(Qt::red);
    painter.setFont(QFont("楷体",_r,700));
    painter.drawText(rect,s[id].getText(),QTextOption(Qt::AlignCenter));
}

bool Board::getRowCol(QPoint pt, int &row, int &col)//每个点定义一个鼠标获取区间
{
    for(row=0;row<=9;row++)
    {
        for(col=0;col<=8;col++)
        {
            QPoint c=center(row,col);
            int x=c.x()-pt.x();
            int y=c.y()-pt.y();
            int len=x*x+y*y;
            if(len<_r*_r)
                return true;
        }
    }
    return false;
}

bool Board::CanMove(int moveid, int row, int col, int killid)
{
    if(s[moveid]._color==s[killid]._color)//两棋子颜色相同不会吃子，且切换选择的棋子
    {
        _selectedid=killid;
        return false;
    }
    return true;
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint pt= ev->pos();//获取鼠标选中的点信息
    int row,col;
    bool bret=getRowCol(pt,row,col);
    if(bret==false)//获取不到点信息，点到棋盘外
        return;
    int i;
    int clickedid=-1;//获取每次点击时是否选中棋子,选中棋子则赋值棋子id,若没有选中棋子则为-1
    for(i=0;i<32;i++)
    {
        if(s[i]._row==row&&s[i]._col==col&&s[i]._dead==false)
        {
            break;
        }
    }
    if(i<32)
    {
        clickedid=i;
    }
    if(_selectedid==-1)//还没有选择任何棋子,则选择棋子
    {
        if(clickedid!=-1)
        {
            _selectedid=clickedid;
            update();
        }

    }
    else//已经选择过棋子则移动棋子
    {
        if(CanMove(_selectedid,row,col,clickedid))//判断棋子走动是否合法
        {
            s[_selectedid]._row=row;
            s[_selectedid]._col=col;
            if(clickedid!=-1)//当前选中棋子移动到位置有棋子，则吃棋
                s[clickedid]._dead=true;
            _selectedid=-1;//刷新获取棋子信息
            update();
        }
    }
}
