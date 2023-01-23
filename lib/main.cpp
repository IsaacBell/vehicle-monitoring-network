#include "obd2_dictionary.hpp"
#include "json_server.hpp"

int main()
{
  JsonServer server(12345);
  server.start();
}
