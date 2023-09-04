#include "display.h"
#include "stdlib.h" 
#include <stdio.h>
#include "windows.h"
#include "conio.h"

// extern main.c
extern HANDLE hOut;
extern COORD pos;
extern CONSOLE_CURSOR_INFO cci;



Menu *currentMenu = NULL;//当前的菜单
Menu *mainMenu = NULL;

char f_LCD_clear;
char LCD_Screen[4][17];
// 显示值中间变量
float LCD_DisplayTempValue = 2.0;
uint32_t LCD_TaskFlag = 0;
//
// 显示菜单选项
// menu：菜单指针
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
        if (i+menu->range_from != menu->selected) {  // 不是当前被选项
            LCD_displayStr(i, 0, menu->menuItems[i+menu->range_from]);
        } else {
            LCD_displayStrRev(i, 0, menu->menuItems[i+menu->range_from]);
        } 
    }
    // 补全剩余行数 为空行
    for (;i<4;i++) {
        LCD_displayStr(i, 0, MSG_LCD_NULL);
    }
}



/*
 * 添加下级单项菜单函数
 * menu：        菜单指针
 * submenus：    下级单项菜单
 * index：       索引值
 * return：索引值 error：-1  
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
   
    //menu的下级菜单索引值[i]为subMenu
    menu->subMenus[index] = subMenu; 
    menu->menuId[index+1] = (menu->menuId[0] << 4) | (index + 1);
    subMenu->menuId[0] = menu->menuId[index+1];
    //subMenu的上级菜单为menu
    subMenu->parent = menu;
    //printf("menu->id=%02x, submenu->id=%02x\r\n", menu->menuId[0], subMenu->menuId[0]);
    return index;
}

/*
 * 返回上级菜单
 * menu：        菜单指针
 */
void Menu_return(Menu * menu) {
    if (menu == 0){
        DEBUGOUT("menu is null");
        return ;
    }
    // LCD清屏操作
    f_LCD_clear = SET;
	if (LCD_TaskFlag & TASK_SET) {
		LCD_TaskFlag &= ~TASK_SET;
	} else {
	    //如果有上级菜单的话
	    if (menu->parent) {
	        currentMenu = menu->parent;
	    }
	}
}

/*
 * 进入子菜单
 * menu：        菜单指针 
 * index：       子菜单索引
 * 如果存在函数，首先运行函数。
 */
void Menu_enter(Menu * menu, uint16_t index) {
    // LCD清屏操作
    f_LCD_clear = SET;

    //索引值大于项目总数 且 不具有下级子菜单
    if (index >= menu->itemCount 
     || (menu->subMenus[index] == NULL 
     || menu->subMenus == NULL)) {
        //如果存在函数  执行函数
        if (menu->func != NULL && menu->func[index] != NULL) {
        	LCD_TaskFlag |= TASK_SET;
            // 执行函数
            (*menu->func[index])();
        }
    } else {
        // 进入子菜单
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
    //添加调用函数
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
 * 以下程序，可以放置在本文件外部
 * 属于本菜单程序实例 
 */

                   
//一级菜单
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

// 二级菜单
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