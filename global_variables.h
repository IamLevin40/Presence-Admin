#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <QString>

// Global variables
extern QString $adminId;
extern QString $adminPin;

// Global messages
namespace Messages
{
    inline QString emptyAdminId() { return "Admin number is required."; }
    inline QString invalidAdminId() { return "Admin number is invalid."; }
    inline QString emptyAdminPin() { return "Pin is required."; }
    inline QString invalidAdminPin() { return "Pin is invalid."; }
    inline QString loginSuccessful() { return "Successfully logged in."; }
}

#endif // GLOBAL_VARIABLES_H
