/*  gcc bitwise_adj_mat.c test_bitwise_adj_mat.c -Wall -Wextra -Werror -o test_bam
 * ./test_bam
 */
#include <assert.h> /* assert */
#include <stdio.h> /* puts */

#include "bitwise_adj_mat.h"

void simple(void );
void init(void );
void resize(void );
void sizing(void );
void null(void);
void invalid(void);

void simple(void){
    struct bitwise_adj_mat *bam = 0;
    int i = 0;
    int j = 0;

    puts("\ntesting basic usage");

    bam = bam_new(0);
    assert( bam );

    assert( bam_resize(bam, 5) );

    assert( bam->n_rows == 5 );
    assert( bam->n_cols == 1 );
    assert( bam_size(bam) == 5 );

    for( i=0; i<5; ++i ){
        for( j=0; j<5; ++j ){
            assert( 0 == bam_test_edge(bam, i, j) );
        }
    }

    assert( bam_add_edge(bam, 0, 2) );
    assert( bam_add_edge(bam, 1, 0) );
    assert( bam_add_edge(bam, 3, 1) );

    assert( bam_test_edge(bam, 0, 2) );
    assert( bam_test_edge(bam, 1, 0) );
    assert( bam_test_edge(bam, 3, 1) );

    assert( 0 == bam_test_edge(bam, 2, 0) );
    assert( 0 == bam_test_edge(bam, 0, 1) );
    assert( 0 == bam_test_edge(bam, 1, 3) );


    assert( bam_remove_edge(bam, 0, 2) );
    assert( 0 == bam_test_edge(bam, 0, 2) );
    assert( bam_test_edge(bam, 1, 0) );
    assert( bam_test_edge(bam, 3, 1) );

    assert( bam_remove_edge(bam, 1, 0) );
    assert( 0 == bam_test_edge(bam, 1, 0) );

    assert( bam_remove_edge(bam, 3, 1) );
    assert( 0 == bam_test_edge(bam, 3, 1) );


    for( i=0; i<5; ++i ){
        for( j=0; j<5; ++j ){
            assert( 0 == bam_test_edge(bam, i, j) );
        }
    }

    /* free bam as it was allocated via new */
    assert( bam_destroy(bam, 1) );

    puts("success!");
}

void init(void){
    struct bitwise_adj_mat bam;

    puts("\ntesting init");

    assert( bam_init(&bam, 0) );

    assert( bam_resize(&bam, 4) );

    assert( bam.n_rows == 4 );
    assert( bam.n_cols == 1 );

    /* do not free bam as it was statically
     * allocated
     */
    assert( bam_destroy(&bam, 0) );

    puts("success!");
}

void resize(void){
    struct bitwise_adj_mat *bam = 0;

    puts("\ntesting resize");

    bam = bam_new(0);
    assert( bam );

    assert( bam_resize(bam, 5) );

    assert( bam->n_rows == 5 );
    assert( bam->n_cols == 1 );
    assert( bam_size(bam) == 5 );


    assert( bam_add_edge(bam, 0, 2) );
    assert( bam_add_edge(bam, 1, 0) );
    assert( bam_add_edge(bam, 3, 1) );


    assert( bam_test_edge(bam, 0, 2) );
    assert( bam_test_edge(bam, 1, 0) );
    assert( bam_test_edge(bam, 3, 1) );

    assert( 0 == bam_test_edge(bam, 2, 0) );
    assert( 0 == bam_test_edge(bam, 0, 1) );
    assert( 0 == bam_test_edge(bam, 1, 3) );


    assert( bam_resize(bam, 9) );

    assert( bam->n_rows == 9 );
    assert( bam->n_cols == 2 );
    assert( bam_size(bam) == 9 );

    assert( bam_test_edge(bam, 0, 2) );
    assert( bam_test_edge(bam, 1, 0) );
    assert( bam_test_edge(bam, 3, 1) );

    assert( 0 == bam_test_edge(bam, 2, 0) );
    assert( 0 == bam_test_edge(bam, 0, 1) );
    assert( 0 == bam_test_edge(bam, 1, 3) );

    assert( bam_add_edge(bam, 8, 7) );

    assert( bam_test_edge(bam, 8, 7) );


    assert( bam_resize(bam, 27) );

    assert( bam->n_rows == 27 );
    assert( bam->n_cols == 4 );
    assert( bam_size(bam) == 27 );

    assert( bam_test_edge(bam, 0, 2) );
    assert( bam_test_edge(bam, 1, 0) );
    assert( bam_test_edge(bam, 3, 1) );
    assert( bam_test_edge(bam, 8, 7) );

    assert( 0 == bam_test_edge(bam, 2, 0) );
    assert( 0 == bam_test_edge(bam, 0, 1) );
    assert( 0 == bam_test_edge(bam, 1, 3) );
    assert( 0 == bam_test_edge(bam, 7, 8) );

    assert( bam_add_edge(bam, 26, 4) );

    assert( bam_test_edge(bam, 26, 4) );


    /* free bam as it was allocated via new */
    assert( bam_destroy(bam, 1) );

    puts("success!");
}

void sizing(void){
    struct bitwise_adj_mat *bam = 0;
    unsigned int i = 0;
    unsigned int j = 0;

    puts("\ntesting initial sizing");

    bam = bam_new(6);
    assert( bam );

    assert( bam->n_rows == 6 );
    assert( bam->n_cols == 1 );
    assert( bam_size(bam) == 6 );


    for( i=0; i<5; ++i ){
        for( j=0; j<5; ++j ){
            assert( 0 == bam_test_edge(bam, i, j) );
        }
    }


    /* free bam as it was allocated via new */
    assert( bam_destroy(bam, 1) );

    puts("success!");
}

void null(void){
    puts("\ntesting calling functions with null (warnings will be printed)");

    assert( 0 == bam_init(0, 0) );

    assert( 0 == bam_destroy(0, 0) );

    assert( 0 == bam_resize(0, 0) );

    assert( 0 == bam_size(0) );

    assert( 0 == bam_add_edge(0, 0, 0) );

    assert( 0 == bam_remove_edge(0, 0, 0) );

    assert( 0 == bam_test_edge(0, 0, 0) );

    puts("success!");
}

void invalid(void){
    struct bitwise_adj_mat *bam = 0;

    puts("\ntesting calling functions with invalid arguments (warnings will be printed)");

    /* 2 * 2 */
    bam = bam_new(2);
    assert(bam);


    /* cannot resize to 0 */
    assert( 0 == bam_resize(bam, 0) );

    /* cannot add an edge with a from >= size (2) */
    assert( 0 == bam_add_edge(bam, 2, 0) );

    /* cannot add an edge with a to >= size (2) */
    assert( 0 == bam_add_edge(bam, 0, 2) );

    /* cannot remove an edge with a from >= size (2) */
    assert( 0 == bam_remove_edge(bam, 2, 0) );

    /* cannot remove an edge with a to >= size (2) */
    assert( 0 == bam_remove_edge(bam, 0, 2) );

    /* cannot test an edge with a from >= size (2) */
    assert( 0 == bam_test_edge(bam, 2, 0) );

    /* cannot test an edge with a to >= size (2) */
    assert( 0 == bam_test_edge(bam, 0, 2) );


    assert( bam_destroy(bam, 1) );
    puts("success!");
}


int main(void){
    simple();

    init();

    resize();

    sizing();

    null();

    invalid();

    puts("\noverall testing success!");

    return 0;
}

