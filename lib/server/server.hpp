#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::asio::io_service;
using boost::asio::ip::tcp;
using boost::property_tree::ptree;

#ifndef _SERVER_H_
#define _SERVER_H_

class Server
{
public:
  Server(int port) : service_(), ep_(tcp::v4(), port), acc_(service_, ep_) {}

  virtual ~Server() {}

  void start()
  {
    while (true)
    {
      tcp::socket sock(service_);
      acc_.accept(sock);
      std::thread(session, this, std::move(sock)).detach();
    }
  }
  virtual void save(ptree const &payload) = 0;

private:
  static void session(Server *server, tcp::socket sock)
  {
    try
    {
      std::vector<char> data(1024);
      boost::system::error_code error;

      size_t length = sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
        return; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::string json_string(data.begin(), data.begin() + length);
      std::stringstream ss;
      ss << json_string;
      ptree payload;
      read_json(ss, payload);

      server->save(payload);
    }
    catch (std::exception &e)
    {
      std::cerr << "Exception in thread: " << e.what() << "\n";
    }
  }
  io_service service_;
  tcp::endpoint ep_;
  tcp::acceptor acc_;
};

#endif
