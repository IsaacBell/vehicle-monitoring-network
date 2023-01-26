#include <boost/property_tree/ptree.hpp>
#include <cassandra.h>
#include <stdexcept>
#include <memory>
#include <type_traits>
#include <variant>
#include <string>

#include "diagnostic.hpp"
#include "vehicle.hpp"
#include "user.hpp"

/*
  auto db = Cassandra::get_instance();
  auto db = Cassandra::get_instance<true>();
*/
using VehicleParams = std::tuple<
    std::string_view,
    double,
    double,
    int>;

using UserParams = std::tuple<
    std::string,
    std::string,
    std::string,
    std::string,
    u_int8_t,
    double,
    double,
    std::string>;

template <bool isProd = false>
class Cassandra
{
public:
  // Singleton
  static Cassandra &getInstance()
  {
    static Cassandra instance;
    return instance;
  }

  Cassandra() : cluster_(cass_cluster_new()),
                session_(cass_session_new())
  {
    // session_.connect(host_, port_);
    connect_future_ = connect_future(cass_session_connect(session_, cluster_));
    cass_future_wait(connect_future_);
    CassError rc = cass_future_error_code(connect_future_);
    if (rc != CASS_OK)
    {
      // handle error
      throw std::runtime_error("could not start cassandra server");
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

  static User &findUser(std::string_view &uuid) { return find<User>("users.attributes", "uuid", uuid.data()); }
  
  template<typename T>
  static T &find(std::string_view &table, std::string_view &uuid) {
    CassCluster* cluster = cass_cluster_new();
    CassSession* session = cass_session_new();
    CassFuture* connect_future = cass_session_connect(session, cluster);
    cass_future_wait(connect_future);
    CassError connect_error = cass_future_error_code(connect_future);
    if (connect_error != CASS_OK) {
        // handle error
        cass_future_free(connect_future);
        cass_session_free(session);
        cass_cluster_free(cluster);
        return nullptr;
    }

    const auto tableName = "SELECT * FROM " + table.data() + " WHERE id = ?";
    CassStatement* statement = cass_statement_new(, 1);
    CassUuid id;
    cass_uuid_from_string(uuid.data(), &id);
    cass_statement_bind_uuid(statement, 0, id);
    CassFuture* query_future = cass_session_execute(session, statement);
    cass_future_wait(query_future);
    CassError query_error = cass_future_error_code(query_future);
    if (query_error != CASS_OK) {
        // handle error
        cass_future_free(query_future);
        cass_statement_free(statement);
        cass_future_free(connect_future);
        cass_session_free(session);
        cass_cluster_free(cluster);
        return nullptr;
    }

    const CassResult* result = cass_future_get_result(query_future);
    CassIterator* iterator = cass_iterator_from_result(result);
    User user;
    if (cass_iterator_next(iterator)) {
        const CassRow* row = cass_iterator_get_row(iterator);
        // populate object with row data
    } else {
        // handle case where not found
    }
    cass_iterator_free(iterator);
    cass_result_free(result);
    cass_future_free(query_future);
    cass_statement_free(statement);
    return user;
}

  static bool insert(Vehicle &vehicle)
  {

    VehicleParams params{vehicle.vehicleId(), vehicle.speed(), vehicle.engineRpm(), vehicle.fuelLevel()};

    return insertData("vehicles.diagnostics", params);
  }

  static bool insert(User &user)
  {
    UserParams params{user.uuid(), user.username(), user.email(), user.address(), user.address2(), user.lat(), user.lon(), user.phone()};

    return insertData("users.attributes", params);
  }

private:
  CassCluster cluster_;
  CassSession session_;
  CassFuture connect_future_;

  void init_tables()
  {
    createKeyspace("users");
    createKeyspace("vehicles");

    if (!create_table("users.attributes", "uuid text", "username text", "address text", "address2 text", "lat double", "lon double", "phone text"))
      throw std::runtime_error("could not instantiate table users.attributes");

    if (!create_table("vehicles.diagnostics", "vehicle_id text", "speed double", "engine_rpm double", "fuel_level int"))
      throw std::runtime_error("could not instantiate table vehicles.diagnostics");
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
  static bool insertData(std::string_view &table, Args... args)
  {
    std::string insert_query = "INSERT INTO " + table.data() + " VALUES (" + std::string(std::tuple_size<decltype(params)>::value, '?') + ")";
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
    if constexpr (isProd)
      create_keyspace_statement = cass_statement_new("CREATE KEYSPACE IF NOT EXISTS vehicles WITH replication = {'class': 'NetworkTopologyStrategy', 'replication_factor': 3};", 0);
    else
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
