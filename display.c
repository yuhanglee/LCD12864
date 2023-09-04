#include "display.h"
#include "stdlib.h" 
#include <stdio.h>
#include "windows.h"
#include "conio.h"

// extern main.c
extern HANDLE hOut;
extern COORD pos;
extern CONSOLE_CURSOR_INFO cci;



Menu *currentMenu = NULL;//��ǰ�Ĳ˵�
Menu *mainMenu = NULL;

char f_LCD_clear;
char LCD_Screen[4][17];
// ��ʾֵ�м����
float LCD_DisplayTempValue = 2.0;
uint32_t LCD_TaskFlag = 0;
//
// ��ʾ�˵�ѡ��
// menu���˵�ָ��
//
void Menu_displayNor(Menu *menu) {
    uint8_t i, count = menu->range_to - menu->range_from + 1;
 
    if (menu == NULL) {
        DEBUGOUT("menu_displayNor");
        return ;
    }

    if (count > menu->itemCount) {
        DEBUGOUT("menu count out");
        printf("to=%d, form=%d\r\n", menu->range_to, menu->range_from);
        printf("count=%d, itemcount=%d\r\n", count, menu->itemCount);
        menu->range_to = menu->itemCount - 1;
        count = menu->itemCount;
    }
    
    for (i = 0;i<count;i++) {
        if (i+menu->range_from != menu->selected) {  // ���ǵ�ǰ��ѡ��
            LCD_displayStr(i, 0, menu->menuItems[i+menu->range_from]);
        } else {
            LCD_displayStrRev(i, 0, menu->menuItems[i+menu->range_from]);
        } 
    }
    // ��ȫʣ������ Ϊ����
    for (;i<4;i++) {
        LCD_displayStr(i, 0, MSG_LCD_NULL);
    }
}



/*
 * �����¼�����˵�����
 * menu��        �˵�ָ��
 * submenus��    �¼�����˵�
 * index��       ����ֵ
 * return������ֵ error��-1  
 */
int Menu_addItem(Menu *menu, Menu * subMenu, uint8_t index) {
    if (menu == NULL) {
        DEBUGOUT("menu is null");
        return -1;
    }
    
    if (index > menu->itemCount) {
        DEBUGOUT("index too big");
        return -2;
    }
   
    //menu���¼��˵�����ֵ[i]ΪsubMenu
    menu->subMenus[index] = subMenu; 
    menu->menuId[index+1] = (menu->menuId[0] << 4) | (index + 1);
    subMenu->menuId[0] = menu->menuId[index+1];
    //subMenu���ϼ��˵�Ϊmenu
    subMenu->parent = menu;
    //printf("menu->id=%02x, submenu->id=%02x\r\n", menu->menuId[0], subMenu->menuId[0]);
    return index;
}

/*
 * �����ϼ��˵�
 * menu��        �˵�ָ��
 */
void Menu_return(Menu * menu) {
    if (menu == 0){
        DEBUGOUT("menu is null");
        return ;
    }
    // LCD��������
    f_LCD_clear = SET;
	if (LCD_TaskFlag & TASK_SET) {
		LCD_TaskFlag &= ~TASK_SET;
	} else {
	    //������ϼ��˵��Ļ�
	    if (menu->parent) {
	        currentMenu = menu->parent;
	    }
	}
}

/*
 * �����Ӳ˵�
 * menu��        �˵�ָ�� 
 * index��       �Ӳ˵�����
 * ������ں������������к�����
 */
void Menu_enter(Menu * menu, uint16_t index) {
    // LCD��������
    f_LCD_clear = SET;

    //����ֵ������Ŀ���� �� �������¼��Ӳ˵�
    if (index >= menu->itemCount 
     || (menu->subMenus[index] == NULL 
     || menu->subMenus == NULL)) {
        //������ں���  ִ�к���
        if (menu->func != NULL && menu->func[index] != NULL) {
        	LCD_TaskFlag |= TASK_SET;
            // ִ�к���
            (*menu->func[index])();
        }
    } else {
        // �����Ӳ˵�
        currentMenu = menu->subMenus[index];
    }
}



char Menu_addFunc(Menu * menu, int(*fun)(void), uint8_t index) {
    if (menu == NULL) {
        DEBUGOUT("menu is null");
        return 0;
    }
    
    if (index > menu->itemCount) {
        DEBUGOUT("index is too big");
        return 0;
    }
    
    if (fun == NULL) {
        DEBUGOUT("function is null");
        return 0;
    }
    //���ӵ��ú���
    menu->func[index] = fun;
    menu->menuId[index+1] = (menu->menuId[0] << 4) | (index + 1);
    //printf("menu->id=%02x, submenu->id=%02x\r\n", menu->menuId[0], menu->menuId[index+1]);
    
    
    return 1;
}
void LCD_Screen_Init(void) {
    uint8_t row;

    for (row = 0;row < 4;row++) {
        LCD_displayStr(row, 0, MSG_LCD_NULL);
    }
}
void dchar(unsigned char ch) {
	if (ch >= ' ' && ch <= '~') {
		printf("%c", ch);
	} else {
		printf(" ");
	}
}
void LCD_Dispaly(void) {
    uint8_t row = 0, col = 0;

    for (row = 0;row < 4;row++) {
    	if (row != currentMenu->selected) {
    		for (col = 0;col < 16;col++) {
    			normal_display(row, col);
				dchar(LCD_Screen[row][col]);
		    }
	    } else {
    		for (col = 0;col < 16;col++) {
				rev_display(row, col);
		    	dchar(LCD_Screen[row][col]);
	    	}
		}
	}
}
void my_LCD_Display_String(unsigned char x, unsigned char y, char *displayStr) {
    uint8_t col, len;

	for (col = 0;col < strlen(displayStr);col++) {
    	LCD_Screen[x][y+col] = displayStr[col];
	}
}

void my_LCD_Display_String_Reverse(unsigned char x, unsigned char y, char *displayStr) {
    uint8_t col, len;
	
	for (col = 0;col < strlen(displayStr);col++) {
    	LCD_Screen[x][y+col] = displayStr[col];
	}
}
void my_LCD_Display_Clear(void) {
    uint8_t row;
	if (!f_LCD_clear) {
		return ;
	}
    for (row = 0;row < 4;row++) {
        my_LCD_Display_String(row, 0, MSG_LCD_NULL);
    }
}





/*
 * ���³��򣬿��Է����ڱ��ļ��ⲿ
 * ���ڱ��˵�����ʵ�� 
 */

                   
//һ���˵�
Menu fMenu = {
    // form, to, count, selected
    0, 3, 4, 0, 
    // items string
    {
        "1",
        "2",
        "3",
        "4",
    },
};

// �����˵�
Menu Menu_Level = {
    // form, to, count, selected
    0, 2, 3, 0, 
    // items string
    {
        "11",
        "12",
        "13",
    },
};

Menu Menu_Temp = {
    // form, to, count, selected
    0, 0, 1, 0, 
    // items string
    {
        "21",
    },
};
Menu Menu_Alarm = {
    // form, to, count, selected
    0, 1, 2, 0, 
    // items string
    {
        "211",
        "212",
    },
};

//int LCD_displayLevelHigh(void) {
//	char s[20];
//	sprintf(s, "   %.0f", LCD_DisplayTempValue);
//	LCD_displayStr(1, 0, s);
//} 
#define GET_LCD_FUNC_NAME(name)    (LCD_display##name)

#define LCD_CREATE_DISPLAY_FUNC(name, title, unit) \
static int LCD_display##name(void) { \
	char s[20]; \
	LCD_displayStr(0, 0, #title); \
	sprintf(s, "%.2f %s", LCD_DisplayTempValue, #unit); \
	LCD_displayStr(1, 3, s); \
}
LCD_CREATE_DISPLAY_FUNC(LevelHigh, level, mm);

void menu_init(void) {
	mainMenu = &fMenu;
	Menu_addFunc(&Menu_Level, GET_LCD_FUNC_NAME(LevelHigh), 0);
	Menu_addItem(mainMenu, &Menu_Level, 0);
    Menu_addItem(mainMenu, &Menu_Temp, 1);
    Menu_addItem(mainMenu, &Menu_Alarm, 2);	
	currentMenu = mainMenu;
}