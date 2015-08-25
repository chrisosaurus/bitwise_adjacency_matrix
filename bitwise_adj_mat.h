#ifndef BITWISE_ADJ_MAT_H
#define BITWISE_ADJ_MAT_H

#include <stdint.h> /* uint8_t */

/* this library tries to improve over the 'bitwise_adjacency_matrix` lib
 * by not wasting bits
 *
 * this means that an edge is only represented as a single bit within an uint8_t
 */
struct bitwise_adj_mat {
    /* number of rows in matrix
     * also number of nodes
     */
    unsigned int n_rows;

    /* number of columns in matrix
     * this is (n_rows +7) / 8
     */
    unsigned int n_cols;

    /* 2d array of uint8_t with each cell representing 8 edges
     * each cell is a collection of 8 edges, each set to 0 or 1
     * stored in row-major order
     *
     * index = row * n_cols + (col / 8);
     * edge = cells[index] & 1 << (col % 8);
     *
     * current size is n_rows * n_cols
     */
    uint8_t *cells;
};

/* allocate and initialise a new adj. matrix containing `num_nodes` nodes
 * `num_nodes` may be 0
 *
 * returns * on success
 * returns 0 on error
 */
struct bitwise_adj_mat * bam_new(unsigned int num_nodes);

/* initialise an existing adj. matrix containing `num_nodes` nodes
 * `num_nodes` may be 0
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_init(struct bitwise_adj_mat *bam, unsigned int num_nodes);

/* destroy an existing adj. matrix
 * will call free on `bam` if `free_bame` is truethy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_destroy(struct bitwise_adj_mat *bam, unsigned int free_bam);

/* resize an existing adj. matrix to include enough space for
 * the number of nodes specified by `num_nodes`
 * `num_nodes` must be greater than 0
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_resize(struct bitwise_adj_mat *bam, unsigned int num_nodes);

/* get current number of nodes
 *
 * returns number of nodes on success (which may be 0)
 * returns 0 on error
 */
unsigned int bam_size(struct bitwise_adj_mat *bam);

/* add a directed edge from node number `from` to node number `to`
 *
 * from and to must be less than current size, otherwise it is an error
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_add_edge(struct bitwise_adj_mat *bam, unsigned int from, unsigned int to);

/* remove the directed edge from node number `from` to node number `to`.
 * such an edge doesn't have to already exist
 *
 * from and to must be less than current size, otherwise it is an error
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int bam_remove_edge(struct bitwise_adj_mat *bam, unsigned int from, unsigned int to);

/* test if an edge exists from node number `from` to node number `to`.
 *
 * if `from` or `to` are not less than current size then `0` is returned
 *
 * returns 1 if edge exists
 * returns 0 if edge does not exist
 */
unsigned int bam_test_edge(struct bitwise_adj_mat *bam, unsigned int from, unsigned int to);


#endif //BITWISE_ADJ_MAT_H

