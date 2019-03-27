#ifndef BMP_H
#define BMP_H

#define PUBLIC
#define PRIVATE
#define PROTECTED
#define STATIC
#define VIRTUAL
#define CONSTRUCTER


#include <Utils/Standard.h>
#include <Engine/IStreamer.h>

class BMP {
public:
    Uint32* Colors = NULL;
    Uint32* Data = NULL;
    Uint32  Width = 0;
    Uint32  Height = 0;
    Uint32  TransparentColorIndex = 0;

    static BMP* Load(const char* filename);
    ~BMP();
};

#endif /* BMP_H */
