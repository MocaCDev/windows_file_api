#ifndef WINDOWS_FILE_API_BASE_API_H
#define WINDOWS_FILE_API_BASE_API_H

/* The code only exists so long as the OS is Windows.
 * If the OS is not Windows, this API has no inherent value,
 * or usage.
 * 
 * This "wrapper" is also useful such that it restricts the amount of
 * similar checks we'll have to do in backend-related functionality.
 * */
#ifdef __WIN32
#include "common.hpp"
#include "backend/empty_file.hpp"

using namespace WindowsFileAPI_EmptyFile;

/* Base file where all API comes together.
 * No need to explicitly include other files,
 * `BaseAPI` will inherit all other code such that developers only have
 * to include `base_api.hpp` to restrict further includes.
 * */

namespace WindowsFileAPI_BaseAPI
{
    class BaseAPI : public EmptyFile
    {
    public:
        explicit BaseAPI()
            : EmptyFile()
        {
#ifdef WRONG_VERSION
           error("Wrong Version Of C++:\n\tRequired Version Is C++20: -std=c++20.\n")
#endif 
        }

        ~BaseAPI() = default;
    };
}
#else
#error "NO"
#endif

#endif