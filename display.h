#ifndef __DISPLAY
#define __DISPLAY
#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif

#define FOREGROUND_WHITE	(FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED)
#define BACKGROUND_WHITE	(BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_RED)

#define normal_display(x, y) \
    do { \
		pos.X = 50+y;\
	    pos.Y = 10+x;\
	    SetConsoleCursorPosition(hOut, pos); \
		SetConsoleTextAttribute(hOut, BACKGROUND_WHITE);\
    } while (0)
	    
#define rev_display(x, y) \
    do { \
		pos.X = 50+y;\
	    pos.Y = 10+x;\
	    SetConsoleCursorPosition(hOut, pos); \
		SetConsoleTextAttribute(hOut, BACKGROUND_GREEN);\
    } while (0)

//typedef enum {
//	FALSE = 0,
//	TRUE = !FALSE,
//} bool;

typedef enum {
	RESET = 0,
	SET = !RESET,
} FlagStatus;

#define MSG_LCD_NULL		"                " 

#define TASK_SET        0x0001  // 设置界面
#define TASK_ROOT       0x0002  // 登陆用户状态 0：user 1：root
#define TASK_DFP        0x0004  // 显示界面状态(DFP:displayFirstPage) 0：首页 1：菜单项
#define TASK_GPRS       0x0008  // GPRS发送状态 0：不可以发送 1：可以发送
#define TASK_SW         0x0010  // 电磁阀开启状态：0：没有开启 1：开启
#define TASK_LOGIN      0x0020  // 登陆状态 0：未登录 1：已登录


//LCD显示函数重定义
#define LCD_displayStr       my_LCD_Display_String    
#define LCD_displayStrRev    my_LCD_Display_String_Reverse
#define LCD_displayChar      my_LCD_Display_Char
#define LCD_displayCharRev   my_LCD_Display_Char_Reverse
#define LCD_Clear            my_LCD_Display_Clear


#define SUBMENUS    20
#define FUNCTION    20

typedef struct menu {  //定义一个菜单
    uint8_t range_from,range_to; //当前显示的项开始及结束序号
    uint8_t itemCount;//项目总数
    uint8_t selected;//当前选择项
    char menuItems[SUBMENUS][17];//菜单项目
    struct menu *subMenus[SUBMENUS];//子菜单
    struct menu *parent;//上级菜单 ,如果是顶级则为null
    int (*func[SUBMENUS])(void);//选择相应项按确定键后执行的函数
    float funcValue[SUBMENUS];
    uint32_t menuId[SUBMENUS];
}Menu;

// 范围信息
struct ValueRange {
    float valueMax;         // 最大值
    float vlaueMin;         // 最小值
    float valueAddSmall;    // 短按增加(减少)值
    float valueAddBig;      // 长按增加(减少)值
};
void    Menu_displayNor(Menu * menu);
int     Menu_addItem(Menu * menu, Menu * subMenus, uint8_t index);
uint8_t Menu_addItems(Menu * menu, Menu subMenus[],uint16_t count);
void    Menu_enter(Menu * menu, uint16_t index);
void    Menu_return(Menu * menu);
void    Menu_setParent(Menu * menu,Menu * parent);
void    Menu_setCount(Menu * menu, uint16_t count);
char    Menu_addFunc(Menu * menu, int (*fun)(void), uint8_t index);
int     Menu_addFuncs(Menu * menu, int (**funs)(void), uint8_t count);
void    LCD_Screen_Init(void);
void    LCD_Dispaly(void);
void    my_LCD_Display_String(unsigned char x, unsigned char y, char *displayStr);
void    my_LCD_Display_String_Reverse(unsigned char x, unsigned char y, char *displayStr);
void    my_LCD_Display_Char(unsigned char x, unsigned char y, char displayChar);
void    my_LCD_Display_Char_Reverse(unsigned char x, unsigned char y, char displayChar);
void    my_LCD_Display_Clear(void);
void    menu_init(void);
#define DEBUGOUT printf

#ifdef __cplusplus
}
#endif
    
#endif
