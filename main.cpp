#include "api/base_api.hpp"

using namespace WindowsFileAPI_BaseAPI;

/* For Testing Only. */

int main(int args, char *argv[])
{
    /* If the OS is not Windows, just return.
     * main function is for TESTING ONLY.
     * */
#ifndef __WIN32
    return 0;
#endif

    BaseAPI api;
    api.empty_file_BN(static_cast<const char *>("test_file"));

    return 0;
}