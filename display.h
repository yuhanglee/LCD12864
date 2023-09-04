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

#define TASK_SET        0x0001  // ���ý���
#define TASK_ROOT       0x0002  // ��½�û�״̬ 0��user 1��root
#define TASK_DFP        0x0004  // ��ʾ����״̬(DFP:displayFirstPage) 0����ҳ 1���˵���
#define TASK_GPRS       0x0008  // GPRS����״̬ 0�������Է��� 1�����Է���
#define TASK_SW         0x0010  // ��ŷ�����״̬��0��û�п��� 1������
#define TASK_LOGIN      0x0020  // ��½״̬ 0��δ��¼ 1���ѵ�¼


//LCD��ʾ�����ض���
#define LCD_displayStr       my_LCD_Display_String    
#define LCD_displayStrRev    my_LCD_Display_String_Reverse
#define LCD_displayChar      my_LCD_Display_Char
#define LCD_displayCharRev   my_LCD_Display_Char_Reverse
#define LCD_Clear            my_LCD_Display_Clear


#define SUBMENUS    20
#define FUNCTION    20

typedef struct menu {  //����һ���˵�
    uint8_t range_from,range_to; //��ǰ��ʾ���ʼ���������
    uint8_t itemCount;//��Ŀ����
    uint8_t selected;//��ǰѡ����
    char menuItems[SUBMENUS][17];//�˵���Ŀ
    struct menu *subMenus[SUBMENUS];//�Ӳ˵�
    struct menu *parent;//�ϼ��˵� ,����Ƕ�����Ϊnull
    int (*func[SUBMENUS])(void);//ѡ����Ӧ�ȷ������ִ�еĺ���
    float funcValue[SUBMENUS];
    uint32_t menuId[SUBMENUS];
}Menu;

// ��Χ��Ϣ
struct ValueRange {
    float valueMax;         // ���ֵ
    float vlaueMin;         // ��Сֵ
    float valueAddSmall;    // �̰�����(����)ֵ
    float valueAddBig;      // ��������(����)ֵ
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