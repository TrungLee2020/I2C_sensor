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

int main() {
    int LA[] = {1, 3, 5, 7, 8};
    int item = 10, k = 3, n = 5;

    printf("Danh sach phan tu trong mang ban dau:\n");
    for (int i = 0; i < n; i++) {
        printf("LA[%d] = %d \n", i, LA[i]);
    }

    n++; // Tăng kích thước của mảng lên một đơn vị

    // Di chuyển tất cả các phần tử sau vị trí chèn sang phải một vị trí
    for (int j = n - 1; j > k; j--) {
        LA[j] = LA[j - 1];
    }

    // Chèn phần tử mới vào vị trí cần chèn
    LA[k] = item;

    printf("Danh sach phan tu cua mang sau hoat dong chen:\n");
    for (int i = 0; i < n; i++) {
        printf("LA[%d] = %d \n", i, LA[i]);
    }

    return 0;
}

