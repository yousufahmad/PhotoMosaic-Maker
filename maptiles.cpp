/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
     map<Point<3>, size_t> maps;
    vector<Point<3>> points;

    for(size_t i = 0; i < theTiles.size(); i++){
      LUVAPixel pixel = theTiles[i].getAverageColor();
      Point<3> point = convertToXYZ(pixel);
      points.push_back(point);
      maps.insert(pair<Point<3>, size_t>(point,i));
    }

    MosaicCanvas* canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    KDTree<3>* tree = new KDTree<3>(points);
    for(int i = 0; i < theSource.getRows(); i++){
      for(int j = 0; j < theSource.getColumns(); j++){
        //LUVAPixel rcolor = theSource.getRegionColor(i, j);
        Point<3> rpoint = convertToXYZ(theSource.getRegionColor(i, j));
        Point<3> npoint = tree->findNearestNeighbor(rpoint);
        size_t index = maps[npoint];
        canvas->setTile(i, j, &theTiles[index]);
      }
    }
    delete tree;
    return canvas;

  //  return NULL;
}
