/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "module.hpp"
#include "roschannels/v2xchannel.hpp"
#include "roschannels/commandchannels.hpp"
#include "roschannels/pathchannel.hpp"
#include "roschannels/monitorchannel.hpp"
#include "roschannels/gnsspathchannel.hpp"
#include <unordered_map>
#include <filesystem>
#include "esmini/esminiLib.hpp"

#include "trajectory.hpp"
#include "atos_interfaces/srv/get_test_origin.hpp"
#include "atos_interfaces/srv/get_object_trajectory.hpp"
#include "atos_interfaces/srv/get_object_trigger_start.hpp"
#include "atos_interfaces/srv/get_object_ip.hpp"

/*!
 * \brief The EsminiAdapter class is a singleton class that 
 * 	handles the communication between the esmini simulator and ATOS
 */
class EsminiAdapter : public Module {
public:
	static inline std::string const moduleName = "esmini_adapter";
	static inline std::filesystem::path oscFilePath;
	static int initializeModule();
	EsminiAdapter(EsminiAdapter const&) = delete;
    EsminiAdapter& operator=(EsminiAdapter const&) = delete;
	static std::shared_ptr<EsminiAdapter> instance();

private:
	EsminiAdapter();

	ROSChannels::StartObject::Pub startObjectPub;
	ROSChannels::V2X::Pub v2xPub;
	ROSChannels::ConnectedObjectIds::Sub connectedObjectIdsSub;
	ROSChannels::Init::Sub initSub;
	ROSChannels::Start::Sub startSub;
	ROSChannels::Abort::Sub abortSub;
	ROSChannels::Exit::Sub exitSub;
	std::unordered_map<uint32_t,ROSChannels::Path::Pub> pathPublishers;
	std::unordered_map<uint32_t,ROSChannels::GNSSPath::Pub> gnssPathPublishers;

	static std::unordered_map<uint32_t,std::shared_ptr<ROSChannels::Monitor::Sub>> monrSubscribers;
	static std::shared_ptr<rclcpp::Service<atos_interfaces::srv::GetObjectTrajectory>> objectTrajectoryService;
	static std::shared_ptr<rclcpp::Service<atos_interfaces::srv::GetObjectTriggerStart>> startOnTriggerService;
	static std::shared_ptr<rclcpp::Service<atos_interfaces::srv::GetObjectIp>> objectIpService;


	void onMonitorMessage(const ROSChannels::Monitor::message_type::SharedPtr monr, uint32_t id);
	// Below is a quickfix, fix properly later
	static void onStaticInitMessage(const ROSChannels::Init::message_type::SharedPtr);
	static void onStaticStartMessage(const ROSChannels::Start::message_type::SharedPtr);
	static void onStaticAbortMessage(const ROSChannels::Abort::message_type::SharedPtr);
	static void onStaticExitMessage(const ROSChannels::Exit::message_type::SharedPtr);

	static void onConnectedObjectIdsMessage(const ROSChannels::ConnectedObjectIds::message_type::SharedPtr msg);
	static void reportObjectPosition(const ROSChannels::Monitor::message_type::SharedPtr monr, uint32_t id);
	static void executeActionIfStarted(const char* name, int type, int state);
	static std::filesystem::path getOpenScenarioFileParameter();
	static void setOpenScenarioFile(const std::filesystem::path&);
	static void handleStoryBoardElementChange(const char* name, int type, int state);
	static void handleActionElementStateChange(const char* name, int state);
	static void InitializeEsmini();
	static void getObjectStates(double timeStep, std::map<uint32_t,std::vector<SE_ScenarioObjectState>>& states);
	static ATOS::Trajectory getTrajectory(uint32_t,std::vector<SE_ScenarioObjectState>& states);
	static std::map<uint32_t,ATOS::Trajectory> extractTrajectories(double timeStep, std::map<uint32_t,ATOS::Trajectory>& idToTraj);
	static std::pair<uint32_t, std::string> parseAction(const std::string& action);
	static bool isStartAction(const std::string& action);
	static bool isSendDenmAction(const std::string& action);
	static void collectStartAction(const char* name, int type, int state);
	static ROSChannels::V2X::message_type denmFromMonitor(const ROSChannels::Monitor::message_type monr, double *llh);

	static void onRequestObjectTrajectory(
		const std::shared_ptr<atos_interfaces::srv::GetObjectTrajectory::Request> req,
		std::shared_ptr<atos_interfaces::srv::GetObjectTrajectory::Response> res);
	
	static void onRequestObjectStartOnTrigger(
		const std::shared_ptr<atos_interfaces::srv::GetObjectTriggerStart::Request> req,
		std::shared_ptr<atos_interfaces::srv::GetObjectTriggerStart::Response> res);
	
	static void onRequestObjectIP(
		const std::shared_ptr<atos_interfaces::srv::GetObjectIp::Request> req,
		std::shared_ptr<atos_interfaces::srv::GetObjectIp::Response> res);
	

	static std::shared_ptr<rclcpp::Client<atos_interfaces::srv::GetTestOrigin>> testOriginClient;
	static std::shared_ptr<EsminiAdapter> me;
	static std::unordered_map<int, int> ATOStoEsminiObjectId;
	static std::map<uint32_t,ATOS::Trajectory> idToTraj;
	static std::map<uint32_t,std::string> idToIp;
	static std::vector<uint32_t> delayedStartIds;

	static geographic_msgs::msg::GeoPose testOrigin;

};
