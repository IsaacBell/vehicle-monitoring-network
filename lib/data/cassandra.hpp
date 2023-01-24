#include <cqlpp.h>
#include "diagnostic.hpp"
#include "vehicle.hpp"

/*
  auto db = Cassandra::get_instance();
  auto db = Cassandra::get_instance<true>();
*/

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

  bool save(Vehicle &v) { return save_or_update(v.currentDiagnostic(), v.currentDiagnostic().uuid(), "vehicles.diagnostics"); }
  bool save(Diagnostic &diag) { return save_or_update(diag, diag.uuid(), "vehicles.diagnostics"); }

  void insert_user(const User &user)
  {
    auto insert = insert_into(users_).value("username", user.username).value("email", user.email).value("address", user.address).value("lat", user.lat).value("lon", user.lon).value("id", user.uuid).value("phone", user.phone);
    session_.execute(insert);
  }

  User get_user(const std::string &uuid)
  {
    auto select = select_from(users_).where(users_["id"] == uuid);
    User user;
    session_.execute(select, user);
    return user;
  }

  void insert_diagnostic(const Diagnostic &diagnostic)
  {
    auto insert = insert_into(diagnostics_).value("vehicle_id", diagnostic.vehicleId).value("speed", diagnostic.speed).value("engine_rpm", diagnostic.engineRpm).value("fuel_level", diagnostic.fuelLevel).value("id", diagnostic.uuid);
    session_.execute(insert);
  }

  Diagnostics get_diagnostic(const std::string &uuid) const
  {
    auto select = select_from(diagnostics_).where(diagnostics_["id"] == uuid);
    Diagnostics diagnostic;
    session_.execute(select, diagnostic);
    return diagnostic;
  }

private:
  std::string_view host;
  std::string_view port;
  cqlpp::Session session_;
  cqlpp::Map<User> users_;
  cqlpp::Map<Diagnostics> diagnostics_;

  // Singleton
  Cassandra(const Cassandra &) = delete;
  Cassandra &operator=(const Cassandra &) = delete;

  Cassandra() : host_(getenv("CASSANDRA_HOST") || "127.0.0.1"),
                port_(getenv("CASSANDRA_PORT" || 9042))
  {
    session_.connect(host_, port_);

    cqlpp::Map<User>
        users_;
    users_["users"]["attributes"] = {
        {"username", &User::username},
        {"email", &User::email},
        {"address", &User::address},
        {"lat", &User::lat},
        {"lon", &User::lon},
        {"id", &User::uuid},
        {"phone", &User::phone}};

    cqlpp::Map<Diagnostic> diagnostics_;
    diagnostics_["vehicles"]["diagnostics"] = {
        {"vehicle_id", &Diagnostics::vehicleId},
        {"speed", &Diagnostics::speed},
        {"engine_rpm", &Diagnostics::engineRpm},
        {"fuel_level", &Diagnostics::fuelLevel},
        {"id", &Diagnostics::uuid}};
  }

  /*
    Examples:
    User user;
    user.username = "John Doe";
    user.email = "john.doe@example.com";
    save_or_update(user, user.uuid, "users.attributes");

    Diagnostics diagnostic;
    diagnostic.speed = 50;
    diagnostic.engineRpm = 2000;
    diagnostic.fuelLevel = 30;
    save_or_update(diagnostics, diagnostics.uuid, "vehicles.diagnostics");
  */
  template <typename RecordType, typename PrimaryKeyType>
  bool save_or_update(RecordType &record, PrimaryKeyType primary_key, const std::string_view &table)
  {
    auto map = cqlpp::Map<RecordType>();
    map[table] = {
        {"id", &RecordType::uuid}};
    // Check if the record already exists in the table
    auto select = select_from(map).where(map["id"] == primary_key);
    auto result = session_.execute(select);
    if (result.next())
    {
      auto update = update(map).set(map["id"] = primary_key);

      auto future = session_.execute(update);
      future.wait();
      if (future.error())
        return false; // todo - log error
      return true;
    }
    else
    {
      auto insert = insert_into(map).value("id", primary_key);

      auto future = session_.execute(insert);
      future.wait();
      if (future.error())
        return false; // todo - log error
      return true;
    }

    return false;
  }

  void Cassandra::create_keyspace(const std::string &keyspace)
  {
    std::string cql;
    if constexpr (isProd)
      cql = "CREATE KEYSPACE IF NOT EXISTS " + keyspace + " WITH replication = {'class': 'NetworkTopologyStrategy', 'replication_factor': '3' };";
    else
      cql = "CREATE KEYSPACE IF NOT EXISTS " + keyspace + " WITH replication = {'class': 'SimpleStrategy', 'replication_factor': '1' };";
    cqlpp::Future future = session.execute(cql);
    future.wait();
    if (future.error())
    {
      std::cout << "Error creating keyspace: " << future.error().message() << "\n";
    }
    else
    {
      std::cout << "Keyspace created successfully\n";
    }
  }

  void Cassandra::create_table(const std::string &table, const std::string &primary_key, cqlpp::DataType primary_key_type)
  {
    std::string cql = "CREATE TABLE IF NOT EXISTS " + table + "(" + primary_key + " " + primary_key_type.name() + " PRIMARY KEY);";
    cqlpp::Future future = session.execute(cql);
    future.wait();
    if (future.error())
    {
      std::cout << "Error creating table: " << future.error().message() << "\n";
    }
    else
    {
      std::cout << "Table created successfully\n";
    }
  }
};
