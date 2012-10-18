#include <ks/sys/system.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <stdlib.h>
#include <string.h>
#include <ks/log.h>
#include "ks/sys/system.h"
#include "ks/sys/graphics.h"
#include "ks/sys/eventq.h"
#include "ks/constants.h"
#include "ks/director.h"

KS_API void ks_system_flush()
{
    ks_system_instance()->klass->flush();
}

KS_API void ks_system_update_message()
{
    ks_system_instance()->klass->update_messages();
}