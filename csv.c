#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "csv.h"

char* title(
    FILE *file,
    int *count_column
) {

    int tec_sim = fgetc(file);
    int capacity_names_columns = 2;
    char *mass_names_columns = malloc(
        capacity_names_columns * sizeof(*mass_names_columns)
    );
    int ind_mass_names_columns = 0;

    if (mass_names_columns == NULL) { //удалось ли выделение памяти
        free(mass_names_columns);
        return NULL;
    }

    while (tec_sim != EOF) {  //цикл заголовка

        if (ind_mass_names_columns + 1 >= capacity_names_columns) { //увелич динам масс
            capacity_names_columns *= 2;

            char *temp = realloc(
                mass_names_columns, 
                capacity_names_columns * sizeof(*mass_names_columns)
            );

            if (temp == NULL) { //удалось ли выделение памяти
                free(mass_names_columns);
                return NULL;
            }

            else {
                mass_names_columns = temp;
            }
        } 

        if (tec_sim == '\n') {
            break;
        }

        if (tec_sim == ',') {
            *count_column += 1;

            mass_names_columns[ind_mass_names_columns] = '\0';
        }
        
        else {
            mass_names_columns[ind_mass_names_columns] = tec_sim;
        }

        ind_mass_names_columns+=1;

        tec_sim = fgetc(file);
    }
    
    if (ferror(file) != 0) {
        free(mass_names_columns);
        return NULL;
    }

    mass_names_columns[ind_mass_names_columns] = '\0';
    ind_mass_names_columns += 1;

    return mass_names_columns;
}


int read_field(
    FILE *file,
    char **mass_tec_sim,
    int *capacity_mass_tec_sim,
    int *ending
) {
    int tec_sim = fgetc(file);
    int ind_mass_tec_sim = 0;
    
    while (tec_sim != EOF) {

        if (ind_mass_tec_sim + 1 >= *capacity_mass_tec_sim) { //увелич динам масс
            *capacity_mass_tec_sim *= 2;

            char *temp = realloc(
                *mass_tec_sim, 
                *capacity_mass_tec_sim * sizeof(char)
            );

            if (temp == NULL) { //удалось ли выделение памяти
                fprintf(stderr, "memory error");

                return -1;
            }

            else {
                *mass_tec_sim = temp;
            }
        }
        
        if (tec_sim == ',') { // новый столбец
            (*mass_tec_sim)[ind_mass_tec_sim] = '\0';
            
            *ending = ',';
            ind_mass_tec_sim = 0;

            return 1;
        }

        else if (tec_sim == '\n') { // новыя строчка
            (*mass_tec_sim)[ind_mass_tec_sim] = '\0';

            *ending = '\n';
            ind_mass_tec_sim = 0;

            return 1;
        }

        else {
            (*mass_tec_sim)[ind_mass_tec_sim] = tec_sim;
            ind_mass_tec_sim += 1;
        }

        tec_sim = fgetc(file);
    }

    int read_error = ferror(file);

    if (read_error != 0) {
        fprintf(stderr, "read tec_sim in field error\n");

        return -1;
    }

    if (tec_sim == EOF && ind_mass_tec_sim > 0) {
        (*mass_tec_sim)[ind_mass_tec_sim] = '\0';

        *ending = EOF;
        return 1;
    }

    return 0;
}