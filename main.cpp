#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include "Codecki_LIB.cpp"

using namespace Codecki;


// Assumptions: class labels start from '1', NOT '0'. There MUST NOT be any label missing. Example of a wrong class label set: 1,2,5,6. Correct: 1,2,3,4.

struct Point
{
    int classLabel;
    std::vector<double> DimensionList;

    auto operator==(const Point& point){
        return (this->classLabel == point.classLabel && this->DimensionList == point.DimensionList);
    }
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

// Calculates distance in Euclidean space (space that has n-dimensions). It works for 2, 3, 4 ... n dimensions.
double EuclideanDistanceBetweenTwoPoints(Point const & point1, Point const & point2)
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

// function kNN_Algorithm() returns True if a point would be classified correctly. Otherwise, it returns False.
bool kNN_Algorithm(Point const & pointToClassify, std::vector<Point> const & reducedSet, int k_choosenNumberOfNeighbors)
{
    // pointToClassify MUST NOT already be in reducedSet, so its removed
    std::vector<Point> tempReducedSet;
    for(auto const & point : reducedSet)
    {
        if(&point != &pointToClassify)
            tempReducedSet.push_back(point);
    }

    std::vector<PointAndDistance> PointAndDistanceList;
    for(auto const & point : tempReducedSet)
    {
        // calculate the distance
        double distance = EuclideanDistanceBetweenTwoPoints(pointToClassify, point);
        PointAndDistance tempPointAndDistance{point,distance};
        PointAndDistanceList.push_back(tempPointAndDistance);
    }

    std::sort(PointAndDistanceList.begin(), PointAndDistanceList.end());
    std::vector<PointAndDistance> subVector;
    subVector = std::vector<PointAndDistance>(PointAndDistanceList.begin(), PointAndDistanceList.begin() + k_choosenNumberOfNeighbors);

    // Check if the point was correctly classified and return result
    return (MostCommonClassLabel(subVector) == pointToClassify.classLabel);
}

std::pair<Point, Point> FindPairOfMutuallyClosestPoints(Point const & startingPoint, double lastDistance)
{
    // For point 'A', find the closest from a different class. Let's call the next point 'B'
    // For point 'B', find the closest from a different class.
    // if 'A' is the closest for 'B' AND 'B' is the closest for 'A'
    //      than return the pair of those two points.
    // else continue the algorithm in the same way. Let's call the next point 'C'.
    //       For point 'C', find the closest from a different class.
    //       if 'C' is the closest for 'B' AND 'B' is the closest for 'C'
    //          than return the pair of those two points.
    //       else ...
    //  ...
    //  and the algorithm goes on until it finds the pair.
    //  Side note: obviously, for every next point the overall distance becomes smaller.

    std::vector<PointAndDistance> PointAndDistanceList;
    for(auto const & point : TrainingSet)
    {
        // calculate the distance
        double distance = EuclideanDistanceBetweenTwoPoints(startingPoint, point);
        PointAndDistance tempPointAndDistance{point, distance};
        PointAndDistanceList.push_back(tempPointAndDistance);
    }
    std::sort(PointAndDistanceList.begin(), PointAndDistanceList.end());
    //  Everytime the first element on the list will be the same point as startingPoint (because distance from it to itself is 0),
    //  hence we skip it.
    for(int i=1; i < PointAndDistanceList.size(); ++i)
    {
        if(startingPoint.classLabel != PointAndDistanceList.at(i).point.classLabel)
        {
            if(lastDistance == PointAndDistanceList.at(i).distanceBetweenThisPointAndpointToClassify)
            {
                // the pair of mutually closest points was found
                return std::make_pair(startingPoint, PointAndDistanceList.at(i).point);
            }
            else
            {
                return FindPairOfMutuallyClosestPoints(PointAndDistanceList.at(i).point, PointAndDistanceList.at(i).distanceBetweenThisPointAndpointToClassify);
            }
        }
    }
    return std::make_pair(Point(),Point());
}

bool pointIsInTheSet(Point point, std::vector<Point> const & set)
{
    bool pointIsInTheSet = false;
    for(auto pointFromSet : set)
    {
        if(point == pointFromSet)
            return true;
    }
    return pointIsInTheSet;
}

std::vector<Point> MutuallyClosestPointsAlgorithm()
{
    std::vector<Point> preprocessedSet;

    for(auto const & point : TrainingSet)
    {
        std::pair<Point,Point> tempPair = FindPairOfMutuallyClosestPoints(point, -1);
        if(!pointIsInTheSet(tempPair.first, preprocessedSet))
            preprocessedSet.push_back(tempPair.first);
        if(!pointIsInTheSet(tempPair.second, preprocessedSet))
            preprocessedSet.push_back(tempPair.second);
    }
    return preprocessedSet;
}




int main() {

    int numOfInputValuesInOneRow;
    int numOfLines;

    std::fstream File("D:\\Projekty\\CPP\\CondensedNearestNeigtbourAlgorithm\\Test_s_epj.txt", std::ios::in);
    File >> numOfClasses >> numOfInputValuesInOneRow >> numOfLines;
    std::stringstream ss;
    ss << File.rdbuf();
    File.close();

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

    std::vector<Point> ReducedSet;
    bool allPointsWereClassifiedCorrectly = false;
    ReducedSet =  MutuallyClosestPointsAlgorithm();

    while(!allPointsWereClassifiedCorrectly) {

        allPointsWereClassifiedCorrectly = true;

        for (int k = 0; k < TrainingSet.size(); ++k) {

            if (!kNN_Algorithm(TrainingSet.at(k), ReducedSet, 1))
            {
                allPointsWereClassifiedCorrectly = false;
                ReducedSet.push_back(TrainingSet.at(k));
            }
        }
    }
    printline("Training set of", TrainingSet.size(), "points can be reduced to a smaller set of", ReducedSet.size(), "points.");

return 0;
}







