#include <iostream>
#include <algorithm>
#include <map>
#include "Codecki_LIB.cpp"

using namespace Codecki;



struct Point
{
    int classLabel;
    std::vector<double> DimensionList;
};

std::vector<Point> TrainingSet;
int numOfClasses;

void printVectorOfPoints(std::vector<Point> const & vectorToPrint)
{
    for (int j = 0; j < vectorToPrint.size(); ++j) {
        std::cout << "Row " << j << "\n";
        for (int i = 0; i < vectorToPrint.at(j).DimensionList.size(); ++i) {
            std::cout << vectorToPrint.at(j).DimensionList.at(i) << ", ";
        }
        std::cout << "\n";
    }
}

struct PointAndDistance
{
    Point point;
    double distanceBetweenThisPointAndpointToClassify;

    bool operator < (const PointAndDistance& pointAndDistance) const
    {
        return (distanceBetweenThisPointAndpointToClassify < pointAndDistance.distanceBetweenThisPointAndpointToClassify);
    }
};

double EuclideanDistanceBetweenTwoPoints(Point const & point1,Point const & point2)
{
    double sum =0;
    if(point1.DimensionList.size() != point2.DimensionList.size()) throw std::exception();
    for (int i = 0; i < point1.DimensionList.size(); ++i) {
        double difference = point1.DimensionList.at(i) - point2.DimensionList.at(i);
        double power = std::pow(difference, 2);
        sum += power;
    }

    return std::sqrt(sum);
}

//  returns a ClassLabel
int MostCommonClassLabel(std::vector<PointAndDistance> const & PointAndDistanceList)
{
    std::map<int, int> ClassLabelsAndTheirPopularity;
    for (int i = 1; i < numOfClasses +1; ++i) {
        ClassLabelsAndTheirPopularity.insert({i,0});
    }
    for (int j = 0; j < PointAndDistanceList.size(); ++j) {
        ClassLabelsAndTheirPopularity.at(PointAndDistanceList.at(j).point.classLabel)++;
    }

    // Biggest value
    int BiggestValue = 0;
    int BiggestLabelIndex = 0;
    for (int k = 1; k < numOfClasses +1; ++k) {
        if(BiggestValue < ClassLabelsAndTheirPopularity.at(k))
        {
            BiggestValue = ClassLabelsAndTheirPopularity.at(k);
            BiggestLabelIndex = k;
        }
    }

    return BiggestLabelIndex;
}

bool kNN_Algorithm(Point const & pointToClassify, int k_choosenNumberOfNeighbors)
{
    std::vector<PointAndDistance> PointAndDistanceList;
//    print("TrainingSet size: "); printline(TrainingSet.size());


    for(auto const & point : TrainingSet)
    {
        // calculate the distance
        double distance = EuclideanDistanceBetweenTwoPoints(pointToClassify, point);
        PointAndDistance tempPointAndDistance{point,distance};
        PointAndDistanceList.push_back(tempPointAndDistance);
    }

    std::sort(PointAndDistanceList.begin(), PointAndDistanceList.end());

//    print("PointAndDistanceList size: "); printline(PointAndDistanceList.size());
//    for(auto pointAndDistance: PointAndDistanceList)
//    {
//        print("Distance: "); printline(pointAndDistance.distanceBetweenThisPointAndpointToClassify);
//    }

    std::vector<PointAndDistance> subVector;
    subVector = std::vector<PointAndDistance>(PointAndDistanceList.begin() + 1, PointAndDistanceList.begin() + 1 + k_choosenNumberOfNeighbors);

//    print("subVector size: "); printline(subVector.size());
//    for(auto pointAndDistance: subVector)
//    {
//        print("Distance: "); print(pointAndDistance.distanceBetweenThisPointAndpointToClassify); print(" Label: "); printline(pointAndDistance.point.classLabel);
//    }

    //print("Most Common Label: "); printline(MostCommonClassLabel(subVector));
    // Check if the point was correctly classified
    if(MostCommonClassLabel(subVector) == pointToClassify.classLabel)
    {
        printline("OK. The point was correctly classified");
        return true;
    }
    else
    {
        printline("ERROR. The point was NOT correctly classified");
        return false;
    }
}


int main() {


    int numOfInputValuesInOneRow;
    int numOfLines;

    std::fstream File("D:\\Projekty\\CPP\\CondensedNearestNeigtbourAlgorithm\\Test_s_epj.txt", std::ios::in);
    File >> numOfClasses >> numOfInputValuesInOneRow >> numOfLines;
    std::stringstream ss;
    ss << File.rdbuf();
    File.close();

    int someGarbage; // for some reason there is an additional number at the beggining
    File >> someGarbage;

    for (int i = 0; i < numOfLines; i++) {

        Point tempPoint;
        std::vector<double> tempVector;
        ss >> tempPoint.classLabel;

        for (int j = 0; j < numOfInputValuesInOneRow; j++) {

            double tempDouble;
            ss >> tempDouble;
            tempVector.push_back(tempDouble);
        }
        tempPoint.DimensionList = tempVector;
        TrainingSet.push_back(tempPoint);
    }

//    printVectorOfPoints(TrainingSet);

    //  Start of the actual program

    std::vector<Point> AbsorbedPoints;

    for (int k = 0; k < TrainingSet.size(); ++k) {

        if(kNN_Algorithm(TrainingSet.at(k), 6))
        {
            AbsorbedPoints.push_back(TrainingSet.at(k));
        }

    }



return 0;
}


