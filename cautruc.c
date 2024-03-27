#include <stdio.h>
#include <string.h>
struct cautruc
{
    char x1;
    int x2;/* data */
};

union Data {
    int i;
    float f;
    char str[20];
};

int main()
{
    struct cautruc ct;
    ct.x1 = 10;
    ct.x2 = 20;
    union Data data;
    data.i = 40;
    data.f = 3.14;
    strcpy(data.str, "abcd");
    
    // In ra giá trị các thành phần của biến struct
    printf("Coordinates of ct: (%d, %d)\n", ct.x1, ct.x2);
    printf("Size of struct cautruc: %zu bytes\n", sizeof(struct cautruc));

    printf("Coordinates of data: (%d, %f, %s)\n", data.i, data.f, data.str);
    printf("Size of struct data: %zu bytes\n", sizeof(union Data));
    printf("Address of data.i: %p\n", &data.i);
    printf("Address of data.f: %p\n", &data.f);
    printf("Address of data.str: %p\n", &data.str);
    return 0;
}
