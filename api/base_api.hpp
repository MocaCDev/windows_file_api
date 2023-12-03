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
#include "backend/file_create.hpp"

using namespace WindowsFileAPI_FileCreateAndDelete;

/* Base file where all API comes together.
 * No need to explicitly include other files,
 * `BaseAPI` will inherit all other code such that developers only have
 * to include `base_api.hpp` to restrict further includes.
 * */

namespace WindowsFileAPI_BaseAPI
{
    class BaseAPI : public FileCreateAndDelete
    {
    public:
        explicit BaseAPI()
            : FileCreateAndDelete()
        {
            check_valid_version();
        }

        ~BaseAPI() = default;
    };
}
#else
#error "NO"
#endif

#endif