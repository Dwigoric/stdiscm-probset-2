#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

struct Instance {
    unsigned short id;
    unsigned short totalTimeRun = 0;
    unsigned short partiesServed = 0;
    bool isActive = false;

    explicit Instance(const unsigned short id) : id(id) {
    }
};

#endif //CUSTOM_TYPES_H
