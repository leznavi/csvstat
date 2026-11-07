#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "typeNUM.h"
#include "csv.h"
#include "analysis.h"
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "input error\n");
        return 2;
    }

    else if (strcmp(argv[1], "--help") == 0) {
        if (argc != 2) {
            fprintf(stderr, "input error\n");
            return 2;
        }
        else {
            printf("schema - show columns names and types\n");
            printf("stats - show columns statistics\n");
            return 0;
        }
    }

    if (argc != 3) {
        fprintf(stderr, "expected mode and file name\n");
        return 2;
    }

    if (strcmp(argv[1],"schema") != 0 && strcmp(argv[1], "stats") != 0) {
        fprintf(stderr, "unknown mode\n");
        return 2;
    }

    FILE *file =
        fopen(argv[2], "r");
    
    if (file == NULL) {
        fprintf(stderr, "FILE error\n");
        return 1;
    }

    char *mass_names_columns;
    int count_column = 1;

    mass_names_columns = title(file, &count_column); // названия столбцов

    if (mass_names_columns == NULL) {
        fprintf(stderr, "title error\n");
        fclose(file);
        
        free(mass_names_columns);
        return 1;
    }

    struct Column *columns = malloc(count_column * sizeof(*columns)); // структура
    int ind_column = 0;
    int ind_mass_columns = 0;

    if (columns == NULL) { //удалось ли выделение памяти
        fprintf(stderr, "memory error\n");
        fclose(file);
        
        free(mass_names_columns);
        return 1;
    }

    while (ind_column < count_column) { // заполнение name + присваивание изначального 0 для составляющих структуры
        columns[ind_column].name = &mass_names_columns[ind_mass_columns];

        columns[ind_column].type = 0;
        columns[ind_column].count_values = 0;

        columns[ind_column].count_poz_values = 0;
        columns[ind_column].count_neg_values = 0;

        columns[ind_column].count_even_values = 0;
        columns[ind_column].count_int_in_doub = 0;

        columns[ind_column].sum_ost_results = 0;
        columns[ind_column].sum_int_results = 0;
        columns[ind_column].average_value_doub = 0;

        while (mass_names_columns[ind_mass_columns] != '\0') {
            ind_mass_columns += 1;
        }

        ind_mass_columns += 1;
        ind_column += 1;
    }

    int capacity_mass_tec_sim = 2;
    char *mass_tec_sim = malloc(
        capacity_mass_tec_sim * sizeof(*mass_tec_sim)
    );
    

    if (mass_tec_sim == NULL) { //удалось ли выделение памяти
        fprintf(stderr, "memory error\n");
        fclose(file);
        free(mass_names_columns);
        free(columns);
        return 1;
    }

    ind_column = 0;
    int count_line = 0;
    int ending;

    int field = read_field(file, &mass_tec_sim, &capacity_mass_tec_sim, &ending);

    while (field == 1) {

        if (ending == ',') { // новый столбец
            columns[ind_column].type = update_type(columns[ind_column].type, mass_tec_sim);

            ind_column += 1;
        }

        else if (ending == '\n') { // новыя строчка
            columns[ind_column].type = update_type(columns[ind_column].type, mass_tec_sim);

            ind_column = 0;
            count_line += 1;
        }

        else if (ending == EOF) {
            columns[ind_column].type = update_type(columns[ind_column].type, mass_tec_sim);

            count_line += 1;
        }

        field = read_field(file, &mass_tec_sim, &capacity_mass_tec_sim, &ending);
    }

    if (field == -1) {
        fprintf(stderr, "read field error");
        fclose(file);

        free(mass_tec_sim);
        free(mass_names_columns);
        free(columns);
        return 1;
    }

    rewind(file); // возвращение чтения файла в начало

    int tec_sim = fgetc(file);

    while (tec_sim != '\n' && tec_sim != EOF) { //пропуск первой строки
        tec_sim = fgetc(file);
    }

    if (ferror(file) != 0) {
        fprintf(stderr, "read file error");
        fclose(file);

        free(mass_tec_sim);
        free(mass_names_columns);
        free(columns);
        return 1;
    }

    ind_column = 0;
    field = read_field(file, &mass_tec_sim, &capacity_mass_tec_sim, &ending);
    
    while (field == 1) {

        if (columns[ind_column].type == 0) {
            long long value_int;
            
            if (is_integer(mass_tec_sim, &value_int)) {
                maxmin_int(&columns[ind_column], value_int);
                posneg_int(&columns[ind_column], value_int);
                even_int(&columns[ind_column], value_int);
                
                columns[ind_column].sum_ost_results += value_int % count_line;
                columns[ind_column].sum_int_results += value_int / count_line;
            }
        }

        if (columns[ind_column].type == 1) {
            double value_doub = strtod(mass_tec_sim, NULL);

            count_int_in_double(&columns[ind_column], value_doub);
            maxmin_doub(&columns[ind_column], value_doub);
            posneg_doub(&columns[ind_column], value_doub);
            average_value_doub(&columns[ind_column], value_doub);
        }

        if (ending == ',') { // новый столбец
            ind_column += 1;
        }

        else if (ending == '\n') { // новыя строчкa
            ind_column = 0;
        }

        field = read_field(file, &mass_tec_sim, &capacity_mass_tec_sim, &ending);
    }

    if (field == -1) {
        fprintf(stderr, "read field error");
        fclose(file);

        free(mass_tec_sim);
        free(mass_names_columns);
        free(columns);
        return 1;
    }

    printf("rows %d\n", count_line);
    printf("columns %d\n", count_column); 

    for (int ind = 0; ind < count_column; ind += 1) {

        if (strcmp(argv[1],"schema") == 0) {

            if (columns[ind].type == 0) {
            printf("%d \"%s\" INT\n", ind, columns[ind].name);
            }

            if (columns[ind].type == 1) {
            printf("%d \"%s\" DOUBLE\n", ind, columns[ind].name);
            }
        }

        if (strcmp(argv[1],"stats") == 0){

            if (columns[ind].type == 0) {
                average_value_int(&columns[ind], count_line);

                printf("column %d\n", ind);
                printf("name \"%s\"\n", columns[ind].name);
                printf("type INT\n");
                printf("nulls 0\n");

                printf("min %lld\n", columns[ind].min_int);
                printf("max %lld\n", columns[ind].max_int);

                printf("mean %.6f\n", columns[ind].average_value_int);

                printf("positive %d\n", columns[ind].count_poz_values);
                printf("negative %d\n", columns[ind].count_neg_values);
                
                printf("even %d\n", columns[ind].count_even_values);
            }

            if (columns[ind].type == 1) {

                printf("column %d\n", ind);
                printf("name \"%s\"\n", columns[ind].name);
                printf("type DOUBLE\n");
                printf("nulls 0\n");

                if (columns[ind].min_doub == 0.0) {
                    columns[ind].min_doub = 0.0;
                }

                printf("min %.6f\n", columns[ind].min_doub);

                if (columns[ind].max_doub == 0.0) {
                    columns[ind].max_doub = 0.0;
                }

                printf("max %.6f\n", columns[ind].max_doub);


                if (columns[ind].average_value_doub == 0.0) {
                    columns[ind].average_value_doub = 0.0;
                }

                printf("mean %.6f\n", columns[ind].average_value_doub);

                printf("positive %d\n", columns[ind].count_poz_values);
                printf("negative %d\n", columns[ind].count_neg_values);
                
                printf("integer %d\n", columns[ind].count_int_in_doub);
            }
            
            if (ind + 1 < count_column){
                printf("\n");
            }
        }

    }

    free(mass_names_columns);
    free(mass_tec_sim);
    free(columns);

    fclose(file);
    return 0;
}