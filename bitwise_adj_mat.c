#include <stdio.h> /* puts */
#include <stdlib.h> /* calloc */
#include <stdbool.h> /* bool */

#include "bitwise_adj_mat.h"

/* return pointer to cell in cells at [col][row]
 * returns 0 on error
 */
static unsigned char * bam_access_cell(uint8_t *cells, unsigned int n_cols, unsigned int n_rows, unsigned int col, unsigned int row){
    unsigned int index = 0;

    if( ! cells ){
        puts("bam_access_cell: cells was null");
        return 0;
    }

    if( col >= n_cols ){
        puts("bam_access_cell: provided column was greater than n_cells");
        return 0;
    }

    if( row >= n_rows ){
        puts("bam_access_cell: provided row was greater than n_cells");
        return 0;
    }

    /* index
     * we only use this in bam_resize for copying from the old cells to the new
     */
    index = row * n_cols + col;

    if( index >= (n_cols * n_rows) ){
        puts("bam_access_cell: illegal index");
        return 0;
    }

    return &(cells[index]);
}

/* set edge representing from -> to to `value`
 * `value` must be 0 or 1
 *
 * returns 1 on success
 * returns 0 on error
 */
static unsigned int bam_set_edge(struct bitwise_adj_mat *bam, unsigned int col, unsigned int row, unsigned int value){
    unsigned int index = 0;
    uint8_t mask = 0;

    if( ! bam ){
        puts("bam_set_edge: cells was null");
        return 0;
    }

    if( col >= bam->n_rows ){
        puts("bam_set_edge: provided column was greater than n_cells");
        return 0;
    }

    if( row >= bam->n_rows ){
        puts("bam_set_edge: provided row was greater than n_cells");
        return 0;
    }

    /* index */
    index = row * bam->n_cols + (col / 8);

    if( index >= (bam->n_cols * bam->n_rows) ){
        printf("bam_set_edge: illegal index requested: '%d'\n for requested col '%d' and row '%d'\n n_cols '%d', n_rows '%d', n_cols * n_rows '%d'\n\n",
                index,
                col,
                row,
                bam->n_cols,
                bam->n_rows,
                bam->n_cols * bam->n_rows
                );
        return 0;
    }

    if( value > 1 ){
        /* flatten value down to 1 */
        value = 1;
    }

    if( value ){
        /* make a mask of all 0s with a 1 in the position we want to set */
        mask = 1 << (col % 8);
        /* set that position */
        bam->cells[index] |= mask;
    } else {
        /* make a mask of all 1s with a 0 in the position we want to clear */
        mask = 0xFF ^ (1 << col % 8);
        /* clear that position */
        bam->cells[index] &= mask;
    }

    return 1;
}

/* set value stored on edge representing from -> to
 *
 * returns value on success (which may be 1 or 0)
 * returns 0 on error
 */
static unsigned int bam_get_edge(struct bitwise_adj_mat *bam, unsigned int col, unsigned int row){
    unsigned int index = 0;
    uint8_t mask = 0;
    unsigned int res = 0;

    if( ! bam ){
        puts("bam_get_edge: cells was null");
        return 0;
    }

    if( col >= bam->n_rows ){
        puts("bam_get_edge: provided column was greater than n_cells");
        return 0;
    }

    if( row >= bam->n_rows ){
        puts("bam_get_edge: provided row was greater than n_cells");
        return 0;
    }

    /* index */
    index = row * bam->n_cols + (col / 8);

    if( index >= (bam->n_cols * bam->n_rows) ){
        printf("bam_get_edge: illegal index requested: '%d'\n for requested col '%d' and row '%d'\n n_cols '%d', n_rows '%d', n_cols * n_rows '%d'\n\n",
                index,
                col,
                row,
                bam->n_cols,
                bam->n_rows,
                bam->n_cols * bam->n_rows
                );
        return 0;
    }

    /* make mask of all 0s with a 1 in the position we want */
    mask = 1 << (col % 8);
    /* get bit we want */
    res = bam->cells[index] & mask;

    /* returns true iff res is non-zero */
    return res != 0;
}

/* allocate and initialise a new adj. matrix containing `num_nodes` nodes
 * `num_nodes` may be 0
 *
 * returns * on success
 * returns 0 on error
 */
struct bitwise_adj_mat * bam_new(unsigned int num_nodes){
    struct bitwise_adj_mat *mat = 0;

    mat = calloc(1, sizeof(struct bitwise_adj_mat));
    if( ! mat ){
        puts("bam_new: call to calloc failed");
        return 0;
    }

    if( ! bam_init(mat, num_nodes) ){
        puts("bam_new: call to bam_init failed");
        return 0;
    }

    return mat;
}

/* initialise an existing adj. matrix containing `num_nodes` nodes
 * `num_nodes` may be 0
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_init(struct bitwise_adj_mat *bam, unsigned int num_nodes){
    if( ! bam ){
        puts("bam_init: bam was null");
        return 0;
    }

    /* initialise to 0 */
    bam->n_cols = 0;
    bam->n_rows = 0;
    bam->cells = 0;

    /* only call bam_resize if we have a `num_nodes` > 0 */
    if( num_nodes ){
        if( ! bam_resize(bam, num_nodes) ){
            puts("bam_init: call to bam_resize failed");
            return 0;
        }
    }

    return 1;
}

/* destroy an existing adj. matrix
 * will call free on `bam` if `free_bame` is truethy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_destroy(struct bitwise_adj_mat *bam, unsigned int free_bam){
    if( ! bam ){
        puts("bam_destroy: bam was null");
        return 0;
    }

    /* always free cells if defined */
    if( bam->cells ){
        free(bam->cells);
        bam->cells = 0;
    }

    bam->n_cols = 0;
    bam->n_rows = 0;

    /* free bam if asked nicely */
    if( free_bam ){
        free(bam);
    }

    return 1;
}

/* resize an existing adj. matrix to include enough space for
 * the number of nodes specified by `num_nodes`
 * `num_nodes` must be greater than 0
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_resize(struct bitwise_adj_mat *bam, unsigned int num_nodes){
    uint8_t *new_cells = 0;
    unsigned int num_rows = 0;
    unsigned int num_cols = 0;
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned char *from = 0;
    unsigned char *to = 0;

    if( ! bam ){
        puts("bam_resize: bam was null");
        return 0;
    }

    if( ! num_nodes ){
        puts("bam_resize: num_nodes must be greater than 0");
        return 0;
    }

    /* num_rows is always exactly the number of nodes */
    num_rows = num_nodes;

    /* in each cell we can store 8 edges, so we only need
     * an 1/8th of the number of nodes
     */
    num_cols = (num_nodes + 7) / 8;

    /* allocate new matrix */
    new_cells = calloc(num_cols * num_rows, sizeof(uint8_t));
    if( ! new_cells ){
        puts("bam_resize: call to calloc failed");
        return 0;
    }

    if( bam->cells ){
        /* migrate over old elements */
        for( i=0; i < bam->n_cols; ++i ){
            for( j=0; j < bam->n_rows; ++j ){

                /* get pointer to `from` element */
                from = bam_access_cell(bam->cells, bam->n_cols, bam->n_rows, i, j);
                if( ! from ){
                    puts("bam_resize: copying over failed, call to bam_access_cell for from failed");
                    return 0;
                }

                /* get pointer to `to` slot */
                to = bam_access_cell(new_cells, num_cols, num_rows, i, j);
                if( ! to ){
                    puts("bam_resize: copying over failed, call to bam_access_cell for to failed");
                    return 0;
                }

                /* perform copy */
                *to = *from;
            }
        }

        /* free old elements */
        free(bam->cells);
    }

    /* swap */
    bam->cells = new_cells;
    bam->n_rows = num_rows;
    bam->n_cols = num_cols;

    return 1;
}

/* get current number of nodes
 *
 * returns number of nodes on success (which may be 0)
 * returns 0 on error
 */
unsigned int bam_size(struct bitwise_adj_mat *bam){
    if( ! bam ){
        puts("bam_size: bam was null");
        return 0;
    }

    return bam->n_rows;
}

/* add a directed edge from node number `from` to node number `to`
 *
 * from and to must be less than current size, otherwise it is an error
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_add_edge(struct bitwise_adj_mat *bam, unsigned int from, unsigned int to){

    /* FIXME consider how much work we need to do here since we do work in bam_access */

    if( ! bam ){
        puts("bam_add_edge: bam was null");
        return 0;
    }

    if( from >= bam->n_rows ){
        puts("bam_add_edge: from node is out of range");
        return 0;
    }

    if( to >= bam->n_rows ){
        puts("bam_add_edge: to node is out of range");
        return 0;
    }

    /* set edge to 1 (final arg) */
    if( ! bam_set_edge(bam, from, to, 1) ){
        puts("bam_add_edge: call to bam_set_edge failed");
        return 0;
    }

    return 1;
}

/* remove the directed edge from node number `from` to node number `to`.
 * such an edge doesn't have to already exist
 *
 * from and to must be less than current size, otherwise it is an error
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_remove_edge(struct bitwise_adj_mat *bam, unsigned int from, unsigned int to){

    /* FIXME consider how much work we need to do here since we do work in bam_access */

    if( ! bam ){
        puts("bam_remove_edge: bam was null");
        return 0;
    }

    if( from >= bam->n_rows ){
        puts("bam_remove_edge: from node is out of range");
        return 0;
    }

    if( to >= bam->n_rows ){
        puts("bam_remove_edge: to node is out of range");
        return 0;
    }

    /* set edge to 0 (final arg) */
    if( ! bam_set_edge(bam, from, to, 0) ){
        puts("bam_add_edge: call to bam_set_edge failed");
        return 0;
    }

    return 1;
}

/* test if an edge exists from node number `from` to node number `to`.
 *
 * if `from` or `to` are not less than current size then `0` is returned
 *
 * returns 1 if edge exists
 * returns 0 if edge does not exist
 */
unsigned int bam_test_edge(struct bitwise_adj_mat *bam, unsigned int from, unsigned int to){

    /* FIXME consider how much work we need to do here since we do work in bam_access */

    if( ! bam ){
        puts("bam_test_edge: bam was null");
        return 0;
    }

    if( from >= bam->n_rows ){
        puts("bam_test_edge: from node is out of range");
        return 0;
    }

    if( to >= bam->n_rows ){
        puts("bam_test_edge: to node is out of range");
        return 0;
    }

    return bam_get_edge(bam, from, to);
}


