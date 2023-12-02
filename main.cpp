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

    std::vector<TCHAR *> f{(TCHAR *)"test_file"};

    BaseAPI<true, true> api(f);
    /*api.empty_file_BN(static_cast<const char *>("test_file"));

    assert(api.is_file_empty_BN(static_cast<const char *>("test_file")),
        "File did not get emptied.\n")*/

    return 0;
}