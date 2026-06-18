
#include "cpu.h"


int main(int argc, char *argv[])
{
    if (run_cpu(argc, argv) != 0) {
        return -1;
    }

    return 0;
}
