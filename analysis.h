struct Column {
    char *name;
    int type; // type = 0 -> int | type = 1 -> double

    long long max_int;
    long long min_int;
    double max_doub;
    double min_doub;

    int count_values;
    int count_poz_values;
    int count_neg_values;

    int count_even_values;
    int count_int_in_doub;

    long long sum_int_results;
    long long sum_ost_results;

    double average_value_int;
    double average_value_doub;
};

void maxmin_int(
    struct Column *columns,
    long long value_int
);

void maxmin_doub(
    struct Column *column,
    double value_doub
);

void posneg_int(
    struct Column *columns,
    long long value_int
);

void posneg_doub(
    struct Column *column,
    double value_doub
);

void even_int(
    struct Column *column,
    long long value_int
);

void count_int_in_double(
    struct Column *column,
    double value_doub
);

void average_value_int(
    struct Column *column,
    long long count_line
);

void average_value_doub(
    struct Column *column,
    double value_doub
);