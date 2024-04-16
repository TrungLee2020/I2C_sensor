# I2C_sensor
camera hiview and TOF sensor

int height = (buffer[0] << 8) | buffer[1]; có nghĩa là lấy 2 byte từ mảng buffer và kết hợp chúng để tạo thành một số nguyên 16-bit

CONFIG_PARTITION_TABLE_CUSTOM=y
CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="partitions.csv"
CONFIG_PARTITION_TABLE_FILENAME="partitions.csv"
CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y

# Macro C++
## ARRAYSIZE(a):
Macro này tính kích thước của một mảng a bằng cách chia kích thước của mảng a cho kích thước của một phần tử trong mảng đó.
sizeof(a): Trả về kích thước của mảng a trong byte.
sizeof(*(a)): Trả về kích thước của một phần tử trong mảng a trong byte.
Do đó, ARRAYSIZE(a) sẽ trả về số lượng phần tử trong mảng a.
## ALIGN(type):
Macro này sử dụng attribute __attribute__((aligned( __alignof__( type ) ))) để chỉ định rằng biến hoặc cấu trúc được khai báo với kiểu type phải được căn chỉnh với biên của kiểu đó.
__alignof__(type): Trả về độ căn chỉnh của kiểu dữ liệu type.
__attribute__((aligned(__alignof__(type)))): Cú pháp attribute để chỉ định độ căn chỉnh.
## PACK(type):
Macro này sử dụng attribute __attribute__((aligned(__alignof__(type)), packed)) để chỉ định rằng cấu trúc có kiểu type được đóng gói mà không có các byte đệm giữa các thành phần của cấu trúc đó.
__attribute__((aligned(__alignof__(type)), packed)): Cú pháp attribute để chỉ định độ căn chỉnh và đóng gói.
## PACK8:
Tương tự như PACK(type), nhưng đặc biệt là nó sử dụng kiểu dữ liệu uint8_t và độ căn chỉnh của uint8_t.

parrtition.csv
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,      0x09000,  0x004000,
otadata,  data, ota,      0x0d000,  0x002000,
phy_init, data, phy,      0x0f000,  0x001000,
factory,  app,  factory,  0x010000, 0x150000,
ota_0,    app,  ota_0,    0x160000, 0x140000,
ota_1,    app,  ota_1,    0x2A0000, 0x140000,
coredump, data, coredump, 0x3E0000, 128k