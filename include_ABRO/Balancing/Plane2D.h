/*
*   This class is a representation of a 2D plane.
*
*   Functions are added using the modified library MyGAL to help the evenly scatter the Points in the Figure
*
*   \Author Vicente Serna
*/

//My Imports
#include "MyGAL/FortuneAlgorithm.h"
#include <vector>
#include "MyGAL/Vector2.h"
#include <thread>
#include <cmath>
#include <random>
#include<iostream>

namespace plane{

    using namespace mygal;
    using Float = double;

    class Plane2D {
    public:
        std::vector<Vector2<Float>> vertices;
        std::vector<Vector2<int>> edges;
        std::vector<Vector2<Float>> scatteredPoints;

        Plane2D(std::vector<Vector2<Float>>& vertices, std::vector<Vector2<int>>& edges) {
            this->vertices = vertices;
            this->edges = edges;

        }

        void scatterPoints(int nPoints,int nIterations) {
            scatteredPoints.reserve(nPoints);

            fillCenteredSquare(nPoints);

            constructDenseEdges(0.01f);
            int sizeVertices = vertices.size();

            vertices.insert(vertices.end(), scatteredPoints.begin(), scatteredPoints.end());

            auto diagram = generateDiagram(vertices);

            std::cout<<"Size vertices: " << vertices.size() << std::endl;
            for (int i = 0; i < nIterations; i++) {
                auto y = diagram.computeLloydRelaxationParallelized(sizeVertices);
                diagram = generateDiagram(y);
            }

            const std::vector<Vector2<Float>> endVertices = diagram.getVerticesVec();
            vertices.assign(endVertices.begin(), endVertices.end());
            
        }

        const std::vector<Vector2<Float>>& getPoints()const{
            return vertices;
        }


    private:
        void fillCenteredSquare(int nbPoints) {
            float min = 10;
            for (const auto& vertice : vertices) {
                float dis = std::pow(vertice.x - 0.5, 2) + std::pow(vertice.y - 0.5, 2);
                if (dis < min) { min = dis; }
            }
            min = std::sqrt(min);

            auto generator = std::default_random_engine(static_cast<float>(min));
            auto len = min * 0.7071067;
            auto distribution1 = std::uniform_real_distribution<Float>(0.5 - len, 0.5 + len);

            for (int i = 0; i < nbPoints; i++) {
                scatteredPoints.push_back(Vector2<Float>(distribution1(generator),distribution1(generator)));
            }
        }

        /**
        * \brief Adds Points between all Points forming edges
        *
        *
        * \param edgeL distance between every point
        */
        void constructDenseEdges(float edgeL) {
            for (const auto& edge : edges) {
                auto p1 = vertices[edge.x];
                auto p2 = vertices[edge.y];

                Vector2<Float> direction = { p2.x - p1.x,p2.y - p1.y };

                int totalPoints = static_cast<int>(1 / edgeL);

                //scatteredPoints.reserve(scatteredPoints.size() + totalPoints);
                /*I don´t know if this improves performance at all*/

                for (int i = 1; i < totalPoints - 1; i++) {
                    vertices.push_back(Vector2<Float>(p1.x + direction.x * edgeL * i, p1.y + direction.y * edgeL * i));
                }
            }
        }

        Diagram<Float> generateDiagram(const std::vector<Vector2<Float>>& points) {
            // Construct diagram
            FortuneAlgorithm<Float> algorithm = FortuneAlgorithm<Float>(points);
            algorithm.construct();

            // Bound the diagram
            algorithm.bound(Box<Float>{-0.05, -0.05, 1.05, 1.05}); // Take the bounding box slightly bigger than the intersection box
            Diagram<Float> diagram = algorithm.getDiagram();

            diagram.intersect(Box<Float>{0.0, 0.0, 1.0, 1.0});

            return diagram;
        }
    };

}