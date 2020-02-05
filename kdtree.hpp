/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
     if(first[curDim] == second[curDim]){
       return first < second;
     }
     return first[curDim] < second[curDim] ? true : false;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
     double current_squared = 0.0;
     double potential_squared = 0.0;
     int i = 0;
     while(i < Dim){
       current_squared += (currentBest[i] - target[i]) * (currentBest[i] - target[i]);
       potential_squared += (target[i] - potential[i]) * (target[i] - potential[i]);
       i++;
     }
     if(potential_squared == current_squared){
       return potential < currentBest;
     }
     return potential_squared < current_squared ? true : false;
     }

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
     if(newPoints.empty()){
       root = NULL;
       size = 0;
     }
     else{
     size = 0;
     vector<Point<Dim>> points_ = newPoints;
     conHelper(points_, 0, points_.size()-1, 0, root);
     }
}

template <int Dim>
void KDTree<Dim>::conHelper(vector<Point<Dim>>& newPoints, int left, int right, int dimension, KDTreeNode *& subroot){
  if(left > right){
    return;
  }
  size_t median = (left + right) / 2;
  Point<Dim> temp = select(newPoints, left, right, median, dimension);
  subroot = new KDTreeNode(temp);
  size++;
  conHelper(newPoints, left, median - 1, (dimension + 1) % Dim, subroot->left);
  conHelper(newPoints, median + 1, right, (dimension + 1) % Dim, subroot->right);
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& vec, int left, int right, size_t index, int dimension){
  Point<Dim> value = vec[index];
  Point<Dim> temp = vec[index]; //swapping
  vec[index] = vec[right];
  vec[right] = temp;
  size_t ltemp = left; //assigning left to variable

int i = left;
while(i < right){
  if(smallerDimVal(vec[i], value, dimension)){
    temp = vec[ltemp];
    vec[ltemp] = vec[i];
    vec[i] = temp;
    ltemp++;
  }
  i++;
}
  temp = vec[ltemp];
  vec[ltemp] = vec[right];
  vec[right] = temp;
  return ltemp;
}

template <int Dim>
Point<Dim> KDTree<Dim>::select(vector<Point<Dim>>& vec, int left, int right, size_t median, int dimension){
  size_t index = (left + right)/2;
  index = partition(vec, left, right, index, dimension);
  if(median == index){
    return vec[median];
  }
return median < index ? select(vec, left, index - 1, median, dimension) :
                        select(vec, index + 1, right, median, dimension);
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
   copy_(root, other->root);
   size = other.size;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
   delete_(root);
   copy_(root, rhs.root);
   return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   delete_(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
     Point<Dim> currentBest = root->point;
     Point<Dim> temp = query;
     nearestNeighborHelper(root, currentBest, temp, 0);
     return currentBest;
}

template <int Dim>
void KDTree<Dim>::nearestNeighborHelper(KDTreeNode* curr, Point<Dim> &currentBest, Point<Dim>& query, int dimension) const
{
  if(curr == NULL){
    return;
  }
  KDTreeNode* next;
  KDTreeNode* other;
  if(smallerDimVal(curr->point, query, dimension)){
      next = curr->right;
      other = curr->left;
  }else{
      next = curr->left;
      other = curr->right;
  }
  nearestNeighborHelper(next, currentBest, query, (dimension + 1) % Dim);
  if(shouldReplace(query, currentBest, curr->point)){
    currentBest = curr->point;
  }
  double radius = 0.0;
  int i = 0;
  while(i < Dim){
    radius += (currentBest[i] - query[i]) * (currentBest[i] - query[i]);
    i++;
  }
  double distance = curr->point[dimension] - query[dimension];
  distance = distance * distance;
  if(distance <= radius){
    nearestNeighborHelper(other, currentBest, query, (dimension + 1) % Dim);
  }
}

template <int Dim>
void KDTree<Dim>::delete_(KDTreeNode* subroot){
  if(subroot == NULL){
    return;
  }
  if(subroot->left != NULL){
    delete_(subroot->left);
  }
  if(subroot->right != NULL){
    delete_(subroot->right);
  }
  delete subroot;
  subroot = NULL;
}

template <int Dim>
void KDTree<Dim>::copy_(KDTreeNode* subroot, KDTreeNode* othersubroot){
  subroot = new KDTreeNode();
  subroot->point = othersubroot->point;
  copy_(subroot->left, othersubroot->left);
  copy_(subroot->right, othersubroot->right);
}
