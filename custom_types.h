#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

struct Instance {
    unsigned short id;
    mutable unsigned short totalTimeRun = 0;
    mutable unsigned short partiesServed = 0;
    mutable bool isActive = false;

    explicit Instance(const unsigned short id) : id(id) {
    }
};

#endif //CUSTOM_TYPES_H
