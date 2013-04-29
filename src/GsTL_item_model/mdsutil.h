#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <QString>
#include <utility>

namespace MDSUtil {

// This is a struct that stores an individual cluster
// that it stores the number of properties that are within
// the cluster and maybe even more information in the future.

struct cluster {
        unsigned int numComponents;
        std::vector< std::pair<QString, int> > components;
        QString centerName;
};

// This is a struct that stores a block of clusters, for instance,
// suppose we run k-means with 5 blocks, then this would have 5
// seperate clusters in it.
struct clusterBlock{
    unsigned int numBlock;
    std::vector<cluster> block;
};
}//end namespace
#endif // UTIL_H

