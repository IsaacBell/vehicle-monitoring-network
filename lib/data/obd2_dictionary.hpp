#include <string>
#include <iostream>

/*
    int main()
    {
        using CodeDict = Obd2Dictionary<std::string_view, std::string_view>;
        std::string_view code = "P0001";
        std::string_view description = CodeDict::lookup(code);
        std::cout << code << ": " << description << "\n";
    }
*/

// Singleton, compile-time lookup
// It's worth noting that the singleton pattern isn't really needed bcs. the method is static and the
// array is constexpr, so the method and array only exist at compile-time

template <typename Code = std::string_view, typename Description = std::string_view>
class Obd2Dictionary
{
private:
    static constexpr std::pair<Code, Description> codes[] = {
        {"P0001", "Fuel Volume Regulator Control Circuit/Open"},
        {"P0002", "Fuel Volume Regulator Control Circuit Range/Performance"},
        {"P0003", "Fuel Volume Regulator Control Circuit Low"},
        {"P0004", "Fuel Volume Regulator Control Circuit High"},
        {"P0005", "Fuel Shutoff Valve Control Circuit/Open"},
        {"P0006", "Fuel Shutoff Valve Control Circuit Low"},
        {"P0007", "Fuel Shutoff Valve Control Circuit High"},
        {"P0008", "Engine Position System Performance"},
        {"P0009", "Engine Position System Circuit Range/Performance"},
        {"P0010", "Engine Position System Circuit Low"},
        {"P0011", "Engine Position System Circuit High"}
        //...and so on
    };

    Obd2Dictionary() = default;

public:
    static constexpr Description lookup(const Code &code)
    {
        for (const auto &[dtc, description] : codes)
        {
            if (dtc == code)
            {
                return description;
            }
        }
        return {};
    }
};
