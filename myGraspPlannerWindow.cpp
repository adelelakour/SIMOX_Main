//
// Created by adelelakour on 19.08.23.
//

#include <nlohmann/json.hpp>
#include "myGraspPlannerWindow.h"
#include <fstream>
using json = nlohmann::json;




myGraspPlannerWindow::myGraspPlannerWindow(std::string& robFile, std::string& eefName, std::string& preshape, std::string& objFile)
:GraspPlannerWindow( robFile, eefName, preshape, objFile)
{

}

myGraspPlannerWindow::~myGraspPlannerWindow() {
    // Destructor implementation, if needed
}



void myGraspPlannerWindow::save_to_json(std::string objName, std::array<Vector3f, 4>  ConPoints)
{
    json newGraspPoints;
    for (const auto& vector : ConPoints) {
        newGraspPoints.push_back(json::array({vector[0], vector[1], vector[2]}));
    }

    std::ifstream inputFile("Database.json");
    json jsonData = json::parse(inputFile);
    inputFile.close();

    std::string objectName = objName; // Specify the object name

    std::string lastKey;
    for (auto it = jsonData[objectName].begin(); it != jsonData[objectName].end(); ++it) {
        lastKey = it.key();
    }

    // Check if the object exists in the JSON
    if (jsonData.contains(objectName)) {
        // Check if the last grasp key exists for the object
        if (jsonData[objectName].contains(lastKey)) {
            // Increment the last grasp key to create a new key (e.g., grasp4)
            int newGraspNumber = std::stoi(lastKey.substr(5)) + 1;
            std::string newGraspKey = "grasp" + std::to_string(newGraspNumber);

            // Create a new entry for the new grasp key and copy the last grasping points
            jsonData[objectName][newGraspKey] = newGraspPoints;

            // Output the updated JSON
            std::ofstream outputFile("Database.json");
            if (outputFile.is_open()) {
                outputFile << jsonData.dump(4); // Indent with 4 spaces for better readability
                outputFile.close();
                std::cout << "New grasp added: " << newGraspKey << std::endl;
            } else {
                std::cerr << "Failed to save JSON file." << std::endl;
            }
        } else {
            std::cerr << "Last grasp key not found for the object." << std::endl;
        }
    } else {
        std::cerr << "Object not found in JSON." << std::endl;
    }

}


void myGraspPlannerWindow::closeEEF ()
{
    contacts.clear();

    if (eefCloned && eefCloned->getEndEffector(eefName))
    {
        contacts = eefCloned->getEndEffector(eefName)->closeActors(object);

        // these four lines are writen by me

        objectName = object->getName();
        ContactPoints[0] = contacts[0].contactPointObstacleLocal;
        ContactPoints[1] = contacts[1].contactPointObstacleLocal;
        ContactPoints[2] = contacts[2].contactPointObstacleLocal;
        ContactPoints[3] = contacts[3].contactPointObstacleLocal;


        save_to_json(objectName, ContactPoints);



        float qual = qualityMeasure->getGraspQuality();
        bool isFC = qualityMeasure->isGraspForceClosure();
        std::stringstream ss;
        ss << std::setprecision(3);
        ss << "Grasp Nr " << grasps->getSize() << "\nQuality: " << qual << "\nForce closure: ";

        if (isFC)
        {
            ss << "yes";
        }
        else
        {
            ss << "no";
        }

        UI.labelInfo->setText(QString(ss.str().c_str()));
    }

    buildVisu();
}
