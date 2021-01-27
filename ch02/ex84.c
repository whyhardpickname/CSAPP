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
    按照x和y的正负号分为四种情况,排除x为正,y为负的情况,
    同时先检查x和y同时为0的特殊情况.
    因为10...0 == 00...0
    */
    return ((ux << 1) == (uy << 1)) ||
            (!sx && !sy && x <= y)    ||
            (!sx && sy)              ||
            (sx && sy && x >= y);
}