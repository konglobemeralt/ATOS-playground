#include <iostream>
#include <unistd.h>

#include "braketrigger.h"
#include "trigger.h"

#include "scenario.h"
#include "logging.h"
#include "util.h"

#define MODULE_NAME "ScenarioControl"

#define SCENARIO_FILE_PATH "path/to/file"

typedef enum {SUCCESS, FAILURE} ReturnCode_t;

/************************ Static functions ******************************************/
static ReturnCode_t initializeScenario(Scenario &s);


/************************ Main task ******************************************/
int main()
{
    COMMAND command = COMM_INV;
    char mqRecvData[MQ_MSG_SIZE];
    const struct timespec sleepTimePeriod = {0,10000000};
    struct timespec remTime;
    Scenario scenario;

    LogInit(MODULE_NAME,LOG_LEVEL_DEBUG);
    LogMessage(LOG_LEVEL_INFO, "Task running with PID: %u",getpid());

    // Initialize message bus connection
    while(iCommInit())
    {
        nanosleep(&sleepTimePeriod,&remTime);
    }

    while(true)
    {
        if (iCommRecv(&command,mqRecvData,MQ_MSG_SIZE,nullptr) < 0)
        {
            util_error("Message bus receive error");
        }

        switch (command) {
        case COMM_INIT:
            try {
                LogMessage(LOG_LEVEL_INFO, "Initializing scenario");
                scenario.initialize(SCENARIO_FILE_PATH);
            }
            catch (std::invalid_argument) { util_error("Invalid scenario file format"); }
            catch (std::ifstream::failure) { util_error("Unable to open scenario file <" SCENARIO_FILE_PATH ">"); }
            break;
        case COMM_INV:
            nanosleep(&sleepTimePeriod,&remTime);
            break;
        default:
            LogMessage(LOG_LEVEL_INFO,"Received command %u",command);
        }
    }

    return 0;
}

