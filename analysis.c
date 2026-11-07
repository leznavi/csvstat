#include <stdio.h>
#include "analysis.h"
#include <math.h>

void maxmin_int(
    struct Column *column,
    long long value_int
) {

    if (column->count_values == 0) {
        column->max_int = value_int;
        column->min_int = value_int;
    }

    else {
        if (value_int > column->max_int) {
            column->max_int = value_int;
        }

        else if (value_int < column->min_int) {
            column->min_int = value_int;
        }
    }

    column->count_values += 1;
}

void maxmin_doub(
    struct Column *column,
    double value_doub
) {

    if (column->count_values == 0) {
        column->max_doub = value_doub;
        column->min_doub = value_doub;
    }

    else {
        if (value_doub > column->max_doub) {
            column->max_doub = value_doub;
        }

        else if (value_doub < column->min_doub) {
            column->min_doub = value_doub;
        }
    }

    column->count_values += 1;
}

void posneg_int(
    struct Column *column,
    long long value_int
) {
    if (value_int > 0) {
        column->count_poz_values += 1;
    }

    else if (value_int < 0) {
        column->count_neg_values += 1;
    }
}

void posneg_doub(
    struct Column *column,
    double value_doub
) {
    if (value_doub > 0) {
        column->count_poz_values += 1;
    }

    else if (value_doub < 0) {
        column->count_neg_values += 1;
    }
}

void even_int(
    struct Column *column,
    long long value_int
) {
    if (value_int % 2 == 0) {
        column->count_even_values += 1;
    }
}

void count_int_in_double(
    struct Column *column,
    double value_doub
) {
    if (value_doub == floor(value_doub)) {
        column->count_int_in_doub += 1;
    }
  
}

void average_value_int(
    struct Column *column,
    long long count_line
) {
    column->average_value_int = column->sum_int_results 
    + (double)column->sum_ost_results / count_line;
}

void average_value_doub(
    struct Column *column,
    double value_doub
) {
    double old_average_value = column->average_value_doub;
    int count_values = column->count_values;

    if (count_values == 1) {
        column->average_value_doub = value_doub;
    }

    else if ((old_average_value>= 0 && value_doub >= 0)
        || (old_average_value <= 0 && value_doub <= 0)) {
            column->average_value_doub = 
            old_average_value + (value_doub - old_average_value) / count_values;
        }
    else {
        column->average_value_doub = 
        old_average_value * ((count_values - 1.0) / count_values) + value_doub / count_values;
    }
}