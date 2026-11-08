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

void print_summary(
    struct Column *columns,
    int count_column
) {
    int count_int_columns = 0;
    int count_double_columns = 0;
    int count_other_columns = 0;

    long long count_numeric_values = 0;
    long long count_positive = 0;
    long long count_negative = 0;

    for (int ind = 0; ind < count_column; ind += 1) {
        if (columns[ind].type == 0) {
            count_int_columns += 1;
        }
        else if (columns[ind].type == 1) {
            count_double_columns += 1;
        }
        else {
            count_other_columns += 1;
        }

        if (columns[ind].type == 0 || columns[ind].type == 1) {
            count_numeric_values += columns[ind].count_values;
            count_positive += columns[ind].count_poz_values;
            count_negative += columns[ind].count_neg_values;
        }
    }

    long long count_zero = count_numeric_values
        - count_positive - count_negative;

    printf("int_columns %d\n", count_int_columns);
    printf("double_columns %d\n", count_double_columns);
    printf("other_columns %d\n", count_other_columns);
    printf("numeric_values %lld\n", count_numeric_values);
    printf("positive %lld\n", count_positive);
    printf("negative %lld\n", count_negative);
    printf("zeros %lld\n", count_zero);
}