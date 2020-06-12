#ifndef POLSTATS_H
#define POLSTATS_H

class PolStats
{
public:
    unsigned long bytes_received;
    unsigned long bytes_sent;
};

extern PolStats polstats;

#endif
