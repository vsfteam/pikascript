#include "PikaObj.h"

void _UpdateHandle(PikaObj *self)
{
    // override the handle function here
}

void _beforDinit(PikaObj *self)
{
    /* override in user code */
}

PikaObj *New_TinyObj(Args *args)
{
    PikaObj *self = pikaMalloc(sizeof(PikaObj));
    self->memSize = sizeof(PikaObj);

    /* List */
    self->attributeList = New_args(NULL);

    /* attribute */
    obj_setPtr(self, "_ctx", self);
    obj_setStr(self, "_n", "root");

    /* load */
    if (NULL != args)
    {
        obj_load(self, args, "_n");
        obj_load(self, args, "_ctx");
    }

    /* hard attribute */
    self->name = obj_getStr(self, "_n");
    return self;
}