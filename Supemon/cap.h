//cap.h
#ifndef CAP_H
#define CAP_H

#define CAP_NAME_MAX 20
#define CAP_DESC_MAX 50

typedef struct {
    char cap_name[CAP_NAME_MAX];
    unsigned char power;        //degat de base, 0 si non offensif
    unsigned char attChg;
    unsigned char defChg;
    unsigned char evaChg;
    unsigned char accChg;
    unsigned char targetSelf; //1 si moi, 0 si PNJ
    char cap_desc[CAP_DESC_MAX];
} Cap;


void init_cap(Cap *cap, const char *cap_name,
              unsigned char power,
              unsigned char attChg, unsigned char defChg,
              unsigned char evaChg, unsigned char accChg,
              unsigned char targetSelf,
              const char *cap_desc
              );

#endif // CAP_H
