namespace Collision
{
    enum
    {
        IGNORE0 = true,
    };
    enum class Flags : unsigned char
    {
        Ignore = 0,
        Wallride = 0x4,
        Vert = 0x8,
        Hollow = 0x10,
        Trigger = 0x40,
        Skatable = 0x80,
    };
};