#include "Utils.hpp"
#include "PolygonalMesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

double findMax(const double& a, const double& b, const double& c){
    double max_value = max(a,max(b,c));
    return max_value;
}


double polyg_area(const vector <Vector2d>& vertices){ //using gaussian formula for the polygon's area
    double area = 0;
    unsigned int n = vertices.size();
    for(unsigned int i = 0; i < n-1; i++)
    {
        area += vertices[i][0] * vertices[i+1][1] - vertices[i+1][0] * vertices[i][1];
    }
    area += vertices[n-1][0] * vertices[0][1] - vertices[n-1][1] * vertices[0][0];
    area *= 0.5;
    return area;
}

double dist_euclid(const Vector2d& v1, const Vector2d& v2){
    double dist = sqrt(pow((v1[0] - v2[0]),2) + pow((v1[1]-v2[1]),2));
    return dist;
}


namespace PolygonalLibrary {

bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while(getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if(mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for(const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        char pv; //caricando il ; in una variabile pv posso usare >>
        Vector2d coord;

        converter >> id >> pv >> marker >> pv >> coord(0) >> pv >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if(marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first) -> second.push_back(id);
        }
    }
    file.close();
    return true;
}

bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }
    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        Vector2i vertices;
        unsigned int id;
        unsigned int marker;
        char pv;

        converter >> id >> pv >> marker >> pv >> vertices(0) >> pv >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}


bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int NumVertices;
        unsigned int NumEdges;

        vector<unsigned int> vertices;
        vector<unsigned int> edges;

        converter >>  id >> marker >> NumVertices; //devo fare in questo modo perchè non so quanti vertici ci siano in ogni poligono
        for(unsigned int i = 0; i < NumVertices; i++)
            converter >> vertices[i];

        converter >> NumEdges;
        for(unsigned int i = 0; i < NumEdges; i++)
            converter >> edges[i];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
    }
    file.close();
    return true;
}

bool ImportMesh(const string &filepath, PolygonalMesh& mesh)
{
    if(!ImportCell0Ds("Cell0Ds.csv", mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        if(mesh.Cell0DMarkers.size() == 0)
        {
            cout << "No Cell0D with marker different from 0 found" << endl;
        }
        else
        {
            for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
            {
                cout << "key:\t" << it -> first << "\t values:";
                for(const unsigned int id : it -> second)
                    cout << "\t" << id;
                cout << endl;
            }

        }

    }

    if(!ImportCell1Ds("Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        if(mesh.Cell1DMarkers.size() == 0)
        {
            cout << "No Cell1D with marker different from 0 found" << endl;
        }
        else
        {
            for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
            {
                cout << "key:\t" << it -> first << "\t values:";
                for(const unsigned int id : it -> second)
                    cout << "\t" << id;

                cout << endl;
            }
        }

    }

    if(!ImportCell2Ds("Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        //Check that edges of polygonals have non-zero lenght
        double toll_1D = pow(10,-15);

        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            Vector2i vert = mesh.Cell1DVertices[c];
            Vector2d origin = mesh.Cell0DCoordinates[vert[0]];
            Vector2d end = mesh.Cell0DCoordinates[vert[1]];

            double dist = dist_euclid(origin, end);
            if (dist < toll_1D)
            {
                cerr << "Edges with vertices " << origin << "and " << end << "have zero length";
            }
        }
    }

    return true;

}
}

