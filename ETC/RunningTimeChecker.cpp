#include <ETC/RunningTimeChecker.hpp>

namespace util {

void startCheckTime()
{
    time_lap_start.SetToCurrent();
}

wxString endCheckTime(bool log_console)
{
    wxDateTime time_lap_end;
    time_lap_end.SetToCurrent();

    wxTimeSpan span = time_lap_end.Subtract(time_lap_start);

    int hour = span.GetHours();
    int minute = span.GetMinutes();
    wxLongLong second = span.GetSeconds();
    wxLongLong mill = span.GetMilliseconds();

    wxString result = wxString::Format("%dh %dm %ds.%d", hour, minute, second, mill);

    if (log_console) {
        std::cout << result << std::endl;
    }

    return result;
}

} // namespace util