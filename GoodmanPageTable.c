/**
* File:   GoodmanPageTable.c
* Simulate shortest-job first processor scheduling algorithms.
*
* Completion Time:  __ hours
*
* @author Goodman
* @version 2020.10.1
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include "PageTable.h"

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
struct page_table_entry{
    int page, frame, order, last, freq;
    unsigned int dirty_valid;
};

struct page_table{
    enum replacement_algorithm mode;
    int page_count, frame_count, *frame, page_faults, verbose, current, counter;
    struct page_table_entry* entries;
};

/////////////////////////////////////////////////////////////////////////////////
/**
 * Creates a new page table object. Returns a pointer to created page table.
 *
 * @param page_count Number of pages.
 * @param frame_count Numbers of frames.
 * @param algorithm Page replacement algorithm
 * @param verbose Enables showing verbose table contents.
 * @return A page table object.
 */
struct page_table* page_table_create(int page_count, int frame_count, enum replacement_algorithm algorithm, int verbose) {
    int i;
    struct page_table* pt = (struct page_table*)malloc(sizeof(struct page_table));
    pt->mode = algorithm;
    pt->page_count = page_count;
    pt->frame_count = frame_count;
    pt->page_faults = 0;
    pt->entries = (struct page_table_entry*)malloc(page_count * sizeof(struct page_table_entry));
    for(i = 0; i < pt->page_count; i++){
        pt->entries[i].dirty_valid = 0; //initialize all entries' dirty & valid/invalid bits to 00
        pt->entries[i].page = i;        //assign page numbers
        pt->entries[i].frame = -1;      //negative value represents an empty frame in entry
        //pt->entries[i].order = 1;
        pt->entries[i].last = -1;
        pt->entries[i].freq = 0;
    }
    pt->frame = (int*)malloc(frame_count * sizeof(int));
    for(i = 0; i < frame_count; i++) {
        pt->frame[i] = 0;              //zero represents free frame
    }
    pt->verbose = verbose;
    pt->current = 1;
    pt->counter = frame_count;
    return pt;
}

/**
 * Destorys an existing page table object. Sets outside variable to NULL.
 *
 * @param pt A page table object.
 */
void page_table_destroy(struct page_table** pt) {
    free((*pt)->entries);
    free((*pt));
    *pt = NULL;
}

/**
 * Simulates an instruction accessing a particular page in the page table.
 *
 * @param pt A page table object.
 * @param page The page being accessed.
 */
void page_table_access_page(struct page_table *pt, int page) {
    if (pt->mode == FIFO) {
        int i, j, k;
        for (i = 0; i < pt->page_count; i++) //search page table for hit
            if (pt->entries[i].page == page) { //hit! no work to do
                if (pt->entries[i].dirty_valid == 1) {
                    pt->entries[i].last = pt->current;
                    pt->entries[i].freq++;
                    pt->current++;
                    return;
                } else {
                    for (j = 0; j < pt->frame_count; j++) { //search for empty frame
                        if (pt->frame[j] == 0) { //empty frame found
                            pt->frame[j] = 1; //mark frame as full
                            pt->entries[i].dirty_valid = 1; //mark entry as valid
                            pt->entries[i].frame = j; //mark frame in entry
                            pt->page_faults++;  //increment page faults
                            pt->entries[i].order = pt->current + 1;
                            pt->entries[i].last = pt->current;
                            pt->entries[i].freq++;
                            pt->current++;
                            return;
                        }
                    }
                    struct page_table_entry* victim;
                    for (k = 0; k < pt->page_count; k++) {
                        victim = &pt->entries[0];
                        if (pt->entries[k].last < pt->entries[k - 1].last && pt->entries[k].page != page)
                            victim = &pt->entries[k];
                    }
                    victim->dirty_valid = 0;
                    pt->entries[i].dirty_valid = 1;
                    pt->entries[i].frame = victim->frame;
                    pt->page_faults++;  //increment page faults
                    pt->entries[i].order = pt->current + 1;
                    pt->entries[i].last = pt->current;
                    pt->entries[i].freq++;
                    pt->current++;
                    return;
                }
            }
    }
}

/**
 * Displays page table replacement algorithm, number of page faults, and the
 * current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display(struct page_table* pt) {
    enum replacement_algorithm m;
    char* mode;
    if(pt->mode == FIFO)
        mode = "FIFO";
    else if(pt->mode == LRU)
        mode = "LRU";
    else mode = "MFU";

    printf("====Page Table====\n");
    printf("Mode: %s\n", mode);
    printf("Page Faults: %d\n", pt->page_faults);
    page_table_display_contents(pt);
}

/**
 * Displays the current contents of the page table.
 *
 * @param pt A page table object.
 */
void page_table_display_contents(struct page_table *pt) {
    int i, dirty, valid;
    for(i = 0; i < pt->page_count; i++) {
        if (pt->entries[i].dirty_valid == 0) {
            dirty = 0;
            valid = 0;
        }
        else if (pt->entries[i].dirty_valid == 1) {
            dirty = 0;
            valid = 1;
        }
        else if (pt->entries[i].dirty_valid == 2) {
            dirty = 1;
            valid = 0;
        } else {
            dirty = 1;
            valid = 1;
        }
    }
    if(pt->verbose == 0) {
        printf("page frame | dirty valid\n");
        for(i = 0; i < pt->page_count; i++)
            printf("   %d     %d |     %d     %d\n", i, pt->entries[i].frame, dirty, valid);
        printf("\n");
    }
    else{
        printf("page frame | dirty valid | order  last freq\n");
        for(i = 0; i < pt->page_count; i++)
            printf("   %d     %d |     %d     %d |     %d     %d    %d\n", i, pt->entries[i].frame, dirty, valid, pt->entries[i].order, pt->entries[i].last, pt->entries[i].freq);
        printf("\n");
    }
}