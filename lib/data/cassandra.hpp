#include <boost/property_tree/ptree.hpp>
#include <cassandra.h>
#include <stdexcept>
#include <memory>
#include <type_traits>
#include <variant>
#include <string>

class Cassandra
{
public:
  Cassandra() : cluster_(cass_cluster_new()),
                session_(cass_session_new())
  {
    connect_future_ = connect_future(cass_session_connect(session_, cluster_));
    cass_future_wait(connect_future_);
    CassError rc = cass_future_error_code(connect_future_);
    if (rc != CASS_OK)
    {
      // handle error
    }

    init_tables();
  }

  ~Cassandra()
  {
    cass_statement_free(statement);
    cass_session_free(session_);
    cass_cluster_free(cluster_);
    cass_future_free(connect_future_);
  }

  static bool insert(Vehicle &vehicle)
  {
    std::tuple<
        std::string_view,
        double,
        double,
        int>
        params{vehicle.vehicleId(), vehicle.speed(), vehicle.engineRpm(), vehicle.fuelLevel()};

    return insertData(params);
  }

private:
  CassCluster cluster_;
  CassSession session_;
  CassFuture connect_future_;

  void init_tables()
  {
    createKeyspace("vehicles");
    if (!create_table("vehicles.diagnostics", "vehicle_id text", "speed double", "engine_rpm double", "fuel_level int"))
      throw std::runtime_error("could not instantiate table vehicle.diagnostics");
  }

  /*

    Example:
    std::tuple<int, std::string, double, std::string> params = { 1, "hello", 2.5, "2022-01-01 00:00:00"};
    Cassandra.insertData(params);


    Make sure that the number of elements in the tuple is the same
    as the number of columns in the table and
    the types of the elements match the types of the columns in the table,
    otherwise you will get a type mismatch error
  */
  template <typename... Args>
  static bool insertData(Args... args)
  {
    std::string insert_query = "INSERT INTO vehicles VALUES (" + std::string(std::tuple_size<decltype(params)>::value, '?') + ")";
    CassStatement *insert_statement = cass_statement_new(insert_query.c_str(), std::tuple_size<decltype(params)>::value);
    std::apply([&](auto &&...args)
               {
    int i = 0;
    (std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            cass_statement_bind_int32(insert_statement, i, arg);
        }
        else if constexpr (std::is_same_v<T, double>) {
            cass_statement_bind_double(insert_statement, i, arg);
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            cass_statement_bind_string(insert_statement, i, arg.c_str());
        }
        else if constexpr (std::is_same_v<T, std::string_view>) {
            cass_statement_bind_string(insert_statement, i, arg.c_str());
        }
        ++i;
    }, args)... },
               params);

    CassFuture *insert_future = cass_session_execute(session_, insert_statement);

    /* Wait for the insert to complete */
    cass_future_wait(insert_future);

    /* Handle any insert errors */
    CassError insert_error = cass_future_error_code(insert_future);
    if (insert_error != CASS_OK)
    {
      std::cerr << "Unable to insert data: " << cass_error_desc(insert_error) << "\n";
      return false;
    }

    return true;
  }

  /*
    Example:
    std::string_view table = "vehicle.diagnostics";
    create_table(table, "vehicle_id text", "speed double", "engine_rpm double", "fuel_level int");
  */
  template <typename... Args>
  static bool createTable(std::string_view &table, Args... args)
  {
    std::string columns = (args + ", ")...;
    columns.pop_back();
    columns.pop_back();
    std::string create_table_query = "CREATE TABLE IF NOT EXISTS " + table.data() + " (id uuid PRIMARY KEY, " + columns + ");";
    CassStatement *create_table_statement = cass_statement_new(create_table_query.c_str(), 0);
    CassFuture *create_table_future = cass_session_execute(session, create_table_statement);
    /* Wait for the query to complete */
    cass_future_wait(create_table_future);

    /* Check for errors */
    CassError create_table_error = cass_future_error_code(create_table_future);
    if (create_table_error != CASS_OK)
    {
      std::cerr << "Unable to create table: " << cass_error_desc(create_table_error) << std::endl;
      return false;
    }
    /* Clean up */
    cass_future_free(create_table_future);
    cass_statement_free(create_table_statement);
    return false;
  }

  static bool createKeyspace(std::string_view &keyspace)
  {
    CassFuture *create_keyspace_future = NULL;
    CassStatement *create_keyspace_statement = NULL;
    create_keyspace_statement = cass_statement_new("CREATE KEYSPACE IF NOT EXISTS vehicles WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1};", 0);
    create_keyspace_future = cass_session_execute(session, create_keyspace_statement);
    cass_future_wait(create_keyspace_future);

    /* Check for errors */
    CassError create_keyspace_error = cass_future_error_code(create_keyspace_future);
    if (create_keyspace_error != CASS_OK)
    {
      std::cerr << "Unable to create keyspace: " << cass_error_desc(create_keyspace_error) << "\n";
      return false;
    }

    /* Clean up */
    cass_future_free(create_keyspace_future);
    cass_statement_free(create_keyspace_statement);
    return false;
  }
}
