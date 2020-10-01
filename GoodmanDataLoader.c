/**
* File:   GoodmanDataLoader.c
* Simulate shortest-job first processor scheduling algorithms.
*
* Completion runningTime:  __ hours
*
* @author Goodman
* @version 2020.10.1
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include "DataLoader.h"

/////////////////////////////////////////////////////////////////////////////////
/**
 * Loads a test_scenario struct from a textfile.
 *
 * @param filename The name of the file to load.
 * @return A struct containing the loaded file.
 */
struct test_scenario* load_test_data(char* filename){
    int i;
    struct test_scenario* data = (struct test_scenario*)malloc(sizeof(struct test_scenario));
    FILE* file = fopen(filename, "r");
    fscanf(file, "%d", &data->page_count);
    fscanf(file, "%d", &data->frame_count);
    fscanf(file, "%d", &data->refstr_len);
    for(i = 0; i < data->refstr_len; i++)
        fscanf(file, "%d", &data->refstr[i]);
    fclose(file);
    return data;
}