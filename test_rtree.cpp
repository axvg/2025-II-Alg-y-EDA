#include "rtree_trait.h"
#include "rtree_rectangle.h"
#include "rtreenode.h"
#include "rtree.h"

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    using MyTrait = RTreeTrait<int, 2, long>; // Int coordinates, 2D, ID is long
    using Tree    = RTree<MyTrait>;
    using Rect    = Rectangle<MyTrait>;

    Tree myTree;
    cout << "--- R-TREE FINAL TEST ---" << endl;

    // 1. INSERCIÓN MASIVA
    cout << "1. Insertando datos..." << endl;
    for(int i=0; i<20; ++i)
    {
        Rect r;
        int p[] = {i, i};
        r.Set(p, p); // Puntos: (0,0), (1,1), (2,2)...
        myTree.Insert(r, i); // ID = i
    }
    cout << "Insercion completada. Altura del arbol: " << myTree.m_Root->m_Level << endl;

    // 2. BÚSQUEDA (Range Query)
    cout << "\n2. Buscando en rango [5,5] a [8,8]..." << endl;

    Rect searchBox;
    int minS[] = {5, 5};
    int maxS[] = {8, 8};
    searchBox.Set(minS, maxS);

    vector<long> results;
    int count = myTree.Search(searchBox, results);

    cout << "Encontrados: " << count << " elementos." << endl;
    cout << "IDs: ";
    bool pass = true;
    for(long id : results)
    {
        cout << id << " ";
        if (id < 5 || id > 8) pass = false; // Validación simple
    }
    cout << endl;

    if(count == 4 && pass)
        cout << "\n[PASS] Sistema R-Tree Completamente Funcional." << endl;
    else
        cout << "\n[FAIL] Fallo en la busqueda." << endl;


    ofstream outFile("rtree_output.txt");
    if (outFile.is_open()) {
        outFile << myTree;
        outFile.close();
        cout << "Tree written to rtree_output.txt" << endl;
    } else {
        cout << "Error opening file" << endl;
    }

    // 3. Read tree from file
    cout << "\n3. Reading tree from file..." << endl;
    Tree loadedTree;
    ifstream inFile("rtree_output.txt");
    if (inFile.is_open()) {
        inFile >> loadedTree;
        inFile.close();
        cout << "Tree loaded from rtree_output.txt" << endl;
        cout << "Loaded tree height: " << loadedTree.m_Root->m_Level << endl;

        // Verify search works on loaded tree
        cout << "\n4. Searching loaded tree in range [5,5] a [8,8]..." << endl;
        vector<long> loadedResults;
        int loadedCount = loadedTree.Search(searchBox, loadedResults);
        cout << "Encontrados: " << loadedCount << " elementos." << endl;
        cout << "IDs: ";
        for(long id : loadedResults) {
            cout << id << " ";
        }
        cout << endl;
    } else {
        cout << "Error opening file for reading" << endl;
    }

    return 0;
}