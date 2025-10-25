//cap.c
#include <string.h>
#include "cap.h"

//s'occupe du changement des stat
void init_cap(Cap *cap, const char *cap_name,
              unsigned char power,
              unsigned char attChg, unsigned char defChg,
              unsigned char evaChg, unsigned char accChg,
              unsigned char targetSelf,
              const char *cap_desc) {
    strncpy(cap->cap_name, cap_name, CAP_NAME_MAX);
    cap->cap_name[CAP_NAME_MAX-1] = '\0';
    cap->power = power;
    cap->attChg = attChg;
    cap->defChg = defChg;
    cap->evaChg = evaChg;
    cap->accChg = accChg;
    cap->targetSelf = targetSelf;
    strncpy(cap->cap_desc, cap_desc, CAP_DESC_MAX);
    cap->cap_desc[CAP_DESC_MAX-1] = '\0';
}

