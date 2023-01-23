#include <boost/property_tree/ptree.hpp>
#include <cassandra.h>
#include <memory>

class Cassandra
{
public:
  Cassandra() : cluster(std::make_unique<CassCluster>(create_cluster())),
                session(std::make_unique<CassSession>(create_cluster()))
  {

    connect_future = std::make_unique<CassFuture>(cass_session_connect(session, cluster));
    cass_future_wait(connect_future);
    CassError rc = cass_future_error_code(connect_future);
    if (rc != CASS_OK)
    {
      // handle error
    }
    cass_future_free(connect_future);
  }

  ~Cassandra()
  {
    cass_statement_free(statement);
    cass_session_free(session);
    cass_cluster_free(cluster);
  }

  void saveDiagnostic(const boost::property_tree::ptree &json, const std::string &keyspace, const std::string &table)
  {
    std::stringstream ss;
    boost::property_tree::write_json(ss, json);
    std::string json_str = ss.str();

    auto statement = cass_statement_new("INSERT INTO " + keyspace + "." + table + " JSON ?", 1);
    CassError rc = cass_statement_bind_string(statement, 0, json_str.c_str());
    if (rc != CASS_OK)
    {
      // handle error
    }

    auto query_future = cass_session_execute(session, statement);
    cass_future_wait(query_future);
    rc = cass_future_error_code(query_future);
    if (rc != CASS_OK)
    {
      // handle error
    }
    cass_future_free(query_future);
  }

  void insert(Vehicle &vehicle)
  {
    const char *insert_query = "INSERT INTO vehicles (make, model) VALUES (?, ?)";
    CassStatement *insert_statement = cass_statement_new(insert_query, 2);

    /* Create a vehicle object */
    Vehicle v;
    v.make = "Toyota";
    v.model = "Camry";

    /* Bind the values to the insert statement */
    cass_statement_bind_string(insert_statement, 0, v.make.c_str());
    cass_statement_bind_string(insert_statement, 1, v.model.c_str());

    /* Execute the insert statement */
    CassFuture *insert_future = cass_session_execute(session, insert_statement);

    /* Wait for the insert to complete */
    cass_future_wait(insert_future);

    /* Handle any insert errors */
    CassError insert_error = cass_future_error_code(insert_future);
    if (insert_error != CASS_OK)
    {
      std::cerr << "Unable to insert data: " << cass_error_desc(insert_error) << std::endl;
      return 1;
    }

    /* Clean up */
    cass_future_free(insert_future);
    cass_statement_free(insert_statement);
  }

private:
  std::unique_ptr<CassCluster> cluster;
  std::unique_ptr<CassSession> session;
  std::unique_ptr<CassFuture> connect_future;
}
