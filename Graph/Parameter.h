#ifndef PARAMETER_H
#define PARAMETER_H

const int WINDOW_WIDTH = 1260;             //程序窗口大小（长）
const int WINDOW_HEIGHT = 779;             //程序窗口大小（高）
const char BACKGROUND_COLOR[] = "#FFFAF0"; //程序背景色
const char TITLE_COLOR[] = "#FFA07A";      //程序窗口标题字色
const char WINDOW_HEADLINE[] = "图";
//GraphInfo
const int INPUTGRID_WIDTH = 100;
const int INPUTGRID_HEIGHT = 20;
//MGraph
const int GRAPH_SPACES = 5;                         //绘图与最外围间隔
const char NODE_ID_COLOR[] = "#C55A11";             //结点编号颜色
const char NODE_BK_COLOR[] = "#FFD966";             //结点背景颜色
const char NODE_EDGE_COLOR[] = "#BF9000";           //结点边框颜色
const char NODE_ID_COLOR_SELECTED[] = "#FFFAF0";    //结点编号颜色（突出显示）
const char NODE_BK_COLOR_SELECTED[] = "#";          //结点背景颜色（突出显示）
const char NODE_EDGE_COLOR_SELECTED[] = "#";        //结点边框颜色（突出显示）
const int NODE_EDGE_WIDTH = 3;                      //结点线宽
const int NODE_TEXT_POINTSIZE = 12;                 //结点编号字体大小
const int NODE_RADIUS = 20;                         //结点半径
const char ARC_WEIGHT_COLOR[] = "#C55A11";          //边权值颜色
const char ARC_LINE_COLOR[] = "#BF9000";            //边连线颜色
const char ARC_WEIGHT_COLOR_SELECTED[] = "#FFFAF0"; //边权值颜色（突出显示）
const char ARC_LINE_COLOR_SELECTED[] = "#";         //边连线颜色（突出显示）
const int ARC_TEXT_POINTSIZE = 12;                  //边权值字体大小
const int ARC_WIDTH = 2;                            //线宽
const int ANIMATION_DURATION = 400;                 //淡入淡出时间
//ShowAdjacencyList
const int CELL_LENGTH = 40; //单元格边长
const int SMALL_CELL_LENGTH = 30;
const int H_CELL_GAP = 20; //横向结点间距
const int V_CELL_GAP = 0;  //纵向结点间距
const int H_ARROW_LENGTH = CELL_LENGTH / 2 + H_CELL_GAP;
//const int V_ARROW_LENGTH=CELL_LENGTH/2+V_CELL_GAP;
const int TEXT_WIDTH = 10;                //文字宽度
const char CELL_TEXT_COLOR[] = "#C55A11"; //链表文字颜色
const char CELL_BK_COLOR[] = "#FFD966";   //链表背景颜色
const char CELL_EDGE_COLOR[] = "#BF9000"; //链表边框颜色
const int CELL_EDGE_WIDTH = 3;            //链表线宽
//Prim
const int UPDATE_INTERVAL = 1600;          //每一轮更新时间间隔
const int COMPONENT_UPDATE_INTERVAL = UPDATE_INTERVAL/3; //每一轮中每个组件更新时间间隔(<UPDATE_INTERVAL➗3)
const char CLOSEDGE_HIGHLIGHT_TEXT_COLOR[] = "#FFA07A"; //程序窗口标题字色

#endif // PARAMETER_H
