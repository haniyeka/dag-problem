    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include "graph_utils.h"

    int dfs(uint32_t digraph[], int n_vertices, int vertex, int visited[], int recStack[]);

    /*
    * Forward declarations of is_dag
    * See function definition further down for documentation
    */
    int is_dag(uint32_t digraph[], int n_vertices);


    int main(int argc, const char **argv)
    {
        int i;

        if (argc <= 1) {
            fprintf(stderr, "\nUsage: is_dag <digraph file>+\n\n");
            return 1;
        }

        for (i = 1; i < argc; i++) {
            uint32_t *digraph;
            int n_vertices;

            if (read_graph(argv[i], &digraph, &n_vertices)) {
                fprintf(stderr, "Error reading digraph from file %s\n", argv[i]);
                return 1;
            }
            printf("%s is a dag? %i\n", argv[i], is_dag(digraph, n_vertices));

            free(digraph);
        }

        return 0;
    }


    /********************************************************************************
    *
    * is_dag
    *
    * Determines whether the given digraph is a dag
    *
    * Given a directed graph and the number of vertices in the graph, returns 1 if
    * the digraph is a dag and 0 otherwise.
    *
    * The digraph is represented as an array of adjacency lists, and each adjacency
    * list is stored as a uint32_t that may be interpreted as a bit-array
    * representation of an adjacency list, with a 1 in bit position i indicates
    * that there is a directed edge to vertex i. Since each adjacency list (i.e.,
    * each uint32_t value of the array) can only represent out edges to vertices 0
    * through 31, the digraph can have at most 32 vertices.
    *
    ********************************************************************************/
    int is_dag(uint32_t digraph[], int n_vertices) {
        int visited[32];
        int recStack[32];
        int i;

        /* Initialize stack arrays */
        for (i = 0; i < n_vertices; i++) {
            visited[i] = 0;
            recStack[i] = 0;
        }

        /* dfs to detect cycle in different DFS trees */
        /* With DFS we can handle deeper graphs in this case since we want to find a cycle */
        /* Cycle starts from a node and ends at exact node */
        for (i = 0; i < n_vertices; i++)
            if (!visited[i] && dfs(digraph, n_vertices, i, visited, recStack))
                return 0;  /* It's not DAG since cycle is found */

        return 1;  /* It's DAG since no cycle is found */
    }

    int dfs(uint32_t digraph[], int n_vertices, int vertex, int visited[], int recStack[]) {
        int j;
        /* If we haven't visited this vertex yet */

        if (!visited[vertex]) {
            visited[vertex] = 1; /* Mark it as visited */ 
            recStack[vertex] = 1; /* Add it to our stack */

            /* look at each bit in the adjacency list to check each possible connection */
            for (j = 0; j < n_vertices; j++) {
                /* If there's a connection from our vertex to j */
                if (digraph[vertex] & (1 << j)) {
                    /* If j hasn't been visited yet and we find a cycle by exploring from j */
                    if (!visited[j] && dfs(digraph, n_vertices, j, visited, recStack))
                        return 1; /* We found a cycle, so return 1 */
                        
                    else if (recStack[j])
                        /* If j is already in our temporary stack */
                        return 1; /* We found a cycle, so return 1 */
                }
            }
        }

        recStack[vertex] = 0;  /* removing vertex from stack */
        return 0;
    }

