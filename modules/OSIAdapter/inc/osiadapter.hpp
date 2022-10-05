#pragma once

#include <boost/asio.hpp>

#include "module.hpp"
#include "roschannel.hpp"
#include "osi_handler.hpp"
#include "unordered_map"
#include <chrono>

class OSIAdapter : public Module
{
  public:
    void initialize(const std::string& address = DEFAULT_ADDRESS,
                  const uint16_t port = DEFAULT_PORT,
                  const bool debug = DEFAULT_DEBUG_VALUE);
    OSIAdapter();
    ~OSIAdapter();


  private:
    static inline std::string const DEFAULT_ADDRESS = "127.0.0.1";
    constexpr static uint16_t DEFAULT_PORT = 55555;
    constexpr static bool DEFAULT_DEBUG_VALUE = false;
    constexpr static uint8_t QUALITY_OF_SERVICE = 10;
    constexpr static std::chrono::duration SEND_INTERVAL = std::chrono::milliseconds(500);

    static inline std::string const moduleName = "osi_adapter";

    void sendOSIData();
    std::vector<char> makeOSIMessage(const OsiHandler::GlobalObjectGroundTruth_t osiData);
    const OsiHandler::GlobalObjectGroundTruth_t makeTestOSIData();
    
    void onAbortMessage(const ROSChannels::Abort::message_type::SharedPtr) override;
    
    ROSChannels::ConnectedObjectIds::Sub connectedObjectIdsSub;	//!< Publisher to report connected objects
    rclcpp::TimerBase::SharedPtr timer;

    boost::asio::ip::tcp::endpoint endpoint;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
    std::shared_ptr<boost::asio::io_service> io_service;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;

    void setupTCPServer(boost::asio::ip::tcp::endpoint endpoint);
    void destroyTCPServer();
    void resetTCPServer(boost::asio::ip::tcp::endpoint endpoint);

    std::unordered_map<uint32_t,ROSChannels::Monitor::message_type> lastMonitors;
    std::unordered_map<uint32_t,std::shared_ptr<ROSChannels::Monitor::Sub>> monrSubscribers;

    void onConnectedObjectIdsMessage(const ROSChannels::ConnectedObjectIds::message_type::SharedPtr msg);
    void onMonitorMessage(const ROSChannels::Monitor::message_type::SharedPtr msg, uint32_t id);

    double linPosPrediction(double position, double velocity, double deltaT);
    void extrapolateMONR(const uint32_t id, const double deltaT);
};
