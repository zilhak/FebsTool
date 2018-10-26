#ifndef FEPSTOOL_RUNNINGTIMECHECKER_HPP
#define FEPSTOOL_RUNNINGTIMECHECKER_HPP

#include <wx/wx.h>

namespace util {

static wxDateTime time_lap_start;
void startCheckTime();
wxString endCheckTime(bool log_console = false);

} // namespace util


#endif //FEPSTOOL_RUNNINGTIMECHECKER_HPP
