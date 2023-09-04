#include <windows.h> 
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "display.h"
#include "time.h"
/*
���峣�������������£�
����	����
FOREGROUND_BLUE			ǰ��ɫ���� ��ɫ
FOREGROUND_GREEN		ǰ��ɫ���� ��ɫ
FOREGROUND_RED			ǰ��ɫ���� ��ɫ
FOREGROUND_INTENSITY	ǰ��ɫ��ǿ
BACKGROUND_BLUE			����ɫ���� ��ɫ
BACKGROUND_GREEN		����ɫ���� ��ɫ
BACKGROUND_RED			����ɫ���� ��ɫ
BACKGROUND_INTENSITY	����ɫ��ǿ
COMMON_LVB_GRID_HORIZONTAL	����ˮƽ����
COMMON_LVB_GRID_LVERTICAL	����ֱ����
COMMON_LVB_GRID_RVERTICAL	����ֱ����
COMMON_LVB_UNDERSCORE	�»���

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
CONSOLE_CURSOR_INFO cci;    //����ṹ��
int main(void) 
{
	int key = 0;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE); 
    cci.bVisible =  0; //���ù�겻�ɼ� FALSE
    SetConsoleCursorInfo(hOut, &cci);    //����(Ӧ��)�����Ϣ
    menu_init();
    
    while (1) {
    	
    	if(!(LCD_TaskFlag & TASK_SET)) {
            // ��ʾ�˵�
           	Menu_displayNor(currentMenu);
        }else{
            // �������ú���
            (*currentMenu->func[currentMenu->selected])();
        }
    	
		LCD_Dispaly();
		
    	getKey();
		LCD_Clear();	
	}
    return 0; 
}