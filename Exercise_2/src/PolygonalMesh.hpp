#pragma once
#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary{

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0; ///< number of Cell0D
    std::vector<unsigned int> Cell0DId = {}; ///< Cell0D id, size 1xNumberCell0D
    std::vector<Vector2d> Cell0DCoordinates = {}; ///< Cell0D coordinates, size 2xNumberCell0D (x,y)
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; ///< Cell0D markers, size 1xNumberCell0D (marker)

    unsigned int NumberCell1D = 0; ///< number of Cell1D
    std::vector<unsigned int> Cell1DId = {}; ///< Cell1D id, size 1xNumberCell0D
    std::vector<Vector2i> Cell1DVertices = {}; ///< Cell1D vertices indices, size 2xNumberCell1D (fromid,toid)
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; ///< Cell1D properties, size 1xNumberCell1D (marker)

    unsigned int NumberCell2D = 0; ///< number of Cell2D
    std::vector<unsigned int> Cell2DId = {}; ///< Cell2D id, size 1xNumberCell0D
    unsigned int Cell2DNumVertices = 0;
    unsigned int Cell2NumEdges = 0;
    std::vector<std::vector<unsigned int>> Cell2DVertices = {};
    unsigned int Cell2DNumEdges = 0;
    std::vector<std::vector<unsigned int>> Cell2DEdges = {};
};
}




