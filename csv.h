#include <stdio.h>

char* title(
    FILE *file,
    int *count_column
);

int read_field(
    FILE *file,
    char **mass_tec_sim,
    int *capacity_mass_tec_sim,
    int *ending
);