#include "BaseObj.h"
#include "dataStrs.h"

void PikaStdLib_SysObj_remove(PikaObj* self, char* argPath) {
    obj_setErrorCode(self, 0);
    int32_t res = obj_removeArg(self, argPath);
    if (1 == res) {
        obj_setSysOut(self, "[error] del: object no found.");
        obj_setErrorCode(self, 1);
        return;
    }
    if (2 == res) {
        obj_setSysOut(self, "[error] del: arg not match.");
        obj_setErrorCode(self, 2);
        return;
    }
}

void PikaStdLib_SysObj_type(PikaObj* self, Arg* arg) {
    if (NULL == arg) {
        obj_setSysOut(self, "[error] type: arg no found.");
        obj_setErrorCode(self, 1);
        return;
    }
    ArgType type = arg_getType(arg);
    if (TYPE_INT == type) {
        obj_setSysOut(self, "int");
        return;
    }
    if (TYPE_FLOAT == type) {
        obj_setSysOut(self, "float");
        return;
    }
    if (TYPE_STRING == type) {
        obj_setSysOut(self, "string");
        return;
    }
    if (TYPE_POINTER == type) {
        obj_setSysOut(self, "pointer");
        return;
    }
    if (TYPE_MATE_OBJECT == type) {
        obj_setSysOut(self, "mate_object");
        return;
    }
    if (TYPE_METHOD == type) {
        obj_setSysOut(self, "method");
        return;
    }
}

float PikaStdLib_SysObj_float(PikaObj* self, Arg* arg) {
    ArgType type = arg_getType(arg);
    if (TYPE_INT == type) {
        return (float)arg_getInt(arg);
    }
    if (TYPE_FLOAT == type) {
        return (float)arg_getFloat(arg);
    }
    obj_setSysOut(self, "[error] convert to float type faild.");
    obj_setErrorCode(self, 1);
    return -99999.99999;
}

int PikaStdLib_SysObj_int(PikaObj* self, Arg* arg) {
    ArgType type = arg_getType(arg);
    if (TYPE_INT == type) {
        return (int)arg_getInt(arg);
    }
    if (TYPE_FLOAT == type) {
        return (int)arg_getFloat(arg);
    }
    obj_setSysOut(self, "[error] convert to int type faild.");
    obj_setErrorCode(self, 1);
    return -999999999;
}