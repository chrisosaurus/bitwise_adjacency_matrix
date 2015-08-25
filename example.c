#include <stdio.h>

#include "bitwise_adj_mat.h"

int main(void){
    /* create a new adjacency matrix with 3 nodes */
    struct bitwise_adj_mat *bam = bam_new(4);

    /* temporary integers used later */
    int i = 0;
    int j = 0;
    int n = 0;

    /* add some edges */
    /* 0 -> 1 */
    bam_add_edge(bam, 0, 1);
    /* 0 -> 2 */
    bam_add_edge(bam, 0, 2);
    /* 2 -> 3 */
    bam_add_edge(bam, 2, 3);

    /* test existing connections */
    if( bam_test_edge(bam, 0, 1) ){
        puts("edge exists from 0 to 1");
    }

    /* test non-existing connections */
    if( ! bam_test_edge(bam, 1, 0) ){
        puts("no edge exists from 1 to 0");
    }

    /* delete edge 0 -> 1 */
    bam_remove_edge(bam, 0, 1);

    /* test removed connections */
    if( ! bam_test_edge(bam, 0, 1) ){
        puts("removed edge from 0 to 1");
    }

    /* get number of nodes */
    n = bam_size(bam);

    puts("\nlist of all edges");

    /* iterate through nodes */
    for( i = 0; i<n; ++i ){
        for( j=0; j<n; ++j ){
            if( bam_test_edge(bam, i, j) ){
                printf("%d -> %d\n", i, j);
            }
        }
    }

    /* cleanup
     *  destroy(bam, free_bam) */
    bam_destroy(bam, 1);

    return 0;
}

