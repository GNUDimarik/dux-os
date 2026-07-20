#include <stdint.h>

namespace
{

constexpr uintptr_t kVgaBufferAddress = 0xB8000;
constexpr uint16_t kVgaWidth = 80;
constexpr uint8_t kDefaultColor = 0x0F;

volatile uint16_t *const kVgaBuffer =
    reinterpret_cast<volatile uint16_t *>(kVgaBufferAddress);

constexpr uint16_t make_vga_entry(char character, uint8_t color)
{
    return static_cast<uint16_t>(
        static_cast<uint8_t>(character)
            | static_cast<uint16_t>(color) << 8
    );
}

void print(const char *string)
{
    uint16_t position = 0;

    while (*string != '\0') {
        kVgaBuffer[position++] = make_vga_entry(*string++, kDefaultColor);
    }
}

} // namespace

extern "C" [[noreturn]]
void main(uint32_t multiboot_magic, uint32_t multiboot_info)
{
    static_cast<void>(multiboot_magic);
    static_cast<void>(multiboot_info);

    print("DUX kernel has been loaded by GRUB2");

    for (;;) {}
}