#ifndef PREDICTION_ASSISTANT_HPP_
#define PREDICTION_ASSISTANT_HPP_

#include "DistanceFunctions.hpp"
#include "PredictionDefinitions.hpp"
#include "KNeighborsFinder.hpp"
#include "DeadReckoning.hpp"

#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <chrono>

namespace prediction
{

class PredictionAssistant
{
public:
    using PredictionFunction =
        std::function<float(const DistanceFunctions*, const InputRecord&)>;
    using DirectionChooseFunction =
        std::function<DeltaRecord(const BasicKNeighborFinder::NeighborsVector& neighbors)>;
    using TestData = std::map<InputRecord, DeltaRecord>;

    PredictionAssistant(unsigned recordSize, unsigned k,
        PredictionFunction distanceFunc, DirectionChooseFunction chooseFunc);
    PredictionAssistant(unsigned recordSize, int recordsNumber, unsigned k,
        PredictionFunction distanceFunc, DirectionChooseFunction chooseFunc);
    ~PredictionAssistant();

    void addDatabaseFile(const std::string& file);
    void addTestFile(const std::string& file);

    void initPredictionAlgorithm();
    std::vector<bool> runTest();
    std::vector<bool> runDeadReckoningTest();

    double getElapsedTime() const;
private:
    TestData getTestData() const;
    std::pair<InputRecord, DeltaRecord> convertToTestData(const DataVector& raw) const;
    bool getTestResult(const std::pair<InputRecord, DeltaRecord>& test);
    bool getDeadReckoningTestResult(const std::pair<InputRecord, DeltaRecord>& test);

    FileVector databaseFiles_;
    FileVector testFiles_;

    const unsigned recordSize_;
    const int recordsNumber_;
    const unsigned k_;
    unsigned testDataSize_;
    const PredictionFunction distanceFunction_;
    const DirectionChooseFunction chooseFunction_;

    DistanceFunctions functions_;
    std::unique_ptr<BasicKNeighborFinder> finder_;
    DeadReckoning<InputRecord, DeltaRecord> deadReckoning_;
    std::chrono::duration<double> elapsedSec_;
};

}

#endif  // PREDICTION_ASSISTANT_HPP_