#include <ks/graphics.h>
#include <stdlib.h>

KS_API void ks_graphics_draw(int x, int y)
{
    ks_graphics_instance()->klass->draw(x, y);
}

KS_API void ks_graphics_flush()
{
    
}