#ifndef _KMEANS_H_
#define _KMEANS_H_

/* Kernel k-means clustering

Alexander Chia
alexchia@stanford.edu
*/

#include <metrics_action_common.h>
#include <string.h>
#include <stdlib.h>
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <GenericKernels.h>
#include <MetricData.h>

using namespace Eigen;
using namespace std;
using Eigen::MatrixXf;


/** class definition */
class METRICSACTIONS_DECL kmeans
{
public:
	kmeans(void);
	~kmeans(void);

	// pointer to data, resulting cluster assignment, number of elements, number of clusters, optional initial cluster assignments
	// input kernel should be assigned to MetricData
	bool execute(const MetricDataCollection* data, int* result, int numElts, int num_clusters, int* initial_clusters);

};

/** begin functions */
kmeans::kmeans(void)
{
}

kmeans::~kmeans(void)
{
}

// execute k-means

bool kmeans::execute(const MetricDataCollection* data, int *result, int numElts, int num_clusters, int* initial_clusters = 0) {

  const int MAX_ITERATIONS = 100;
  const float INFINITY = 9999999999.0f;

	if (numElts <= 0) return false;

	int i, j, k;

	int* cluster = (int*)malloc(sizeof(int) * numElts);
	// cluster assignments for next step
	int* next_cluster = (int*)malloc(sizeof(int) * numElts);

	// sum of kernel products for points in each cluster
	float* cluster_pdt = (float*)malloc(sizeof(float) * num_clusters);

	// randomly assign data points to clusters if needed
	if (initial_clusters)
		memcpy(cluster,initial_clusters, sizeof(int) * numElts);
	else {
		for (i = 0; i < numElts; i++)
			cluster[i] = rand() % num_clusters;
	}

	// compute kernel matrix
	int n = numElts;
	ArrayXXf K(n, n);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			K(i, j) = data[i].distance(data[j]);
		}
	}

	// do kernel k-means clustering

	int best_cluster;		// best cluster seen so far
	float best_score;		// best score seen so far
	float score;			// current score
	vector<int> indexes;	// points in cluster
	vector<int>::iterator it1;	// iterators over the vector
	vector<int>::iterator it2;
	int num_in_cluster;		// counter

	for (int step = 0; step < MAX_ITERATIONS; step++) {

		// first, precompute sum of kernel product of points in each cluster
		for (i = 0; i < num_clusters; i++) {
			cluster_pdt[i] = 0;
			indexes.clear();
			for (j = 0; j < n ; j++)
				if (cluster[j] == i) indexes.push_back(j);
			// process each pair of points in the cluster
			for (it1 = indexes.begin(); it1 != indexes.end(); it1++)
				for (it2 = indexes.begin(); it2 != indexes.end(); it2++)
					cluster_pdt[i] += K(*it1, *it2);
			if (indexes.size() != 0)
				cluster_pdt[i] /= ((float)indexes.size() * (float)indexes.size());
			else
				cluster_pdt[i] = INFINITY;
		}

		// now get next cluster assignments for each point
		for (i = 0; i < n; i++) {
			best_cluster = cluster[i];
			best_score = INFINITY;
			// evaluate cluster j
			for (j = 0; j < num_clusters; j++) {
				score = 0;

				num_in_cluster = 0;
				for (k = 0; k < n; k++) {
					if (cluster[k] == j) {
						score += K(i, k);
						num_in_cluster++;
					}
				}
				if (num_in_cluster == 0)
					score = INFINITY;
				else
					score = K(i, i) - 2.0f*score/(float)num_in_cluster + cluster_pdt[j];

				if (score < best_score) {
					best_score = score;
					best_cluster = j;
				}
			}
			next_cluster[i] = best_cluster;
		}

		// copy cluster assignments
		memcpy(cluster, next_cluster, sizeof(int) * numElts);

	}


	// copy resulting cluster assignments to result vector
	memcpy(result, cluster, sizeof(int) * numElts);

	// cleanup
	free(cluster);
	free(next_cluster);
	free(cluster_pdt);

	return true;
}


#endif
