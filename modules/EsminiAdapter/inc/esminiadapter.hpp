#pragma once

#include "module.hpp"
#include "roschannel.hpp"
#include <unordered_map>
#include "util.h"
#include "esmini/esminiLib.hpp"
#include "trajectory.hpp"
#include "atos_interfaces/srv/get_test_origin.hpp"
#include "atos_interfaces/srv/get_object_trajectory.hpp"
#include "atos_interfaces/srv/get_start_on_trigger.hpp"

/*!
 * \brief The EsminiAdapter class is a singleton class that 
 * 	handles the communication between the esmini simulator and ATOS
 */
class EsminiAdapter : public Module {
public:
	static inline std::string const moduleName = "esmini_adapter";
	static inline std::string oscFilePath;
	static int initializeModule(const LOG_LEVEL logLevel);
	EsminiAdapter(EsminiAdapter const&) = delete;
    EsminiAdapter& operator=(EsminiAdapter const&) = delete;
	static std::shared_ptr<EsminiAdapter> instance();

private:
	EsminiAdapter();

	ROSChannels::ActionConfiguration::Pub accmPub;
	ROSChannels::ExecuteAction::Pub exacPub;
	ROSChannels::V2X::Pub v2xPub;
	ROSChannels::ConnectedObjectIds::Sub connectedObjectIdsSub;

	static std::unordered_map<uint32_t,std::shared_ptr<ROSChannels::Monitor::Sub>> monrSubscribers;
	static std::shared_ptr<rclcpp::Service<atos_interfaces::srv::GetObjectTrajectory>> objectTrajectoryService;
	static std::shared_ptr<rclcpp::Service<atos_interfaces::srv::GetStartOnTrigger>> startOnTriggerService;

	void onAbortMessage(const ROSChannels::Abort::message_type::SharedPtr) override;
	void onAllClearMessage(const ROSChannels::AllClear::message_type::SharedPtr) override;
	void onMonitorMessage(const ROSChannels::Monitor::message_type::SharedPtr monr, uint32_t id) override;
	void onInitMessage(const ROSChannels::Init::message_type::SharedPtr) override;
	void onStartMessage(const ROSChannels::Start::message_type::SharedPtr) override;
	void onExitMessage(const ROSChannels::Start::message_type::SharedPtr) override;
	static void onConnectedObjectIdsMessage(const ROSChannels::ConnectedObjectIds::message_type::SharedPtr msg);
	static void reportObjectPosition(const ROSChannels::Monitor::message_type::SharedPtr monr, uint32_t id);
	static void onEsminiStoryBoardStateChange(const char* name, int type, int state);
	static void onEsminiConditionTriggered(const char* name, double timestamp);
	static void onEsminiConditionTriggeredPre(const char* name, double timestamp);
	static void InitializeEsmini();
	static void getObjectStates(double timeStep, double endTime, std::map<uint32_t,std::vector<SE_ScenarioObjectState>>& states);
	static ATOS::Trajectory getTrajectory(uint32_t,std::vector<SE_ScenarioObjectState>& states);
	static std::map<uint32_t,ATOS::Trajectory> extractTrajectories(double timeStep, double endTime, std::map<uint32_t,ATOS::Trajectory>& idToTraj);
	static void onRequestObjectTrajectory(
		const std::shared_ptr<atos_interfaces::srv::GetObjectTrajectory::Request> req,
		std::shared_ptr<atos_interfaces::srv::GetObjectTrajectory::Response> res);
	
	static void onRequestStartOnTrigger(
		const std::shared_ptr<atos_interfaces::srv::GetStartOnTrigger::Request> req,
		std::shared_ptr<atos_interfaces::srv::GetStartOnTrigger::Response> res);

	static std::shared_ptr<rclcpp::Client<atos_interfaces::srv::GetTestOrigin>> testOriginClient;
	static std::shared_ptr<EsminiAdapter> me;
	static std::unordered_map<int, int> objectIdToIndex;
	static std::map<uint32_t,ATOS::Trajectory> idToTraj;
	static std::vector<uint32_t> delayedStartIds;
	const std::vector<std::string> supportedActions {"start", 
													 "send_denm"
													};
	static int actionId;
	static void testingFun();

};
