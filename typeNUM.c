#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "typeNUM.h"

int is_integer( // проверка на integer
    char mass_tec_sim[],
    long long *value
) {
    char *ind_end;
    errno = 0;
    *value = strtoll(mass_tec_sim, &ind_end, 10);

    if (ind_end != mass_tec_sim && *ind_end == '\0' && errno != ERANGE)  { //проверка на целое число
        return 1;
    }

    else {
        return 0;
    }
}

int is_double( // проверка на double
    char mass_tec_sim[],
    double *value
) {
    char *ind_end;
    errno = 0;
    int ind_tec_sim = 0;

    if (mass_tec_sim[ind_tec_sim] == '+' || mass_tec_sim[ind_tec_sim] == '-') {
        ind_tec_sim += 1;
    }
    int save_ind = ind_tec_sim;

    while (mass_tec_sim[ind_tec_sim] <= '9' && mass_tec_sim[ind_tec_sim] >= '0' ) {
        ind_tec_sim += 1;
    }

    if (ind_tec_sim != save_ind && mass_tec_sim[ind_tec_sim] == '.') {
        ind_tec_sim += 1;
    }
    
    else {
        return 0;
    }
    
    save_ind = ind_tec_sim;

    while (mass_tec_sim[ind_tec_sim] <= '9' && mass_tec_sim[ind_tec_sim] >= '0' ) {
        ind_tec_sim += 1;
    }

    if (ind_tec_sim != save_ind && mass_tec_sim[ind_tec_sim] == '\0') {

    }
    
    else {
        return 0;
    }

    *value = strtod(mass_tec_sim, &ind_end);

    if (ind_end != mass_tec_sim && *ind_end == '\0' && errno != ERANGE) {
        return 1;
    }

    else {
        return 0;
    }
}

int update_type( // присваиваем строке тип
    int previous_type,
    char *value
    ) {

    long long value_int;
    double value_doub;

    if (is_integer(value, &value_int) == 1)  {

    }

    else if (is_double(value, &value_doub) == 1) {

        if (previous_type == 0) {
            return 1;
    }

    }

    else {
        return 2;
    }

    return previous_type;
}