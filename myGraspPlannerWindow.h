//
// Created by adelelakour on 19.08.23.
//

#ifndef GRASPER_MYGRASPPLANNERWINDOWS_H
#define GRASPER_MYGRASPPLANNERWINDOWS_H
#include "GraspPlannerWindow.h"

class myGraspPlannerWindows : public GraspPlannerWindow{

public:
    myGraspPlannerWindows(std::string& robFile, std::string& eefName, std::string& preshape, std::string& objFile);

    ~myGraspPlannerWindows();

};


#endif //GRASPER_MYGRASPPLANNERWINDOWS_H
