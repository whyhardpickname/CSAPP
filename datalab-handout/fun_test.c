#include<stdio.h>
unsigned floatPower2(int x) {
    int sign_mask = 1 << 31;
    int bias = 127;//2^(8 -1) - 1 = 127
    int E = x;

    //E最小值为阶码数全为0,尾数只有最低有效位在为1,小数点后第23位为1,即-149
    if (E & sign_mask && ((E + 149) & sign_mask))
    {
        return 0;
    }
    //E阶码数全为0,V = 2^(E + 149) * 2^(-126)
    else if (E & sign_mask && ((E + 126) & sign_mask))
    {
        return 1 << (E + 149);
    }
    //E最大值为阶码数不全为0,除了最低有效位,尾数全为0,即127
    else if ((E - 128) & sign_mask)
    {
        int e = E + bias;
        return e << 23;
    }
    return ((1 << 8) - 1) << 23;
}

int main()
{
    printf("%10s %10s %10s\n", "input", "output", "expetcted");
    printf("%10s %10x %10s\n", "-150", floatPower2(-150), "0");
    printf("%10s %10x %10s\n", "-149", floatPower2(-149), "1");
    printf("%10s %10x %10s\n", "-148", floatPower2(-148), "2");
    printf("%10s %10x %10s\n", "-127", floatPower2(-127), "400000");
    printf("%10s %10x %10s\n", "-126", floatPower2(-126), "800000");
    printf("%10s %10x %10s\n", "-125", floatPower2(-125), "1000000");
    printf("%10s %10x %10s\n", "127", floatPower2(127), "7f000000");
    printf("%10s %10x %10s\n", "128", floatPower2(128), "7f800000");
    return 0;
}