#include<stdio.h>

unsigned f2u(float f);/* 浮点数转为相同位表示的无符号数 */
int main()
{
    
    return 0;
}

/* 返回1当x <= y.否则返回0 */
int float_le(float x, float y)
{
    unsigned ux = f2u(x);
    unsigned uy = f2u(y);

    /* 获得符号位 */
    unsigned sx = ux >> 31;
    unsigned sy = uy >> 31;

    /* 
    异号时x为负返回1 
    unsigned sign_diff = sx ^ sy & sx;

    同号时x - y符号位为负返回1 
    unsigned sign_same = (ux - uy) >> 31;

    错误:无法处理正负0的情况
    参考答案:
    将x,y分为
    1. x = y = 0
    2. x >= 0, y < 0
    3, x >= 0, y >= 0
    4. x < 0, y < 0;
    */
    return (sx ^ sy & sx) | ((ux - uy) >> 31);
}