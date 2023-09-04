#include <windows.h> 
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "display.h"
#include "time.h"
/*
具体常见设置属性如下：
属性	意义
FOREGROUND_BLUE			前景色包含 蓝色
FOREGROUND_GREEN		前景色包含 绿色
FOREGROUND_RED			前景色包含 红色
FOREGROUND_INTENSITY	前景色加强
BACKGROUND_BLUE			背景色包含 蓝色
BACKGROUND_GREEN		背景色包含 绿色
BACKGROUND_RED			背景色包含 红色
BACKGROUND_INTENSITY	背景色加强
COMMON_LVB_GRID_HORIZONTAL	顶部水平网格
COMMON_LVB_GRID_LVERTICAL	左竖直网格
COMMON_LVB_GRID_RVERTICAL	右竖直网格
COMMON_LVB_UNDERSCORE	下划线

*/


HANDLE hOut;

#define UP			0x48
#define DOWN		0x50
#define LEFT		0x4B
#define RIGHT  		0x4D

extern Menu *currentMenu;
extern uint32_t LCD_TaskFlag;
int getKey(void) {
	if (getch() == 0xE0) {
		switch (getch()) {
			case UP:
				if (currentMenu->selected == 0) {
					currentMenu->selected = currentMenu->itemCount;
				}
				currentMenu->selected--;
			return 1;
			
			case DOWN:
				currentMenu->selected++;
				if (currentMenu->selected >= currentMenu->itemCount) {
					currentMenu->selected = 0;
				}
			return 2;
			
			case LEFT:
				Menu_return(currentMenu);
			return 3;
			
			case RIGHT:
				Menu_enter(currentMenu, currentMenu->selected);
			return 4;
			
			default:
			return 0;
		}
	}
	return 0;
} 
	
HANDLE hOut; 
COORD pos = {0,0};
CONSOLE_CURSOR_INFO cci;    //定义结构体
int main(void) 
{
	int key = 0;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE); 
    cci.bVisible =  0; //设置光标不可见 FALSE
    SetConsoleCursorInfo(hOut, &cci);    //设置(应用)光标信息
    menu_init();
    
    while (1) {
    	
    	if(!(LCD_TaskFlag & TASK_SET)) {
            // 显示菜单
           	Menu_displayNor(currentMenu);
        }else{
            // 进入设置函数
            (*currentMenu->func[currentMenu->selected])();
        }
    	
		LCD_Dispaly();
		
    	getKey();
		LCD_Clear();	
	}
    return 0; 
}