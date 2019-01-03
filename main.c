#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define DEBUG

#if defined(DEBUG)
#define debug printf
#else
#define debug
#endif

int main(int argc, char* argv[])
{
    unsigned int input_rgb;
    unsigned char input_r, input_g, input_b;
    unsigned char output_y, output_u, output_v;
    float r, g, b;
    float y, u, v;

    /* 打印函数使用方法 */
    if(argc !=  2)
    {
        printf("usage: ./app <RGB888 color>\n");
        printf("eg   : ./app 0xFF0000\n");
        printf("\n");
        return -1;
    }

    if(strlen(argv[1]) != 8)
    {
        printf("please enter Full color value\n");
        printf("  eg1: 0xFFFFFF\n");
        printf("  eg2: 0xFF0000\n");
        printf("  eg3: 0x123456\n\n");
        printf("The following format is incorrect\n");
        printf("  err fmt1: 0xFF00\n");
        printf("  err fmt2: FF0000\n");
        printf("  err fmt2: 0xxFFFFF\n\n");
        printf("\n");
        return -1;
    }

    input_rgb = (unsigned int)strtol(argv[1], NULL, 0);
    debug("input_rgb = 0x%x\n", input_rgb);

    input_r = input_rgb >> 16;
    input_g = input_rgb >> 8;
    input_b = input_rgb;
    debug("input_r   = 0x%x(h), %d(d)\n", input_r, input_r);
    debug("input_g   = 0x%x(h), %d(d)\n", input_g, input_g);
    debug("input_b   = 0x%x(h), %d(d)\n", input_b, input_b);

    r = (float)input_r;
    g = (float)input_g;
    b = (float)input_b;
    debug("r         = %f\n", r);
    debug("g         = %f\n", g);
    debug("b         = %f\n", b);

#if 1
    /* CCIR 601 */
    /* RGB和YUV的范围都是[0,255] */
    y =  0.299 * r + 0.587 * g + 0.114 * b;
    u =  0.500 * r - 0.419 * g - 0.081 * b + 128;
    v = -0.169 * r - 0.331 * g + 0.500 * b + 128;
#else
    /* ISBN 1-878707-09-4 */
    /* RGB的范围是[0,255], Y的范围是[16,235], UV的范围是[16,239] */
    y =  0.257 * r + 0.504 * g + 0.098 * b + 16;
    u =  0.439 * r - 0.368 * g - 0.071 * b + 128;
    v = -0.148 * r - 0.291 * g + 0.439 * b + 128;
#endif

    debug("y = %f\n", y);
    debug("u = %f\n", u);
    debug("v = %f\n", v);

    /* float 类型强转为整形，做四舍五入转化的话，原本是要加 0.5 的 */
    /* 但是由于 FF 的颜色值，计算到的 YUV 分量的值为 255.5, 再加 0.5 后数据会溢出 */
    /* 因此这里迫不得已加 0.4999 将就一下 */
    output_y = (unsigned char)(y + 0.4999);
    output_u = (unsigned char)(u + 0.4999);
    output_v = (unsigned char)(v + 0.4999);
    printf("y = 0x%x(h), %d(d), %f(f)\n", output_y, output_y, y);
    printf("u = 0x%x(h), %d(d), %f(f)\n", output_u, output_u, u);
    printf("v = 0x%x(h), %d(d), %f(f)\n", output_v, output_v, v);

    printf("\n");
    return 0;
}
