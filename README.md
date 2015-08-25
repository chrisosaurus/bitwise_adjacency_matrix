# bitwise_adjacency_matrix [![Build Status](https://travis-ci.org/mkfifo/bitwise_adjacency_matrix.svg)](https://travis-ci.org/mkfifo/bitwise_adjacency_matrix) [![Coverage Status](https://coveralls.io/repos/mkfifo/bitwise_adjacency_matrix/badge.svg)](https://coveralls.io/github/mkfifo/bitwise_adjacency_matrix)

adjacency matrix implementation using bitwise operations

each edge is stored using a single bit with 0 for no edge, 1 for edge.

this is intended as a more space efficient implementation compared to https://github.com/mkfifo/naive_adjacency_matrix

Time / Speed comparisons will come shortly.

example usage
=============

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

output:


    chris@Ox1b bitwise_adjacency_matrix(master)-> make example

    running example
    ./example
    edge exists from 0 to 1
    no edge exists from 1 to 0
    removed edge from 0 to 1

    list of all edges
    0 -> 2
    2 -> 3

